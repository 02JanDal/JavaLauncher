#include "JavaLauncher.h"

#include <string>
#include <iostream>

int main(int argc, char **argv)
{
	// find the tests directory
	std::string dir = __FILE__;
	const int index = dir.rfind('/');
	if (index != std::string::npos)
	{
		dir = dir.erase(index);
	}

	std::list<std::string> opts;
	opts.push_back("-Djava.class.path=" + dir);
	std::list<std::string> args;
	args.push_back("abc");
	args.push_back("def");

	JavaLauncher launcher1;
	launcher1.setOptions(opts);
	launcher1.setArguments(args);
	launcher1.setMainClass("Test");
	launcher1.setJVMPath(JVM_LIBRARY_PATH);
	launcher1.launch();

	return 0;
}
