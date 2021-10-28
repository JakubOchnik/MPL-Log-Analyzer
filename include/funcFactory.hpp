#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>
#include <consts/consts.hpp>
#include <inputArgHandler.hpp>
#include <line_types/logLine.hpp>
#include <algorithm>
#include <functional>

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

using boolFunc = std::function<bool(std::string, std::shared_ptr<LogLine>&)>;

class FunctionFactory
{
public:

    static std::vector<std::function<bool(std::string,  std::shared_ptr<LogLine>&)>> createFiltersFromArgs(std::vector<KeyOperation> ops)
    {
        std::vector<std::function<bool(std::string,  std::shared_ptr<LogLine>&)>> filters;
        for(auto& entry: ops)
        {
            if(entry.op == consts::FilterOperationSpecifier::eq)
            {
                filters.push_back(getEqualFilterVisitor(entry.key));
            }
            else
            {
                filters.push_back(getContainsFilterVisitor(entry.key));
            }
        }
        return filters;
    }

    static auto getEqualFilter(std::string& key, std::shared_ptr<LogLine>& line) 
    {
         switch(line->getLineParameter(key).index())
         {
            case 0:
                return std::function<bool(std::string, std::shared_ptr<LogLine>&)> {
                    [key](std::string value, std::shared_ptr<LogLine>& line){
                        boost::to_lower(value);
                        return std::get<std::string>(line->getLineParameter(key)) == value;
                    }
                };
            case 1:
                return std::function<bool(std::string, std::shared_ptr<LogLine>&)> {
                    [key](std::string value, std::shared_ptr<LogLine>& line){
                    // NOT IMPLEMENTED YET
                        return false;
                    }
                };
            case 2:
                return std::function<bool(std::string, std::shared_ptr<LogLine>&)> {
                    [key](std::string value, std::shared_ptr<LogLine>& line){
                        return consts::LineTypeMap.at(value) == std::get<consts::LineType>(line->getLineParameter(key));
                    }
                };
            case 3:
                return std::function<bool(std::string, std::shared_ptr<LogLine>&)> {
                    [key](std::string value, std::shared_ptr<LogLine>& line){
                        return std::get<long long>(line->getLineParameter(key)) == std::stoll(value);
                    }
                };
            default:
                return std::function<bool(std::string, std::shared_ptr<LogLine>&)> {
                    [key](std::string value, std::shared_ptr<LogLine>& line){
                            return false;
                    }
                };
         };
    };


    static std::function<bool(std::string, std::shared_ptr<LogLine>&)> getEqualFilterVisitor(const std::string& key)
    {
        // variant
        LogLine dummyBase;
        auto val = dummyBase.getLineParameter(key);
        return std::visit(overload{
            [key](std::string&) {
                return std::function<bool(std::string, std::shared_ptr<LogLine>&)> {
                    [key](std::string value, std::shared_ptr<LogLine>& line){
                        boost::to_lower(value);
                        return std::get<std::string>(line->getLineParameter(key)) == value;
                    }
                };
            },
            [key](consts::LineType&) {
                return std::function<bool(std::string, std::shared_ptr<LogLine>&)> {
                    [key](std::string value, std::shared_ptr<LogLine>& line){
                        boost::to_upper(value);
                        return consts::LineTypeMap.at(value) == std::get<consts::LineType>(line->getLineParameter(key));
                    }
                };
            },
            [key](long long&) {
                return std::function<bool(std::string, std::shared_ptr<LogLine>&)> {
                    [key](std::string value, std::shared_ptr<LogLine>& line){
                        return std::get<long long>(line->getLineParameter(key)) == std::stoll(value);
                    }
                };
            },
            [key](pt&) {
                return std::function<bool(std::string, std::shared_ptr<LogLine>&)> {
                    [key](std::string value, std::shared_ptr<LogLine>& line){
                        return false;
                    }
                };
            },
        }, val);
    };

    static std::function<bool(std::string, std::shared_ptr<LogLine>&)> getContainsFilterVisitor(const std::string& key)
    {
        LogLine dummyBase;
        auto val = dummyBase.getLineParameter(key);
        return std::visit(overload{
            [key](std::string&) {
                return std::function<bool(std::string, std::shared_ptr<LogLine>&)> {
                    [key](std::string value, std::shared_ptr<LogLine>& line){
                        return boost::algorithm::contains(std::get<std::string>(line->getLineParameter(key)), value);
                    }
                };
            },
            [](consts::LineType&){return boolFunc{[](std::string, std::shared_ptr<LogLine>&){return false;}};},
            [](long long&){return boolFunc{[](std::string, std::shared_ptr<LogLine>&){return false;}};},
            [](pt&){return boolFunc{[](std::string, std::shared_ptr<LogLine>&){return false;}};},
        }, val);
    };


};