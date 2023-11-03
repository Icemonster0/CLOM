# CLOM
Yet another Command Line Option Manager for C++. Use this single header library if you think Boost would be overkill.

---

### Usage
Simply include [clom.hpp](clom.hpp) in your source code.  
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
    clom.register_setting<std::string>("name", "Mr X");
    clom.register_setting<float>("height", 6.0f);
    clom.register_flag("--smart");

    // 2. let clom process the options
    clom.process_cl_options(argc, argv);

    // 3. save and print the settings
    std::string name = clom.get_setting_value<std::string>("name");
    float height = clom.get_setting_value<float>("height");
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
Here is a list of all public member functions of the class `CL_Option_Manager` with their descriptions:

###### Construtor
``` C++
CL_Option_Manager();
```
The constructor of CL_Option_Manager. Create a new empty manager with no registered options.

###### Register Options
``` C++
void register_setting<T>(std::string name, T default_value);
void register_flag(std::string name);
```
Register a new option (setting or flag respectively). Options not registered this way are treated as invalid!  
A *setting* is an option with a value of type T. Valid types for T are: `int, float, double, char, std::string`  
A *flag* is an option that is either set or not set (boolean).  

###### Process/Parse Options
``` C++
void process_cl_options(int argc, char const *argv[]);
```
Scan argv for registered options. All `register_setting` and `register_flag` calls must happen before this.

###### Get Option Values
``` C++
T get_setting_value<T>(std::string name);
bool is_flag_set(std::string name);
```
Get the value of options. Must be called after `process_cl_options`.  
The type T must be the same as the one specified for the setting with the same name in `register_setting`.

###### Manage Help Message
``` C++
void set_user_hint(std::string hint);
void print_user_hint();
```
Set the help/hint message (eg. "Usage: my-app [options]" etc.) or print it.  
The default hint is "Invalid command line options!", so you might want to set a custom one.  
`print_user_hint` only prints, does not exit automatically.
