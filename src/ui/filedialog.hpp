#pragma once
#include "tinyfiledialogs.h"
#include <optional>
#include <string>

char* unsafe_open_exe_dialog();
std::optional<std::string> safe_open_exe_dialog();