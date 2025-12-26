#pragma once

#include <slint.h>
#include <memory>
#include <vector>
#include <string>
#include <type_traits>
#include <expected>
#include <optional>
#include "exceptionhandler.hpp"
#include "datahandle.hpp"

// Displays sequences in Slint.
#define DISPLAY_SEQUENCES_CHECK_ERRORS(readed_json_sequences, err_method_name) \
if (auto res_seq = vec_sequences_to_vec_names(readed_json_sequences)) \
{ \
    DISPLAY_VEC_CHECK_ERRORS(*res_seq, false, set_sequences, err_method_name) \
} \
else throw_code_error(make_error_message(err_method_name, res_seq.error()));

// Basic std::vector<> displaying in Slint
#define DISPLAY_VEC_CHECK_ERRORS(vector, allow_empty_vec_for_converting, set_method, err_method_name) \
if (auto res = vector_to_slint_string_array(vector, allow_empty_vec_for_converting)) ui->set_method(*res); \
else throw_code_error(make_error_message(err_method_name, res.error()));

#define CLEAR_UI_ERRORS \
slint::SharedString err(""); \
ui->set_error_message(err); \

template<typename T>
std::expected<std::shared_ptr<slint::VectorModel<slint::SharedString>>, std::string>
vector_to_slint_string_array(const std::vector<T>& values, bool allow_empty = false)
{
    if (!allow_empty) if (values.empty()) return std::unexpected(make_error_message("vector_to_slint_string_array", "Given vector is empty!"));
    auto model = std::make_shared<slint::VectorModel<slint::SharedString>>();
    for (const auto& v : values)
    {
        if constexpr (std::is_same_v<T, std::string>) model->push_back(slint::SharedString(v));
        else if constexpr (std::is_convertible_v<T, std::string>)model->push_back(slint::SharedString(static_cast<std::string>(v))); 
        else if constexpr (std::is_arithmetic_v<T>) model->push_back(slint::SharedString(std::to_string(v)));
        else static_assert(std::is_arithmetic_v<T>, "Type T is not convertible to string");  
    }
    return model;
}

// Optional cuz it's ok if no saved data
inline std::optional<std::vector<std::string>> set_base_elements_in_seq()
{
    auto readed_json_sequences = read_sequences_from_json();
    if (readed_json_sequences.empty()) return std::nullopt;

    std::vector<std::string> sequences_names;
    for (auto const& seq : readed_json_sequences) sequences_names.push_back(seq.sequence_name);

    return sequences_names;
}

// If we need to display only names from sequence in Slint
inline std::expected<std::vector<std::string>, std::string> vec_sequences_to_vec_names(const std::vector<SequenceData> &readed_json_sequences)
{
    if (readed_json_sequences.empty()) return std::unexpected(make_error_message("vec_sequences_to_vec_names", "Given vector is empty!"));
    std::vector<std::string> sequences_names; 
    for (auto const& seq : readed_json_sequences) sequences_names.push_back(seq.sequence_name); 
    return sequences_names;
}