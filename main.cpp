#include <iostream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <memory>
#include <variant>
#include <optional>

#include <inputArgHandler.hpp>
#include <consts/consts.hpp>
#include <funcFactory.hpp>
#include <helpCommands.hpp>
#include <line_types/logLine.hpp>
#include <line_types/baseLine.hpp>
#include <line_types/internalEntry.hpp>
#include <helpCommands.hpp>
#include <lib/utils.hpp>
#include <lib/castUtils.hpp>

using namespace std;

int main(int argc, char* argv[])
{
    InputArgHandler cmdline(argc, argv);
    auto initialArgs = cmdline.parseStartArgs();

    if(initialArgs.isHelp)
    {
        HelpCommands::displayHelpCmd();
        return 0;
    }

    auto conditionArgs = cmdline.parseConditionArgs(initialArgs.condStartIndex);
    
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
    
    // Iterate over lines
    for(const auto& elem: db)
    {
        std::shared_ptr<InternalEntry> errorInstance;
        // if lineInstance is a nullptr, it means that the entry it is an errorInstance
        std::shared_ptr<LogLine> lineInstance = CastUtils::downcast<LogLine>(elem);
        if(lineInstance == nullptr)
        {
            errorInstance = CastUtils::downcast<InternalEntry>(elem);
            // nothing works for errorInstance yet, it's only stored
        }
        bool fail{false};
        // filters don't work for internal errors yet
        // also, OR mode doesn't work yet
        if(lineInstance)
        {
            // AND mode: iterate over filters vector, check every one of them for each line
            for(int i{0}; i<filters.size(); ++i)
            {
                if(!filters[i](conditionArgs[i].value, lineInstance))
                {
                    // If any filter doesn't match, break and don't continue
                    fail = true;
                    break;
                }
            }
        }
        if(!fail && lineInstance)
        {
            // if there was no fail, print the line
            std::cout << lineInstance->getFormattedOutput() << "\n";
        }
    }
    return 0;
}