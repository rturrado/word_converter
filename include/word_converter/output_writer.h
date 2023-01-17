#pragma once

#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <stdexcept>  // runtime_error

namespace fs = std::filesystem;


struct could_not_create_file_error : public std::runtime_error {
    explicit could_not_create_file_error(const fs::path& file_path) : std::runtime_error{ "" } {
        message_ += fmt::format("'{}'", file_path.generic_string());
    }
    [[nodiscard]] const char* what() const noexcept override { return message_.c_str(); };
private:
    std::string message_{ "could not create file: " };
};


class output_writer {
    [[nodiscard]] virtual std::ostream& get_ostream() = 0;
public:
    virtual ~output_writer() = default;

    void write(const std::string& text) {
        auto& os{ get_ostream() };
        os << text;
    }
};


class file_writer : public output_writer {
public:
    explicit file_writer(const fs::path& file_path) : ofs_{ file_path } {
        if (not ofs_) {
            throw could_not_create_file_error{ file_path };
        }
    }
private:
    std::ofstream ofs_;

    [[nodiscard]] std::ostream& get_ostream() override {
        return ofs_;
    }
};


class stream_writer : public output_writer {
public:
    explicit stream_writer(std::ostream& os) : os_{ os } {}
private:
    std::ostream& os_;

    [[nodiscard]] std::ostream& get_ostream() override {
        return os_;
    }
};


using output_writer_up = std::unique_ptr<output_writer>;
