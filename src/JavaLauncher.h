#pragma once

#include <string>
#include <list>
#include <exception>

class JavaLauncherException : public std::exception
{
public:
	explicit JavaLauncherException(const std::string &error)
		: m_error(error) {}
	~JavaLauncherException() noexcept {}

	const char *what() const noexcept override { return m_error.c_str(); }

private:
	std::string m_error;
};

class JavaLauncher
{
public:
	void setJVMPath(const std::string &path) { m_path = path; }
	void setMainClass(const std::string &clazz) { m_mainClass = clazz; }
	void setOptions(const std::list<std::string> &options) { m_options = options; }
	void setArguments(const std::list<std::string> &args) { m_arguments = args; }

	void launch();

private:
	std::string m_path;
	std::string m_mainClass;
	std::list<std::string> m_options;
	std::list<std::string> m_arguments;
};
