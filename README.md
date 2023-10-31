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

    // 1. register settings (with default values) and flags
    clom.register_setting("name", "Mr X");
    clom.register_setting("height", "6.0");
    clom.register_flag("--smart");

    // 2. let clom process the options
    clom.process_cl_options(argc, argv);

    // 3. save and print the settings
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
$ my-app
Mr X is 6 foot tall and is not smart.
```
```
$ my-app name Mark --smart height 5.2
Mark is 5.2 foot tall and is smart.
```
```
$ my-app name Mark age 30
Unknown option: age
Invalid command line options!
```

### Functions
Here are all public functions of the class CL_Option_Manager:
``` C++
// Constructor
CL_Option_Manager();

// Options not registered this way are treated as invalid!
// A 'setting' is an option with a value
void register_setting(std::string name, std::string default_value);
// A 'flag' is an option that is either set or not set (boolean)
void register_flag(std::string name);

// Scan argv for registered options
void process_cl_options(int argc, char const *argv[]);

// Get the value of options (need to be called after process_cl_options())
std::string get_setting_value(std::string name);
bool is_flag_set(std::string name);

// Set and print the help/hint text (eg. "Usage: my-app [options]" etc.)
// The default hint is "Invalid command line options!",
// so you might want to set a custom one.
void set_user_hint(std::string hint);
void print_user_hint(); // Does not exit automatically
```
