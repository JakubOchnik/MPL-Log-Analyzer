#pragma once
#include <iostream>
#include <vector>
#include <string>

#include <helpCommands.hpp>
#include <consts/consts.hpp>
#include <boost/algorithm/string.hpp>


struct KeyOperation
{
    KeyOperation(std::string k, consts::FilterOperationSpecifier o, std::string v):key(k), op(o), value(v){};
    const std::string key;
    const consts::FilterOperationSpecifier op;
    const std::string value;
};

struct StartParams
{
    StartParams(std::string path, consts::CondConcatMode con, bool help, size_t i):filePath(path), mode(con), isHelp(help), condStartIndex(i){};
    const std::string filePath;
    const consts::CondConcatMode mode;
    const bool isHelp;
    size_t condStartIndex;
};

class InputArgHandler
{
private:
    const int argCount;
    char** args;
public:
    InputArgHandler(int ac, char* av[]);

    KeyOperation parseCondition(const std::string& key, const std::string& op, const std::string& val);
    std::vector<KeyOperation> parseConditionArgs(size_t condStartIndex);
    StartParams parseStartArgs();

};

