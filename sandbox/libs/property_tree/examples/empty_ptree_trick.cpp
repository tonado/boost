// ----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <iostream>
#include <iomanip>
#include <string>

using namespace boost::property_tree;

// Process settings using empty ptree trick
void process_settings(const std::string &filename)
{
    ptree pt;
    read_info(filename, pt);    
    const ptree *settings = pt.get_child_d("settings", empty_ptree<ptree>());
    std::cout << "\n    Processing " << filename << std::endl;
    std::cout << "        Setting 1 is " << settings->get_d("setting1", 0) << std::endl;
    std::cout << "        Setting 2 is " << settings->get_d("setting2", 0.0) << std::endl;
    std::cout << "        Setting 3 is " << settings->get_d("setting3", "default") << std::endl;
}

// Process settings not using empty ptree trick
void process_settings_without_trick(const std::string &filename)
{
    ptree pt;
    read_info(filename, pt);    
    ptree *settings;
    if (pt.get_child_b("settings", &settings))
    {
        std::cout << "\n    Processing " << filename << std::endl;
        std::cout << "        Setting 1 is " << settings->get_d("setting1", 0) << std::endl;
        std::cout << "        Setting 2 is " << settings->get_d("setting2", 0.0) << std::endl;
        std::cout << "        Setting 3 is " << settings->get_d("setting3", "default") << std::endl;
    }
    else
    {
        std::cout << "\n    Processing " << filename << std::endl;
        std::cout << "        Setting 1 is " << 0 << std::endl;
        std::cout << "        Setting 2 is " << 0.0 << std::endl;
        std::cout << "        Setting 3 is " << "default" << std::endl;
    }
}

int main()
{
    try
    {
        std::cout << "Processing settings with empty-ptree-trick:\n";
        process_settings("settings_fully-existent.info");
        process_settings("settings_partially-existent.info");
        process_settings("settings_non-existent.info");
        std::cout << "\nProcessing settings without empty-ptree-trick:\n";
        process_settings_without_trick("settings_fully-existent.info");
        process_settings_without_trick("settings_partially-existent.info");
        process_settings_without_trick("settings_non-existent.info");
    }
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << "\n";
    }
    return 0;
}
