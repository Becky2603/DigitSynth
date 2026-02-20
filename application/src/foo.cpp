#include "foo.h"

Foo::Foo(int n) {
    this->n = n;
}

int Foo::do_something(char c) {
    return this->n + c;
}

