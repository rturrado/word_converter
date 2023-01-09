#pragma once

#include "lexer.h"

#include <fmt/core.h>
#include <stdexcept>  // runtime_error
#include <string>
#include <unordered_map>


inline static const std::unordered_map<std::string, int> word_to_number_map{
    { "zero", 0 },  // zero
    { "one", 1 },  // one
    { "two", 2 },  // two to nine
    { "three", 3 },
    { "four", 4 },
    { "five", 5 },
    { "six", 6 },
    { "seven", 7 },
    { "eight", 8 },
    { "nine", 9 },
    { "ten", 10 },  // ten to nineteen
    { "eleven", 11 },
    { "twelve", 12 },
    { "thirteen", 13 },
    { "fourteen", 14 },
    { "fifteen", 15 },
    { "sixteen", 16 },
    { "seventeen", 17 },
    { "eighteen", 18 },
    { "nineteen", 19 },
    { "twenty", 20 },  // tens
    { "thirty", 30 },
    { "forty", 40 },
    { "fifty", 50 },
    { "sixty", 60 },
    { "seventy", 70 },
    { "eighty", 80 },
    { "ninety", 90 },
    { "hundred", 100 },  // hundred
    { "thousand", 1'000 },  // thousand
    { "million", 1'000'000 },  // million
    { "billion", 1'000'000'000 }  // billion
};


struct invalid_token_error : public std::runtime_error {
    explicit invalid_token_error(const token_t& t) : std::runtime_error{ "" } {
        message_ += fmt::format("'{}'", t);
    }
    [[nodiscard]] const char* what() const noexcept override { return message_.c_str(); };
private:
    std::string message_{ "invalid token: " };
};


struct parser {
    [[nodiscard]] static std::string parse(std::string text) {
        //fmt::print("'{}' -> ", text);
        std::string ret{};
        lexer lexer{ std::move(text) };
        //fmt::print("[");
        //bool first{ true };
        for (auto&& token : lexer.get_next_token() ) {
            //fmt::print("{}{}", first ? "" : ", ", token);
            //first = false;
            if (token.lexeme == lexeme_t::end) {
                break;
            }
            ret += token.text;
        }
        //fmt::print("]\n");
        return ret;
    }
};
