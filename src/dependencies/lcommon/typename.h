/*
 * typename.h:
 *  Utility for getting the (clean) name of a type
 */

#ifndef TYPENAME_H_
#define TYPENAME_H_

#include <string>
#include <typeinfo>

std::string cpp_demangle_type_name(const char* name);

template <typename T>
inline std::string cpp_type_name(const T& object) {
	return cpp_demangle_type_name(typeid(object).name());
}

template <typename T>
inline std::string cpp_type_name_no_namespace(const T& object) {
    std::string str = cpp_type_name(object);
    int n = 0;
    for (int i = str.size() - 1; i >= 0; i--) {
        if (str[i] == ':') {
            return str.substr(i + 1, n);
        }
        n++;
    }
    return str;
}

#endif
