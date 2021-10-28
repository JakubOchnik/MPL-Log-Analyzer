#include <iostream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <inputArgHandler.hpp>
#include <consts/consts.hpp>
#include <helpCommands.hpp>
#include <fstream>
#include <memory>
#include <variant>
#include <optional>
#include <funcFactory.hpp>

#include <line_types/logLine.hpp>
#include <line_types/baseLine.hpp>
#include <line_types/internalEntry.hpp>

#include <lib/utils.hpp>
#include <lib/castUtils.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using pt = boost::posix_time::ptime;


#define FILENAME "mpl.log"
using namespace std;
int main(int argc, char* argv[])
{
    InputArgHandler cmdline(argc, argv);
    auto initialArgs = cmdline.parseStartArgs();

    if(initialArgs.isHelp)
    {
        // display help
        return 0;
    }
    // do sth with them

    auto conditionArgs = cmdline.parseConditionArgs(initialArgs.condStartIndex);
    
    // is_request

    std::ifstream file(initialArgs.filePath);

    std::vector<std::shared_ptr<BaseLine>> db;
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if(!Utils::isInternalStart(line) && line.size() > 0)
            {
                db.emplace_back(new LogLine(line));
            }
            else if(line.size() > 0)
            {
                db.emplace_back(new InternalEntry(line, file));
            }
        }
        file.close();
    }

    //std::vector<std::string> keys = {"source", "type", "message"};
    //std::vector<std::string> vals = {"MPL", "ERROR", "fp-1"};

    //sample line
    std::vector<std::function<bool(std::string, std::shared_ptr<LogLine>&)>> filters;
    filters = FunctionFactory::createFiltersFromArgs(conditionArgs);
    if(!filters.size())
    {
        // If there are no filters, match all (just display all logs)
        auto trueFilter = std::function<bool(std::string, std::shared_ptr<LogLine>&)> {
                    [](std::string value, std::shared_ptr<LogLine>& line){
                        return true;
                    }
                };
        filters.push_back(trueFilter);
        conditionArgs.emplace_back("",consts::FilterOpSpecMap.at(std::string("-c").substr(1)), "");
    }
    
    for(const auto& elem: db)
    {
        std::shared_ptr<InternalEntry> errorInstance;
        std::shared_ptr<LogLine> lineInstance = CastUtils::downcast<LogLine>(elem);

        if(lineInstance == nullptr)
        {
            errorInstance = CastUtils::downcast<InternalEntry>(elem);
            // everything for errorInstance
        }
        bool fail{false};
        // filters don't work for internal errors yet
        if(lineInstance)
        {
            for(int i{0}; i<filters.size(); ++i)
            {
                if(!filters[i](conditionArgs[i].value, lineInstance))
                {
                    fail = true;
                    break;
                }
            }
        }
        if(!fail && lineInstance)
        {
            std::cout << lineInstance->getFormattedOutput() << "\n";
        }
    }

    return 0;
}