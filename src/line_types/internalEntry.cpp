#include <line_types/internalEntry.hpp>

InternalEntry::InternalEntry(std::string& rawLine, std::ifstream& file)
{
    parseError(rawLine, file);
}

void InternalEntry::parseError(std::string& rawLine, std::ifstream& file)
{
    message = rawLine;
    std::string line;
    bool changedPos;
    while (std::getline(file, line) && line.size() > 0 && Utils::isInternalLine(line)) 
    {
        message += line;
        changedPos = true;
    }
    //
    if(changedPos)
    {
        file.seekg(file.tellg()-line.size()-1);
    }

}