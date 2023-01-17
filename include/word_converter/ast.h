#pragma once

#include <algorithm>  // for_each
#include <fmt/format.h>
#include <numeric>  // accumulate
#include <stdexcept>  // runtime_error
#include <string>  // to_string
#include <unordered_map>
#include <variant>  // visit
#include <vector>


inline static const std::unordered_map<int, std::string> number_to_word_map{
    { 0, "zero" },  // zero
    { 1, "one" },  // one
    { 2, "two" },  // two to nine
    { 3, "three" },
    { 4, "four" },
    { 5, "five" },
    { 6, "six" },
    { 7, "seven" },
    { 8, "eight" },
    { 9, "nine" },
    { 10, "ten" },  // ten to nineteen
    { 11, "eleven" },
    { 12, "twelve" },
    { 13, "thirteen" },
    { 14, "fourteen" },
    { 15, "fifteen" },
    { 16, "sixteen" },
    { 17, "seventeen" },
    { 18, "eighteen" },
    { 19, "nineteen" },
    { 20, "twenty" },  // tens
    { 30, "thirty" },
    { 40, "forty" },
    { 50, "fifty" },
    { 60, "sixty" },
    { 70, "seventy" },
    { 80, "eighty" },
    { 90, "ninety" },
    { 100, "hundred" },  // a hundred
    { 1'000, "thousand" },  // a thousand
    { 1'000'000, "million" },  // a million
    { 1'000'000'000, "billion" }  // a billion
};


struct invalid_number_expression_error : public std::runtime_error {
    explicit invalid_number_expression_error(const std::string& number_expression_str) : std::runtime_error{ "" } {
        message_ += fmt::format("'{}'", number_expression_str);
    }
    [[nodiscard]] const char* what() const noexcept override { return message_.c_str(); };
private:
    std::string message_{ "invalid number expression: " };
};


class number_expression_stack {
    std::vector<int> numbers_{};
public:
    void push(int number) {
        if (numbers_.empty()) {
            numbers_.push_back(number);
        } else if (number > numbers_.back()) {
            int sum{};
            while ((not numbers_.empty()) and (sum + numbers_.back() < number)) {
                sum += numbers_.back();
                numbers_.pop_back();
            }
            numbers_.push_back(number * sum);
        } else if (number < numbers_.back()) {
            numbers_.push_back(number);
        } else if (number == numbers_.back()) {
            throw invalid_number_expression_error{
                fmt::format("{} {}", number, numbers_.back())
            };
        }
    }
    [[nodiscard]] int value() const {
        return std::accumulate(numbers_.begin(), numbers_.end(), 0);
    }
};


namespace ast {

struct text_node {
    std::string data{};
    explicit text_node(std::string text) : data{ std::move(text) } {}
    [[nodiscard]] std::string dump() const { return data; }
    [[nodiscard]] std::string evaluate() const { return data; }
};


struct int_node {
    int data{};
    explicit int_node(int value) : data{ value } {}
    [[nodiscard]] std::string dump() const { return number_to_word_map.at(data); }
    [[nodiscard]] std::string evaluate() const { return std::to_string(data); }
};


class number_expression_node {
    using node_t = std::variant<text_node, int_node>;
    using nodes_t = std::vector<node_t>;
private:
    nodes_t nodes_{};
public:
    void add(node_t n) {
        nodes_.push_back(std::move(n));
    }
    [[nodiscard]] int value() const {
        number_expression_stack numbers_stack{};
        std::ranges::for_each(nodes_, [&numbers_stack](const auto& node) {
            if (std::holds_alternative<int_node>(node)) {
                numbers_stack.push(std::get<int_node>(node).data);
            }
        });
        return numbers_stack.value();
    }
    [[nodiscard]] std::string dump() const {
        std::string ret{};
        std::ranges::for_each(nodes_, [&ret](auto&& node) {
            std::visit([&ret](auto&& arg) { ret += arg.dump(); }, node);
        });
        return ret;
    }
    [[nodiscard]] std::string evaluate() const {
        if (nodes_.empty()) {
            return {};
        }
        auto expression_value{ value() };
        std::string ret{ std::to_string(expression_value) };
        if (std::holds_alternative<text_node>(nodes_.back())) {
            ret += std::get<text_node>(nodes_.back()).data;
        }
        return ret;
    }
};


class sentence_node {
    using node_t = std::variant<text_node, number_expression_node>;
    using nodes_t = std::vector<node_t>;
private:
     nodes_t nodes_{};
public:
    void add(node_t n) {
        nodes_.push_back(std::move(n));
    }
    [[nodiscard]] std::string dump() const {
        std::string ret{};
        std::ranges::for_each(nodes_, [&ret](auto&& node) {
            std::visit([&ret](auto&& arg) { ret += arg.dump(); }, node);
        });
        return ret;
    }
    [[nodiscard]] std::string evaluate() const {
        std::string ret{};
        std::ranges::for_each(nodes_, [&ret](auto&& node) {
            std::visit([&ret](auto&& arg) { ret += arg.evaluate(); }, node);
        });
        return ret;
    }
};


class tree {
    using node_t = sentence_node;
    using nodes_t = std::vector<node_t>;
private:
    nodes_t nodes_{};
public:
    void add(node_t n) {
        nodes_.push_back(std::move(n));
    }
    [[nodiscard]] std::string dump() const {
        return std::accumulate(nodes_.begin(), nodes_.end(), std::string{}, [](const auto& total, const auto& node) {
            return total + node.dump();
        });
    }
    [[nodiscard]] std::string evaluate() const {
        return std::accumulate(nodes_.begin(), nodes_.end(), std::string{}, [](const auto& total, const auto& node) {
            return total + node.evaluate();
        });
    }
};

}   // namespace ast
