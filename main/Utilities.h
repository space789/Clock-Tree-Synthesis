///////////////////////////
// Utilities.h           //
// Author: Shiina        //
// Date: 2024/09/30      //
// Version: 1.0          //
// copiright 2024        //
///////////////////////////

#ifndef UTILITIES_H
#define UTILITIES_H

#include <memory>
#include <utility>

// Custom make_unique implementation for C++11
namespace std {
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

#endif
