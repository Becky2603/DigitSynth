
#include "foo.h"


int test_foo_1() {
	auto foo_instance = Foo(10);
	int result = foo_instance.do_something(2);

	return result == 12 ? 0 : -1;
}

int main(int argc, char **argv) {
	(void) argc;
	(void) argv;

	return test_foo_1();
}

