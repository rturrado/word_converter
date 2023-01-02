#pragma once

#include "generator.hpp"
#include "input_reader.h"
#include "output_writer.h"

#include <algorithm>  // all_of, for_each
#include <cctype>  // isspace
#include <memory>  // unique_ptr
#include <numeric>  // accumulate
#include <regex>
#include <stdexcept>  // runtime_error
#include <string>
#include <unordered_map>
#include <vector>

using input_reader_up = std::unique_ptr<input_reader>;
using output_writer_up_list = std::vector<std::unique_ptr<output_writer>>;


inline static const std::unordered_map<std::string, int> word_to_number_map{
    { "zero", 0 }, { "one", 1 }, { "two", 2 }, { "three", 3 }, { "four", 4 },
    { "five", 5 }, { "six", 6 }, { "seven", 7 }, { "eight", 8 }, { "nine", 9 },
    { "ten", 10 }, { "eleven", 11 }, { "twelve", 12 }, { "thirteen", 13 }, { "fourteen", 14 },
    { "fifteen", 15 }, { "sixteen", 16 }, { "seventeen", 17 }, { "eighteen", 18 }, { "nineteen", 19 },
    { "twenty", 20 }, { "thirty", 30 }, { "forty", 40 }, { "fifty", 50 },
    { "sixty", 60 }, { "seventy", 70 }, { "eighty", 80 }, { "ninety", 90 },
    { "hundred", 100 }, { "thousand", 1'000 },  { "million", 1'000'000 }, { "billion", 1'000'000'000 }
};


struct unexpected_token_error : public std::runtime_error {
    explicit unexpected_token_error(const std::string& token) : std::runtime_error{ "" } {
        message_ = message_ + "'" + token + "'";
    }
    [[nodiscard]] const char* what() const noexcept override { return message_.c_str(); };
private:
    std::string message_{ "unexpected token: " };
};


class converter {
public:
    virtual ~converter() = default;
    [[nodiscard]] virtual std::string parse(const std::string& text) = 0;
};
using converter_up = std::unique_ptr<converter>;


class word_to_number_converter : public converter {
    using token_queue = std::vector<int>;
private:
    token_queue queue_{};
    std::string last_connector_{};

    static auto is_word_number(const std::string& token) {
        return word_to_number_map.contains(token);
    }
    static auto is_space_word_number_connector(const std::string& token) {
        return std::ranges::all_of(token, [](unsigned char c) { return std::isspace(c); });
    }
    static auto is_allowed_word_number_connector(const std::string& token) {
        return
            token == "and" or
            token == "-" or
            is_space_word_number_connector(token);
    }
    std::string push_token(const std::string& token_str) {
        auto token{ word_to_number_map.at(token_str) };
        if (queue_.empty()) {
            queue_.emplace_back(token);
            return {};
        }
        // input             -> output
        // queue before push -> queue after push
        auto& top_token{ queue_.back() };
        if (top_token < token) {
            if (is_space_word_number_connector(last_connector_)) {
                // "hundred" -> ""
                // (1, " ")   -> (100, "")
                top_token *= token;
                last_connector_.clear();
                return {};
                // Not checking here for semantic errors such as: 'two ninety'
            }
            // "hundred"    -> "1 and "
            // (1, " and ") -> (100, "")
            auto ret{ std::to_string(top_token) };
            queue_.back() = token;
            last_connector_.clear();
            return ret;
        }
        // top_token.number >= token
        //
        // "ninety"       -> ""
        //                   ( 90, "")
        // (100, " and ") -> (100, "")
        //
        // "ninety"  -> ""
        //              ( 90, "")
        // (100, "") -> (100, "")
        queue_.emplace_back(token);
        last_connector_.clear();
        return {};
        // Not checking here for semantic errors such as: 'hundred hundred'
    }
    [[nodiscard]] std::string pop_all_tokens() {
        if (queue_.empty()) {
            return last_connector_;
        }
        auto sum_of_all_tokens{ std::accumulate(queue_.begin(), queue_.end(), 0) };
        auto ret{ std::to_string(sum_of_all_tokens) + last_connector_ };
        queue_.clear();
        last_connector_.clear();
        return ret;
    }
public:
    [[nodiscard]] std::string parse(const std::string& token) override {
        // Number
        if (is_word_number(token)) {
            return push_token(token);
        }
        // Space or "and"
        if (is_allowed_word_number_connector(token)) {
            if (queue_.empty()) {
                return token;
            }
            last_connector_ += token;
            return {};
        }
        // Other, word number splitter
        return pop_all_tokens() + token;
    }
};


class tokenizer {
    std::string text_{};
public:
    explicit tokenizer(std::string text)
        : text_{ std::move(text) }
    {}

    std::generator<std::string> get_next_token() {
        std::string ret{};
        std::regex word_pattern{ R"([a-zA-Z]+)" };
        std::smatch sm{};
        while (std::regex_search(text_, sm, word_pattern)) {
            if (not sm.prefix().str().empty()) {
                co_yield sm.prefix();
            }
            co_yield sm.str();
            text_ = sm.suffix();
        }
        if (not text_.empty()) {
            co_yield text_;
        }
    }
};


class conversion_manager {
    std::unique_ptr<converter> converter_{};
public:
    explicit conversion_manager(std::unique_ptr<converter> c)
        : converter_{ std::move(c) }
    {}

    void run(input_reader_up reader, output_writer_up_list& writers) {
        while (not reader->eof()) {
            std::string input_text{ reader->read() };
            std::string output_text{};

            // Texts that do not form a sentence (i.e. that do not end in a period) are not converted
            if (input_text.ends_with('.')) {
                tokenizer tokenizer{ input_text };
                for (const auto& token : tokenizer.get_next_token()) {
                    output_text += converter_->parse(token);
                }
            } else {
                output_text = std::move(input_text);
            }

            // All the texts are written out though
            std::ranges::for_each(writers, [&output_text](auto& writer) { writer->write(output_text); });
        }
    }
};
