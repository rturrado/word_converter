# pragma once

#include <cstring>
#include <optional>
#include <stdexcept>  // runtime_error
#include <string>  // to_string


struct invalid_number_of_arguments_error : public std::runtime_error {
    explicit invalid_number_of_arguments_error(int n) : std::runtime_error{ "" } {
        message_ += std::to_string(n);
    }
    [[nodiscard]] const char* what() const noexcept override { return message_.c_str(); };
private:
    std::string message_{ "invalid number of arguments: " };
};


struct invalid_argument_error : public std::runtime_error {
    explicit invalid_argument_error(std::string arg) : std::runtime_error{ "" } {
        message_ = message_ + "'" + std::move(arg) + "'";
    }
    [[nodiscard]] const char* what() const noexcept override { return message_.c_str(); };
private:
    std::string message_{ "invalid argument: " };
};


struct command_line_options {
    std::string input_file{};
    std::optional<std::string> output_file{};
};


struct command_line_parser {
    [[nodiscard]] static auto parse(int argc, const char** argv) {
        command_line_options clo{};
        if (argc != 3 and argc != 5) {
            throw invalid_number_of_arguments_error{ argc };
        }
        std::string argv_1{ argv[1] };
        std::string dash_i_arg{ "-i" };
        std::string dash_o_arg{ "-o" };
        if (argc == 3) {
            if (argv_1 != dash_i_arg) {
                throw invalid_argument_error{ argv_1 };
            }
            clo.input_file = argv[2];
        } else {  // argc == 5
            std::string argv_3{ argv[3] };
            if (argv_1 == dash_i_arg) {  // -i in -o out
                if (argv_3 != dash_o_arg) {
                    throw invalid_argument_error{ argv_3 };
                }
                clo.input_file = argv[2];
                clo.output_file = argv[4];
            } else {  // -o out -i in
                if (argv_1 != dash_o_arg) {
                    throw invalid_argument_error{ argv_1 };
                }
                if (argv_3 != dash_i_arg) {
                    throw invalid_argument_error{ argv_3 };
                }
                clo.input_file = argv[4];
                clo.output_file = argv[2];
            }
        }
        return clo;
    }
};
