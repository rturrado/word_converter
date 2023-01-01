#pragma once

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;


struct could_not_create_file_error : public std::runtime_error {
    explicit could_not_create_file_error(const fs::path& file_path) : std::runtime_error{ "" } {
        message_ = message_ + "'" + file_path.generic_string() + "'";
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
    void write_sentence(const std::string& sentence) {
        auto& os{ get_ostream() };
        os << sentence << ".";
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
