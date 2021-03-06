#include <inputArgHandler.hpp>

InputArgHandler::InputArgHandler(const int ac, char* av[]): argCount(ac), args(av)
{

}

std::vector<KeyOperation> InputArgHandler::parseConditionArgs(size_t condStartIndex)
{
    std::vector<KeyOperation> operations;
    std::string key, op, val;
    for(size_t i{condStartIndex}; i < argCount; i+=4)
    {
        size_t cur_ind{i};
        if(args[i] == consts::condInputParams::KEY && i+1 < argCount)
        {
            key = args[i+1];
        }
        else
        {
            // Throw exception
        }

        if(i+3 < argCount)
        {
            auto parsedOp = Utils::parseOperation(args[i+2]);
            if(parsedOp.coreOp.empty())
            {
                continue;
            }
            auto op = consts::PairParamMap.at(parsedOp.coreOp);
            operations.emplace_back(key, op, args[i+3], parsedOp.neg, parsedOp.equalGl);
        }
    }
    return operations;
}

StartParams InputArgHandler::parseStartArgs()
{
    std::string filePath;
    consts::CondConcatMode concatMode{consts::CondConcatMode::concat_and};

    size_t i{1};
    while(i < argCount && args[i] != consts::condInputParams::KEY)
    {
        if(args[i] == consts::startInputParams::HELP)
        {
            return StartParams{"",consts::CondConcatMode::concat_and, true, 0};
        }
        else if(args[i] == consts::startInputParams::INPUT && filePath.empty())
        {
            if(filePath.empty())
            {
                // already specified
            }
            if(i > argCount - 2)
            {
                // no input was specified, ran out of args
            }
            ++i;
            filePath = args[i];
        }
        else if(args[i] == consts::startInputParams::MODE_AND)
        {
            concatMode = consts::CondConcatMode::concat_and;
        }
        else if(args[i] == consts::startInputParams::MODE_OR)
        {
            concatMode = consts::CondConcatMode::concat_or;
        }
        ++i;
    }

    if(filePath.empty())
    {
        // error: filePath cannot be empty
    }
    //args
    
    return StartParams{filePath, concatMode, false, i};
}