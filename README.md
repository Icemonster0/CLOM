# CLOM
Yet another Command Line Option Manager for C++. Use this single header library if you think Boost would be overkill.

---

### Usage
Simply include [clom.hpp](clom.hpp) in your source code.  
Then, you just need to do the following steps:  
1. Register all settings and flags
2. Let clom parse the command line options
3. Use the options however you like

### Example
Here is an example of how to use clom:
``` C++
#include "clom.hpp"
#include <iostream>

int main(int argc, char const *argv[]) {

    // create the clom object
    CL_Option_Manager clom;

    // 1. register settings (with default values) and flags
    clom.register_setting<std::string>("name", "Mr X", "The name of our subject");
    clom.register_setting<float>("height", 6.0f, "The height of our subject in feet");
    clom.register_flag("--smart", "Specify whether our subject is smart");

    // Optionally generate help massage
    clom.generate_user_hint("my-app");

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
error: Unknown option: 'age'
Usage: my-app [<setting> <value>] [<flag>] ...

Settings:
 Name:               Type:           Description:
  height              float           The height of our subject in feet
  name                std::string     The name of our subject

Flags:
 Name:               Description:
  --smart             Specify whether our subject is smart
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
void register_setting<T>(std::string name, T default_value, std::string description = "");
void register_flag(std::string name, std::string description = "");
```
Register a new option (setting or flag respectively). Options not registered this way are treated as invalid. All option names must be unique!  
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
std::string get_user_hint();
void generate_user_hint(std::string app_name);
void print_user_hint();
```
Set or get the help massage. By default it is an empty string.  
`generate_user_hint` must be called after option registrations and before `process_cl_options`!
`print_user_hint` only prints, does not exit automatically.
