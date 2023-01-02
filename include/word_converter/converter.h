#pragma once

#include "input_reader.h"
#include "output_writer.h"

#include <algorithm>  // for_each
#include <memory>  // unique_ptr
#include <regex>
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
    [[nodiscard]] virtual std::string convert(std::string text) = 0;
};
using converter_up = std::unique_ptr<converter>;


class word_to_number_converter : public converter {
public:
    [[nodiscard]] std::string convert(std::string text) override {
        std::string ret{};
        std::regex word_pattern{ R"([a-zA-Z]+)" };
        std::smatch sm{};
        while (std::regex_search(text, sm, word_pattern)) {
            ret += sm.prefix();
            const auto& word{ sm.str() };
            ret += word_to_number_map.contains(word)
                   ? std::to_string(word_to_number_map.at(word))
                   : word;
            text = sm.suffix();
        }
        ret += text;
        return ret;
    }
};


class conversion_manager {
    std::unique_ptr<converter> converter_{};
public:
    explicit conversion_manager(std::unique_ptr<converter> c)
        : converter_{ std::move(c) }
    {}

    void run(input_reader_up reader, output_writer_up_list& writers) {
        for (std::string sentence{};;) {
            sentence = reader->read_sentence();
            if (reader->eof()) {  // input stream empty
                if (not sentence.empty()) {  // or ending with something other than a period
                    std::ranges::for_each(writers, [&sentence](auto& writer) { writer->write(sentence); });
                }
                break;
            } else {  // sentence read
                sentence = converter_->convert(sentence);
                std::ranges::for_each(writers, [&sentence](auto& writer) { writer->write_sentence(sentence); });
            }
        }
    }
};
