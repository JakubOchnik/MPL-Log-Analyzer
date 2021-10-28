#include <helpCommands.hpp>

void HelpCommands::displayHelpCmd()
{
    std::cout << "MPL Log Analyzer\nVersion: 1.0.0\n" << textConsts::helpText;
}

void HelpCommands::displayVersionCmd()
{
    std::cout << "Mpl Log Analyzer\n Version: 1.0.0\n";
}