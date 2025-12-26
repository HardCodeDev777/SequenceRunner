#pragma once

#include <fstream>
#include <filesystem>
#include <optional>
#include <expected>
#include "exceptionhandler.hpp"
#include "data.hpp"

#define JSON_FILE_NAME "sequences.json"
#define SEQUENCES_JSON_NAME "sequences"

using json = nlohmann::json;

void write_sequence_to_json(const SequenceData& sequence);
std::vector<SequenceData> read_sequences_from_json();
std::expected<void, std::string> remove_sequence_from_json(std::vector<SequenceData>& readed_from_json, const std::string& name_of_sequence_to_remove);
std::expected<std::optional<bool>, std::string> exists_sequence_with_this_name_already(const std::vector<SequenceData>& vec_data, std::string name_of_sequence);