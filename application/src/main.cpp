#include <iostream>
#include "foo.h"


int main(int argc, char **argv) {
    std::cout << "\nStarting DigitSynth application\nArguments: \n";

    for (int i = 0; i < argc; i++) {
	std::cout << "\t" << argv[i] << "\n"; 
    }

    auto foo = Foo(10);
    std::cout << foo.do_something('a') << "\n";

    return 0;
}
