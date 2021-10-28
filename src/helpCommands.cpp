#include <helpCommands.hpp>

void HelpCommands::displayHelpCmd(const po::options_description& desc)
{
    std::cout << "MPL Log Parser\n" << desc;
}

void HelpCommands::displayVersionCmd()
{
    std::cout << "Mpl Log Parser\n Version: 1.0.0\n";
}