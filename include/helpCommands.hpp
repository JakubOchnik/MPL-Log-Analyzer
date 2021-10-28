#pragma once
#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

class HelpCommands
{
public:
    static void displayHelpCmd(const po::options_description& desc);
    static void displayVersionCmd();
};