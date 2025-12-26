#include "datahandle.hpp"

void write_sequence_to_json(const SequenceData& sequence)
{
    json j;

    auto sequences = read_sequences_from_json();

    // It'll write even if sequences vector is empty
    sequences.push_back(sequence);
    j[SEQUENCES_JSON_NAME] = sequences;

    std::ofstream out_s(JSON_FILE_NAME);
    out_s << j.dump(4);
}

// If no saved data, will return empty vector
std::vector<SequenceData> read_sequences_from_json()
{
    json j;

    if (std::filesystem::exists(JSON_FILE_NAME))
    {
        std::ifstream if_s(JSON_FILE_NAME);
        if (if_s.is_open()) if_s >> j;
    }

    std::vector<SequenceData> sequences;

    if (j.contains(SEQUENCES_JSON_NAME)) return j[SEQUENCES_JSON_NAME].get<std::vector<SequenceData>>();
    else return sequences;
}

// It removes given sequence by name both from given and json file. Very handy
std::expected<void, std::string> remove_sequence_from_json(std::vector<SequenceData>& readed_from_json, const std::string& name_of_sequence_to_remove) 
{
    if (readed_from_json.empty()) return std::unexpected(make_error_message("remove_sequence_from_json", "Given vector is empty!"));
    else if (name_of_sequence_to_remove.empty()) return std::unexpected(make_error_message("remove_sequence_from_json", "Given string is empty!"));

    for (int i = 0; i < readed_from_json.size(); i++) 
    {
        if (readed_from_json[i].sequence_name == name_of_sequence_to_remove) 
            readed_from_json.erase(readed_from_json.begin() + i);
    }

    std::filesystem::remove(JSON_FILE_NAME);

    if (!readed_from_json.empty()) 
    {
        json j;

        j[SEQUENCES_JSON_NAME] = readed_from_json;

        std::ofstream out_s(JSON_FILE_NAME);
        out_s << j.dump(4);
    }
}

std::expected<std::optional<bool>, std::string> exists_sequence_with_this_name_already(const std::vector<SequenceData>& vec_data, std::string name_of_sequence)
{
    if (vec_data.empty()) return std::nullopt;
    if (name_of_sequence.empty()) return std::unexpected(make_error_message("exists_sequence_with_this_name_already", "Given string is empty!"));
    for (auto const& sec : vec_data) if (sec.sequence_name == name_of_sequence) return true;
    return false;
}