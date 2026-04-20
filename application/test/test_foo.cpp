
#include "foo.h"
#include <cassert>


void test_foo_1() {
	auto foo_instance = Foo(10);
	int result = foo_instance.do_something(2);

	assert(result == 12);
}

int main(int argc, char **argv) {
	(void) argc;
	(void) argv;

	test_foo_1();
	
	return 0;
}
