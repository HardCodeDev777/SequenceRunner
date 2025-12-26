#pragma region includes
#include "app_window.h"
#include <iostream>
#include <slint.h>
#include <string>
#include <filesystem>
#include "filedialog.hpp"
#include "uiutils.hpp"
#include "data.hpp"
#include "datahandle.hpp"
#include "cfg.hpp"
#include "exceptionhandler.hpp"
#include "sequence_runner.hpp"
#pragma endregion

int main()
{
#pragma region pre
    std::vector<std::string> current_sequence_pathes_from_ui;
    // Only for UI, not for core logic
    std::vector<std::string> current_sequence_names_from_ui;

    auto ui = MainWindow::create();

    if (auto existed_seqs = set_base_elements_in_seq()) 
    { 
        if (is_debug) LOG("Found saved data!")
        DISPLAY_VEC_CHECK_ERRORS(*existed_seqs, false, set_sequences, "main") 
    }
#pragma endregion

#pragma region create sequence tab callbacks
    ui->on_add_element([&]() {
        CLEAR_UI_ERRORS
        if (auto res = safe_open_exe_dialog()) {
            std::filesystem::path p(*res);

            if (is_debug) LOG("Added element: " << p.filename().string())

            current_sequence_pathes_from_ui.push_back(p.string());
            current_sequence_names_from_ui.push_back(p.filename().string());

            DISPLAY_VEC_CHECK_ERRORS(current_sequence_names_from_ui, false, set_added_sequence_elements, "ui->on_add_element")
        }
    });

    ui->on_remove_element([&]() {
        CLEAR_UI_ERRORS
        if (is_debug) LOG("Removed last element")

        current_sequence_pathes_from_ui.pop_back();
        current_sequence_names_from_ui.pop_back();
        DISPLAY_VEC_CHECK_ERRORS(current_sequence_names_from_ui, true, set_added_sequence_elements, "ui->on_remove_element")
    });

    ui->on_create_sequence([&](slint::SharedString name) {
        CLEAR_UI_ERRORS
        auto readed_json_sequences = read_sequences_from_json();
        if (!readed_json_sequences.empty()) 
        {
            if (auto executed = exists_sequence_with_this_name_already(read_sequences_from_json(), std::string(name)))
            {
                if (**executed)
                {
                    LOG("Displaying UI error about sequence name already exists")
                        slint::SharedString err(UI_ERROR_NAME_ALREADY_EXISTS);
                    ui->set_error_message(err);
                    return;
                }
            }
            else
            {
                throw_code_error(make_error_message("ui->on_create_sequence", executed.error()));
                return;
            }
        }

        // First writing
        write_sequence_to_json(SequenceData(std::string(name), current_sequence_pathes_from_ui));

        // Then reading all, not only those that existed before
        readed_json_sequences = read_sequences_from_json();

        DISPLAY_SEQUENCES_CHECK_ERRORS(readed_json_sequences, "ui->on_create_sequence")

        current_sequence_pathes_from_ui.clear();
        current_sequence_names_from_ui.clear();

        // Clearing ListView
        std::vector<std::string> empty_vec;
        DISPLAY_VEC_CHECK_ERRORS(empty_vec, true, set_added_sequence_elements, "ui->on_create_sequence")
    });
#pragma endregion

#pragma region run sequence tab callbacks
    ui->on_start_sequence([&]() {
        CLEAR_UI_ERRORS

        // I could make callback with string, but it's easier
        auto sequence_name = std::string(ui->get_selected_sequence_to_run());

        auto readed_json_sequences = read_sequences_from_json();
        for (const auto& seq : readed_json_sequences)
        {
            if (seq.sequence_name == sequence_name)
            {
                if (auto res = run_sequence(seq)) 
                {
                    if (is_debug) LOG("Running sequence " << seq.sequence_name)
                }
                else throw_code_error(make_error_message("ui->on_start_sequence", res.error()));
                break;
            }
        }
    });

    ui->on_show_sequence_files([&]() {
        // Same
        auto sequence_name = std::string(ui->get_selected_sequence_to_run());

        auto readed_json_sequences = read_sequences_from_json();
        std::vector<std::string> files_names;
        for (const auto& seq : readed_json_sequences) 
        {
            if (seq.sequence_name == sequence_name) 
            {
                for (const auto& file : seq.sequence_files) files_names.push_back(std::filesystem::path(file).filename().string());    
                break;
            }
        }

        DISPLAY_VEC_CHECK_ERRORS(files_names, false, set_existed_sequence_elements, "ui->on_show_sequence_files")
    });

    ui->on_hide_sequence_files([&]() {
        std::vector<std::string> empty_vec;
        DISPLAY_VEC_CHECK_ERRORS(empty_vec, true, set_existed_sequence_elements, "ui->on_hide_sequence_files")
    });

    ui->on_remove_sequence([&]() {
        CLEAR_UI_ERRORS
        
        auto seq_name = std::string(ui->get_selected_sequence_to_run());
        auto readed_json_sequences = read_sequences_from_json();
        
        if (auto executed = remove_sequence_from_json(readed_json_sequences, seq_name)) 
        {
            if (!readed_json_sequences.empty()) 
            { 
                if (is_debug) LOG("Was deleted not last sequence")
                DISPLAY_SEQUENCES_CHECK_ERRORS(readed_json_sequences, "ui->on_remove_sequence") 
            }
            else 
            {
                if (is_debug) LOG("Was deleted last sequence")
                std::vector<std::string> empty_vec;
                DISPLAY_VEC_CHECK_ERRORS(empty_vec, true, set_sequences, "ui->on_remove_sequence")
            }
        }
        else throw_code_error(make_error_message("ui->on_remove_sequence", executed.error()));
    });
#pragma endregion

    ui->run();
    return 0;
}