/*
* CLOM is licensed under the MIT License:
*
* Copyright (c) 2023 András Borsányi
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef CLOM_HPP
#define CLOM_HPP

#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <memory>

struct CLOM_General_Setting {
    CLOM_General_Setting(std::string p_name) : name(p_name) {
    }

    std::string name;
};

template<typename T>
struct CLOM_Setting : public CLOM_General_Setting {
    CLOM_Setting(std::string p_name, T p_value)
        : CLOM_General_Setting {p_name}, value(p_value) {
    }

    T value;
};

struct CLOM_Flag {
    CLOM_Flag(std::string p_name)
        : name(p_name), is_set(false) {
    }

    std::string name;
    bool is_set;
};

class CL_Option_Manager {
public:

    CL_Option_Manager() : user_hint("Invalid command line options!\n") {
    }

    template<typename T>
    void register_setting(std::string name, T default_value) {
        settings.push_back(new CLOM_Setting<T>(name, default_value));
    }

    void register_flag(std::string name) {
        flags.emplace_back(name);
    }

    void process_cl_options(int argc, char const *argv[]) {
        for (size_t i = 1; i < argc;) {
            CLOM_Setting<int> *int_setting = get_setting_by_name<int>(argv[i]);
            CLOM_Setting<float> *float_setting = get_setting_by_name<float>(argv[i]);
            CLOM_Setting<std::string> *string_setting = get_setting_by_name<std::string>(argv[i]);
            CLOM_Flag *flag = get_flag_by_name(argv[i]);

            if (int_setting != nullptr && i < argc-1) {
                int_setting->value = std::stoi(argv[i+1]);
                i += 2;
            }
            else if (float_setting != nullptr && i < argc-1) {
                float_setting->value = std::stof(argv[i+1]);
                i += 2;
            }
            else if (string_setting != nullptr && i < argc-1) {
                string_setting->value = argv[i+1];
                i += 2;
            }
            else if (flag != nullptr) {
                flag->is_set = true;
                i += 1;
            }
            else {
                std::cout << "Unknown option: " << argv[i] << '\n';
                print_user_hint();
                exit(1);
            }
        }
    }

    template<typename T>
    T get_setting_value(std::string name) {
        CLOM_Setting<T> *setting = get_setting_by_name<T>(name);

        if (setting != nullptr)
            return setting->value;
        else {
            std::cout << "The setting " << name << " is not registered!" << '\n';
            exit(1);
        }
    }

    bool is_flag_set(std::string name) {
        CLOM_Flag *flag = get_flag_by_name(name);

        if (flag != nullptr)
            return flag->is_set;
        else {
            std::cout << "The flag " << name << " is not registered!" << '\n';
            exit(1);
        }
    }

    void set_user_hint(std::string hint) {
        user_hint = hint;
    }

    void print_user_hint() {
        std::cout << user_hint << '\n';
    }

private:

    template<typename T>
    CLOM_Setting<T>* get_setting_by_name(std::string name) {
        for (std::unique_ptr<CLOM_General_Setting> setting : settings) {
            if (typeid(T) == typeid(setting->get_value()) &&
                setting->name.compare(name) == 0) {
                return setting.get();
            }
        }

        return nullptr;
    }

    CLOM_Flag* get_flag_by_name(std::string name) {
        for (CLOM_Flag &flag : flags) {
            if (flag.name.compare(name) == 0) {
                return &flag;
            }
        }

        return nullptr;
    }

    std::vector<std::unique_ptr<CLOM_General_Setting>> settings;
    std::vector<CLOM_Flag> flags;

    std::string user_hint;
};

#endif /* end of include guard: CLOM_HPP */
