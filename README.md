# CLOM
Yet another Command Line Option Manager for C++. Use this single header library if you think Boost would be overkill.

---

### Usage
Simply include `clom.hpp` in your source code.  
Then, you just need to do the following steps:  
- Create the clom object
- Register all settings and flags
- Let clom process the command line options
- Use the settings however you like

### Example
Here is an example of how to use clom:
``` C++
#include "clom.hpp"
#include <iostream>

int main(int argc, char const *argv[]) {

    // create the clom object
    CL_Option_Manager clom;

    // register settings (with default values) and flags
    clom.register_setting("name", "Mr X");
    clom.register_setting("height", "6.0");
    clom.register_flag("--smart");

    // let clom process the options
    clom.process_cl_options(argc, argv);

    // save and print the settings
    std::string name = clom.get_setting_value("name");
    float height = std::stof(clom.get_setting_value("height"));
    bool is_smart = clom.is_flag_set("--smart");

    std::cout << name << " is " << height << " foot tall ";
    std::cout << "and is " << (is_smart ? "smart." : "not smart.") << '\n';

    return 0;
}
```
Here are possible results:
```
$ my_app
Mr X is 6 foot tall and is not smart.
```
```
$ my_app name Mark --smart height 5.2
Mark is 5.2 foot tall and is smart.
```
```
$ my_app name Mark age 30
Unknown option: age
Invalid command line options!
```
