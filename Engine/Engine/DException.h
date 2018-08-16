#pragma once
#include <exception>

struct DException : public std::exception
{
public:
	DException(const char* message);
};
