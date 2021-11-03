module;
#include <iostream>         // import declaration
// import std.core;
export module helloWorld;  // module declaration

export void hello() {      // export declaration
    std::cout << "Hello 中国！\n";
}