#pragma once

#include <string>
#include <iostream>
#include <sstream>

#define UI_ERROR_NAME_ALREADY_EXISTS "ERROR: Sequence with this name already exists!"

// For calling in std::unexpected
inline std::string make_error_message(const std::string& method_name, const std::string& message) 
{
	return (std::ostringstream() << "Error in " << method_name << ": " << message << "\n").str();
}

// For calling in normal code. Use with "make_error_message"
inline void throw_code_error(const std::string& error_message)
{
	std::cerr << error_message << std::endl;;
}