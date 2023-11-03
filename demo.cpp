/*
 * This demonstration shows how one could use the CLOM library.
 * In this case, we can set the name, height and smartness
 * of our subject using command line options.
 * Example:
 * $ MyApp name Mark height 5.2 --smart
 * Result:
 * Mark is 5.2 foot tall and is smart.
*/

#include "clom.hpp"
#include <iostream>

int main(int argc, char const *argv[]) {

    // create the clom object
    CL_Option_Manager clom;

    // 1. register settings (with default values) and flags
    clom.register_setting<std::string>("name", "Mr X");
    clom.register_setting<float>("height", 6.0f);
    clom.register_flag("--smart");

    // 2. let clom process the options
    clom.process_cl_options(argc, argv);

    // 3. save and print the options
    std::string name = clom.get_setting_value<std::string>("name");
    float height = clom.get_setting_value<float>("height");
    bool is_smart = clom.is_flag_set("--smart");

    std::cout << name << " is " << height << " foot tall ";
    std::cout << "and is " << (is_smart ? "smart." : "not smart.") << '\n';

    return 0;
}
