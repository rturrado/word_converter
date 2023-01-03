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


class converter {
public:
    virtual ~converter() = default;
    [[nodiscard]] virtual std::string parse(const std::string& text) = 0;
};
using converter_up = std::unique_ptr<converter>;


class word_to_number_converter : public converter {
    using number_stack = std::vector<int>;
private:
    number_stack stack_{};
    std::string last_connector_{};

    static auto is_word_number(const std::string& token) {
        return word_to_number_map.contains(token);
    }
    static auto all_of_character_connector(const std::string& connector) {
        return std::ranges::all_of(connector, [](unsigned char c) {
            return
                c == '-' or
                std::isspace(c);
        });
    }
    static auto is_allowed_connector(const std::string& token) {
        return
            token == "and" or
            all_of_character_connector(token);
    }
    static auto number_admits_and_connector(int n) {
        // For instance: one hundred and one, two thousand and two, three million and three
        // But not: ten and one, ninety and one, one hundred and two and three
        return n >= 100 and n % 100 == 0;
    }
    void collapse_stack(int new_number) {
        auto pos{std::ssize(stack_) - 1 };
        for (; pos > 0; --pos) {
            auto sum_of_last_two{stack_[pos - 1] + stack_[pos] };
            if (sum_of_last_two < new_number) {
                stack_[pos - 1] = sum_of_last_two;
            } else {
                break;
            }
        }
        stack_.resize(pos + 1);
    }
    std::string push_number(const std::string& number_str) {
        auto number{word_to_number_map.at(number_str) };
        // Case 1: the stack is empty
        //         the input number is just pushed to the top
        if (stack_.empty()) {
            stack_.emplace_back(number);
            return {};
        }
        // number_str                 -> ret
        // stack before push          -> stack after push
        // last connector before push -> last connector after push
        auto& top_number{stack_.back() };
        std::string ret{};
        if (top_number < number) {
            if (all_of_character_connector(last_connector_)) {
                // Case 2a: the input number is bigger than the one at the top of the stack,
                //          and only whitespaces separate this input number from the previous one;
                //          the stack is collapsed, and the new top number is multiplied by the input number
                //
                // "thousand"      -> ""
                // 3000000, 600, 3 -> 3000000, 603000
                // " "             -> ""
                collapse_stack(number);
                stack_.back() *= number;
                // TODO: not checking here for semantic errors such as: 'two ninety'
            } else {
                // Case 2b: the input number is bigger than the one at the top of the stack,
                //          and an and-connector separates this input number from the previous one;
                //          the input number is treated as a new expression
                //
                // "four"  -> "1 and "
                // 1       -> 4
                // " and " -> ""
                ret = std::to_string(top_number) + last_connector_;
                stack_.back() = number;
            }
        } else if (top_number > number) {
            if (all_of_character_connector(last_connector_)) {
                // Case 3a: the input number is smaller than the one at the top of the stack,
                //          and only whitespaces separate this input number from the previous one;
                //          the input number is pushed to the stack
                //
                // "ninety" -> ""
                // 100      -> 100, 90
                // " and "  -> ""
                stack_.emplace_back(number);
            } else {
                if (number_admits_and_connector(top_number)) {
                    // Case 3b: the input number is smaller than the one at the top of the stack,
                    //          an and-connector separates this input number from the previous one,
                    //          and the number at the top of the stack admits an and-connector as part of a word number expression;
                    //          the input number is added to the one at the top of the stack
                    //
                    // "four"  -> ""
                    // 100     -> 104
                    // " and " -> ""
                    stack_.back() += number;
                } else {
                    // Case 3b': the input number is smaller than the one at the top of the stack,
                    //           an and-connector separates this input number from the previous one,
                    //           and the number at the top of the stack does not admit an and-connector as part of a word number expression;
                    //           the input number is treated as a new expression
                    //
                    // "four"  -> "8 and "
                    // 8       -> 4
                    // " and " -> ""
                    ret = std::to_string(top_number) + last_connector_;
                    stack_.back() = number;
                }
            }
        } else {  // top_number == number
            // TODO: not checking here for semantic errors such as: 'hundred hundred'
        }
        last_connector_.clear();
        return ret;
    }
    [[nodiscard]] std::string pop_all_numbers(const std::string& token) {
        std::string ret{};
        if (not stack_.empty()) {
            auto sum_of_all_tokens{ std::accumulate(stack_.begin(), stack_.end(), 0) };
            ret = std::to_string(sum_of_all_tokens);
            stack_.clear();
        }
        ret = ret + last_connector_ + token;
        last_connector_.clear();
        return ret;
    }
public:
    [[nodiscard]] std::string parse(const std::string& token) override {
        // Number
        if (is_word_number(token)) {
            return push_number(token);
        }
        // Space or "and"
        if (is_allowed_connector(token)) {
            if (stack_.empty()) {
                return token;
            } else {
                last_connector_ += token;
                return {};
            }
        }
        // Other, word number splitter
        return pop_all_numbers(token);
    }
};


class tokenizer {
    std::string text_{};
public:
    explicit tokenizer(std::string text)
        : text_{ std::move(text) }
    {}

    // TODO: not considering digit numbers as possible parts of a word number (e.g. 3 million)
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
            std::string text{ reader->read() };

            // Texts that do not form a sentence (i.e. that do not end in a period) are not converted
            if (text.ends_with('.')) {
                tokenizer tokenizer{ text };
                text.clear();
                for (const auto& token : tokenizer.get_next_token()) {
                    text += converter_->parse(token);
                }
            }

            // All the texts are written out though
            std::ranges::for_each(writers, [&text](auto& writer) { writer->write(text); });
        }
    }
};
