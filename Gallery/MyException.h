#pragma once
#include <exception>
#include <string>

class MyException : public std::exception
{
public:
	MyException(const std::string& message) : m_message(message) {}
	virtual ~MyException() noexcept = default;
	virtual const char* what() const noexcept { return m_message.c_str(); }

protected:
	std::string m_message;
};
