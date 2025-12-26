#pragma once

#include <json.hpp>
#include <vector>
#include <string>

#define SEQUENCE_JSON_FIELD_NAME "name"
#define SEQUENCE_JSON_FIELD_FILES_NAME "files"

using json = nlohmann::json;

class SequenceData {
public:
	std::string sequence_name;
	std::vector<std::string> sequence_files;

	SequenceData() = default;
	SequenceData(const std::string& sequence_name, const std::vector<std::string>& sequence_files)
	{
		this->sequence_files = sequence_files;
		this->sequence_name = sequence_name;
	}
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SequenceData, sequence_name, sequence_files)