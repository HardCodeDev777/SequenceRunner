#pragma once

#include <windows.h>
#include <vector>
#include <expected>
#include <string>
#include "exceptionhandler.hpp"
#include "data.hpp"

inline std::expected<void, std::string> run_sequence(const SequenceData& sequence)
{
	if (sequence.sequence_files.empty()) return std::unexpected(make_error_message("run_sequence", "Given vector is empty!"));
	for (const auto& file : sequence.sequence_files) 
	{
		ShellExecute(
			NULL,
			"open",
			file.c_str(),
			NULL,
			NULL,
			SW_SHOW
		);
	}
}