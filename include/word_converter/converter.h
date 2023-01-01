#pragma once

#include "input_reader.h"
#include "output_writer.h"

#include <algorithm>  // for_each
#include <memory>  // unique_ptr
#include <vector>


using input_reader_up = std::unique_ptr<input_reader>;
using output_writer_up_list = std::vector<std::unique_ptr<output_writer>>;


class converter {
    [[nodiscard]] static std::string convert(std::string text) {
        return text;
    }
public:
    static void run(input_reader_up reader, output_writer_up_list& writers) {
        for (std::string sentence{};;) {
            sentence = reader->read_sentence();
            if (reader->eof()) {  // input stream empty or ending with something other than a period
                if (not sentence.empty()) {
                    std::ranges::for_each(writers, [&sentence](auto& writer) { writer->write(sentence); });
                }
                break;
            } else {
                sentence = convert(sentence);
                std::ranges::for_each(writers, [&sentence](auto& writer) { writer->write_sentence(sentence); });
            }
        }
    }
};
