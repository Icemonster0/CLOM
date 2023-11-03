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

class CL_Option_Manager {
private:
    enum CLOM_Type {
        INT, FLOAT, DOUBLE, CHAR, STRING
    };

    struct CLOM_General_Setting {
        CLOM_General_Setting(std::string p_name, CLOM_Type p_type) : name(p_name), type(p_type) {
        }

        // this is not hacky idk what you're talking about
        virtual void this_is_polymorphic() = 0;

        std::string name;
        CLOM_Type type;
    };

    template<typename T>
    struct CLOM_Setting : public CLOM_General_Setting {
        CLOM_Setting(std::string p_name, T p_value)
            : CLOM_General_Setting {p_name, CL_Option_Manager::cpp_type_to_clom_type<T>()}, value(p_value) {
        }

        // the other bit that is also not hacky
        void this_is_polymorphic() {
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

public:

    CL_Option_Manager() : user_hint("Invalid command line options!\n") {
    }

    template<typename T>
    void register_setting(std::string name, T default_value) {
        settings.push_back(std::unique_ptr<CLOM_General_Setting>(new CLOM_Setting<T>(name, default_value)));
    }

    void register_flag(std::string name) {
        flags.emplace_back(name);
    }

    void process_cl_options(int argc, char const *argv[]) {
        for (size_t i = 1; i < argc;) {
            CLOM_General_Setting *setting = get_general_setting_by_name(argv[i]);
            CLOM_Flag *flag = get_flag_by_name(argv[i]);

            if (setting) {
                if (!(i < argc-1)) {
                    std::cout << "Missing value for '" << argv[i] << "'\n";
                    print_user_hint();
                    exit(1);
                }

                try {
                    switch (setting->type) {
                        case INT: dynamic_cast<CLOM_Setting<int>*>(setting)->value = std::stoi(argv[i+1]); break;
                        case FLOAT: dynamic_cast<CLOM_Setting<float>*>(setting)->value = std::stof(argv[i+1]); break;
                        case DOUBLE: dynamic_cast<CLOM_Setting<double>*>(setting)->value = std::stod(argv[i+1]); break;
                        case CHAR: dynamic_cast<CLOM_Setting<char>*>(setting)->value = argv[i+1][0]; break;
                        case STRING: dynamic_cast<CLOM_Setting<std::string>*>(setting)->value = argv[i+1]; break;
                        default: std::cout << "CLOM internal error: setting '" << argv[i] << "', value '" << argv[i+1]
                                           << "', type " << setting->type << "\n"; exit(1);
                    }
                } catch (...) {
                    std::cout << "Failed to parse value '" << argv[i+1] << "' of '" << argv[i]
                              << "'! Expected type: " << clom_type_names[setting->type] << "\n";
                    print_user_hint();
                    exit(1);
                }

                i += 2;
            }

            else if (flag) {
                flag->is_set = true;
                i += 1;
            }

            else {
                std::cout << "Unknown option: '" << argv[i] << "'\n";
                print_user_hint();
                exit(1);
            }
        }
    }

    template<typename T>
    T get_setting_value(std::string name) {
        CLOM_General_Setting *setting = get_general_setting_by_name(name);

        if (setting) {
            CLOM_Type t = cpp_type_to_clom_type<T>();

            if (setting->type != t) {
                std::cout << "Type of '" << name << "' is " << clom_type_names[setting->type]
                          << ", but " << clom_type_names[t]
                          << " requested\n(from: CL_Option_Manager::get_setting_value<" << clom_type_names[t] << ">(\"" << name << "\"))\n";
                exit(1);
            }

            return dynamic_cast<CLOM_Setting<T>*>(setting)->value;
        }
        else {
            std::cout << "The setting " << name << " is not registered!" << '\n';
            exit(1);
        }
    }

    bool is_flag_set(std::string name) {
        CLOM_Flag *flag = get_flag_by_name(name);

        if (flag)
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

    CLOM_General_Setting* get_general_setting_by_name(std::string name) {
        for (std::unique_ptr<CLOM_General_Setting> &setting : settings) {
            if (setting->name.compare(name) == 0) {
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

    template<typename T>
    static CLOM_Type cpp_type_to_clom_type() {
        if      (typeid(T) == typeid(int)) return INT;
        else if (typeid(T) == typeid(float)) return FLOAT;
        else if (typeid(T) == typeid(double)) return DOUBLE;
        else if (typeid(T) == typeid(char)) return CHAR;
        else if (typeid(T) == typeid(std::string)) return STRING;
        else {std::cout << "Unsopported option type: " << typeid(T).name() << '\n'; exit(1);}
    }

    std::vector<std::unique_ptr<CLOM_General_Setting>> settings;
    std::vector<CLOM_Flag> flags;

    std::string user_hint;

    const std::string clom_type_names[5] {
        "INT", "FLOAT", "DOUBLE", "CHAR", "STRING"
    };
};

#endif /* end of include guard: CLOM_HPP */
