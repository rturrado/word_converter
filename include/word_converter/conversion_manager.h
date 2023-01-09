#pragma once

#include "input_reader.h"
#include "output_writer.h"
#include "parser.h"

#include <algorithm>  // for_each
#include <string>

using input_reader_up = std::unique_ptr<input_reader>;
using output_writer_up_list = std::vector<std::unique_ptr<output_writer>>;


struct conversion_manager {
    [[nodiscard]] static bool is_sentence(const std::string& text) {
        return text.ends_with(".");
    }

    static void run(input_reader_up reader, output_writer_up_list& writers) {
        while (not reader->eof()) {
            auto text{ reader->read() };
            // Texts that do not form a sentence (i.e. that do not end in a period) are not converted
            if (is_sentence(text)) {
                text = parser::parse(text);
            }
            // All the texts are written out though
            std::ranges::for_each(writers, [&text](auto& writer) { writer->write(text); });
        }
    }
};
