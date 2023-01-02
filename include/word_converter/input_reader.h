#pragma once

#include <filesystem>
#include <fstream>
#include <stdexcept>  // runtime_error
#include <system_error>  // error_code

namespace fs = std::filesystem;


struct file_is_not_a_regular_file_error : public std::runtime_error {
    explicit file_is_not_a_regular_file_error(const fs::path& file_path) : std::runtime_error{ "" } {
        message_ = message_ + "'" + file_path.generic_string() + "'";
    }
    [[nodiscard]] const char* what() const noexcept override { return message_.c_str(); };
private:
    std::string message_{ "file is not a regular file: " };
};


class input_reader {
public:
    virtual ~input_reader() = default;

    // Read a sentence, i.e. until a period is found
    // or until the end of file, if no period is found
    std::string read() {
        auto& is{ get_istream() };
        std::string sentence{};
        std::getline(is, sentence, '.');
        // It could happen that the last text in the stream does not end with a period
        // In that case, no period is added to the read text
        // Otherwise, std::getline stopped when finding a period
        // In that case, the period is added back to the read text
        if (not is.eof()) {
            sentence += ".";
        }
        return sentence;
    }
    auto eof() { return get_istream().eof(); }
    auto fail() { return get_istream().fail(); }
private:
    [[nodiscard]] virtual std::istream& get_istream() = 0;
};


class file_reader : public input_reader {
public:
    explicit file_reader(const fs::path& file_path) : ifs_{ file_path } {
        std::error_code ec{};
        if (not fs::is_regular_file(file_path, ec)) {
            throw file_is_not_a_regular_file_error{ file_path };
        }
    }
private:
    std::ifstream ifs_;

    [[nodiscard]] std::istream& get_istream() override {
        return ifs_;
    }
};


class stream_reader : public input_reader {
public:
    explicit stream_reader(std::istream& is) : is_{ is } {}
private:
    std::istream& is_;

    [[nodiscard]] std::istream& get_istream() override {
        return is_;
    }
};
