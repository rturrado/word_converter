#pragma once

#include "ast.h"
#include "input_reader.h"
#include "lexer.h"

#include <fmt/core.h>
#include <memory>  // make_unique, unique_ptr
#include <ranges>
#include <rtc/string.h>
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
    { "hundred", 100 },  // a hundred
    { "thousand", 1'000 },  // a thousand
    { "million", 1'000'000 },  // a million
    { "billion", 1'000'000'000 }  // a billion
};


struct invalid_token_error : public std::runtime_error {
    invalid_token_error(const token_t& token, const std::string& node_str) : std::runtime_error{ "" } {
        message_ = fmt::format("invalid token: '{}', while parsing node: '{}'", token, node_str);
    }
    [[nodiscard]] const char* what() const noexcept override { return message_.c_str(); };
private:
    std::string message_{};
};


class parser {
    std::unique_ptr<lexer> lexer_{};
    std::unique_ptr<ast::tree> ast_{};
private:
    void advance_to_next_token(auto& node) {
        lexer_->advance_to_next_token();
        if (lexer_->get_current_lexeme() == lexeme_t::space) {
            node.add(ast::text_node{ lexer_->get_current_text() });
            lexer_->advance_to_next_token();
        }
    }
private:
    [[nodiscard]] bool end() {
        return (lexer_->get_current_lexeme() == lexeme_t::end);
    }
    [[nodiscard]] bool space(auto& node) {
        if (lexer_->get_current_lexeme() == lexeme_t::space) {
            node.add(ast::text_node{ lexer_->get_current_text() });
            advance_to_next_token(node);
            return true;
        }
        return false;
    }
    [[nodiscard]] bool dash(auto& node) {
        if (lexer_->get_current_lexeme() == lexeme_t::dash) {
            node.add(ast::text_node{ lexer_->get_current_text() });
            advance_to_next_token(node);
            return true;
        }
        return false;
    }
    [[nodiscard]] bool period(auto& node) {
        if (lexer_->get_current_lexeme() == lexeme_t::period) {
            node.add(ast::text_node{ lexer_->get_current_text() });
            advance_to_next_token(node);
            return true;
        }
        return false;
    }
    [[nodiscard]] bool and_connector(auto& node) {
        if (lexer_->get_current_lexeme() == lexeme_t::and_connector) {
            node.add(ast::text_node{ lexer_->get_current_text() });
            advance_to_next_token(node);
            return true;
        }
        return false;
    }
    [[nodiscard]] bool other(auto& node) {
        if (lexer_->get_current_lexeme() == lexeme_t::other) {
            node.add(ast::text_node{ lexer_->get_current_text() });
            advance_to_next_token(node);
            return true;
        }
        return false;
    }
    [[nodiscard]] bool zero(auto& node) {
        if (lexer_->get_current_lexeme() == lexeme_t::zero) {
            node.add(ast::int_node{ 0 });
            advance_to_next_token(node);
            return true;
        }
        return false;
    }
    [[nodiscard]] bool one(auto& node) {
        if (lexer_->get_current_lexeme() == lexeme_t::one) {
            node.add(ast::int_node{ 1 });
            advance_to_next_token(node);
            return true;
        }
        return false;
    }
    [[nodiscard]] bool two_to_nine(auto& node) {
        if (lexer_->get_current_lexeme() == lexeme_t::two_to_nine) {
            auto word_lc{ rtc::string::to_lowercase(lexer_->get_current_text()) };
            auto one_to_nine_number{ word_to_number_map.at(word_lc) };
            node.add(ast::int_node{ one_to_nine_number });
            advance_to_next_token(node);
            return true;
        }
        return false;
    }
    [[nodiscard]] bool one_to_nine(auto& node) {
        return one(node) or two_to_nine(node);
    }
    [[nodiscard]] bool ten_to_nineteen(auto& node) {
        if (lexer_->get_current_lexeme() == lexeme_t::ten_to_nineteen) {
            auto word_lc{ rtc::string::to_lowercase(lexer_->get_current_text()) };
            auto ten_to_nineteen_number{ word_to_number_map.at(word_lc) };
            node.add(ast::int_node{ ten_to_nineteen_number });
            advance_to_next_token(node);
            return true;
        }
        return false;
    }
    [[nodiscard]] bool twenty_to_ninety_nine(auto& node) {
        if (lexer_->get_current_lexeme() == lexeme_t::tens) {
            auto word_lc{ rtc::string::to_lowercase(lexer_->get_current_text()) };
            auto tens_number{ word_to_number_map.at(word_lc) };
            node.add(ast::int_node{ tens_number });
            advance_to_next_token(node);
            if (dash(node)) {
                return one_to_nine(node);
            } else {
                (void) one_to_nine(node);
                return true;
            }
        }
        return false;
    }
    [[nodiscard]] bool ten_to_ninety_nine(auto& node) {
        return ten_to_nineteen(node) or twenty_to_ninety_nine(node);
    }
    [[nodiscard]] bool one_to_ninety_nine(auto& node) {
        return one_to_nine(node) or ten_to_ninety_nine(node);
    }
    [[nodiscard]] bool below_one_hundred(auto& node) {
        return (and_connector(node) and one_to_ninety_nine(node));
    }
    [[nodiscard]] bool hundred(auto& node) {
        if (lexer_->get_current_lexeme() == lexeme_t::hundred) {
            node.add(ast::int_node{ 100 });
            advance_to_next_token(node);
            return true;
        }
        return false;
    }
    [[nodiscard]] bool hundreds(auto& node) {
        if (one_to_nine(node)) {
            if (hundred(node)) {
                (void) below_one_hundred(node);
            }
            return true;
        }
        return false;
    }
    [[nodiscard]] bool below_one_thousand(auto& node) {
        return (and_connector(node) and one_to_ninety_nine(node)) or
            (one_to_nine(node) and hundred(node) and and_connector(node) and one_to_ninety_nine(node));
    }
    [[nodiscard]] bool thousand(auto& node) {
        if (lexer_->get_current_lexeme() == lexeme_t::thousand) {
            node.add(ast::int_node{ 1'000 });
            advance_to_next_token(node);
            return true;
        }
        return false;
    }
    [[nodiscard]] bool thousands(auto& node) {
        if (hundreds(node) or twenty_to_ninety_nine(node) or ten_to_nineteen(node)) {
            if (thousand(node)) {
                (void) below_one_thousand(node);
            } else if (hundred(node)) {
                (void) below_one_hundred(node);
            }
            return true;
        }
        return false;
    }
    [[nodiscard]] bool below_one_million(auto& node) {
        return (and_connector(node) and one_to_ninety_nine(node)) or
            thousands(node);
    }
    [[nodiscard]] bool million(auto& node) {
        if (lexer_->get_current_lexeme() == lexeme_t::million) {
            node.add(ast::int_node{ 1'000'000 });
            advance_to_next_token(node);
            return true;
        }
        return false;
    }
    [[nodiscard]] bool millions(auto& node) {
        if (hundreds(node) or twenty_to_ninety_nine(node) or ten_to_nineteen(node)) {
            if (million(node)) {
                (void) below_one_million(node);
            } else if (thousand(node)) {
                (void) below_one_thousand(node);
            } else if (hundred(node)) {
                (void) below_one_hundred(node);
            }
            return true;
        }
        return false;
    }
    [[nodiscard]] bool below_one_billion(auto& node) {
        return (and_connector(node) and one_to_ninety_nine(node)) or
            millions(node);
    }
    [[nodiscard]] bool billion(auto& node) {
        if (lexer_->get_current_lexeme() == lexeme_t::billion) {
            node.add(ast::int_node{ 1'000'000'000 });
            advance_to_next_token(node);
            return true;
        }
        return false;
    }
    [[nodiscard]] bool billions(auto& node) {
        if (hundreds(node) or twenty_to_ninety_nine(node) or ten_to_nineteen(node)) {
            if (billion(node)) {
                (void) below_one_billion(node);
            } else if (million(node)) {
                (void) below_one_million(node);
            } else if (thousand(node)) {
                (void) below_one_thousand(node);
            } else if (hundred(node)) {
                (void) below_one_hundred(node);
            }
            return true;
        }
        return false;
    }
    [[nodiscard]] bool number_expression(auto& parent_node) {
        ast::number_expression_node node{};
        if (zero(node) or billions(node)) {
            parent_node.add(std::move(node));
            return true;
        }
        return false;
    }
    [[nodiscard]] bool text_without_number_expression(auto& node) {
        return (space(node) or dash(node) or and_connector(node) or other(node));
    }
    [[nodiscard]] bool text_without_number_expressions(auto& node) {
        while (text_without_number_expression(node));
        return true;
    }
    [[nodiscard]] bool rest_of_sentence_body(auto& node) {
        return end() or
            period(node) or
            (text_without_number_expression(node) and sentence(node));
    }
    [[nodiscard]] bool sentence_body(auto& node) {
        return end() or
            period(node) or
            (number_expression(node) and rest_of_sentence_body(node));
    }
    [[nodiscard]] bool sentence_prefix(auto& node) {
        return (text_without_number_expressions(node));
    }
    [[nodiscard]] bool sentence(auto& node) {
        return (sentence_prefix(node) and sentence_body(node));
    }

    void sentences() {
        while (not end()) {
            ast::sentence_node node{};
            if (sentence(node)) {
                ast_->add(std::move(node));
            } else {
                throw invalid_token_error{ lexer_->get_current_token(), node.dump() };
            }
        }
    }
    void start() {
        sentences();
    }
public:
    explicit parser(input_reader_up reader)
        : lexer_{ std::make_unique<lexer>(std::move(reader)) }
        , ast_{ std::make_unique<ast::tree>() }
    {}
    [[nodiscard]] std::string parse() {
        start();
        return ast_->evaluate();
    }
};


using parser_up = std::unique_ptr<parser>;
