#pragma once

#include <algorithm>  // for_each
#include <fmt/format.h>
#include <numeric>  // accumulate
#include <stdexcept>  // runtime_error
#include <string>  // to_string
#include <variant>  // visit
#include <vector>


struct invalid_number_expression_error : public std::runtime_error {
    explicit invalid_number_expression_error(const std::string& message) : std::runtime_error{ "" } {
        message_ += fmt::format("'{}'", message);
    }
    [[nodiscard]] const char* what() const noexcept override { return message_.c_str(); };
private:
    std::string message_{ "invalid number expression error: " };
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
    [[nodiscard]] std::string to_string() const { return data; }
};


struct int_node {
    int data{};
    explicit int_node(int value) : data{ value } {}
    [[nodiscard]] std::string to_string() const { return std::to_string(data); }
};


class number_expression_node {
    using node_t = std::variant<int_node, text_node>;
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
    [[nodiscard]] std::string to_string() const {
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
    [[nodiscard]] std::string to_string() const {
        std::string ret{};
        std::ranges::for_each(nodes_, [&ret](auto&& node) {
            std::visit([&ret](auto&& arg) { ret += arg.to_string(); }, node);
        });
        return ret;
    }
};


class tree {
    sentence_node start_;
public:
    void add(sentence_node n) {
        start_ = std::move(n);
    }
    [[nodiscard]] std::string to_string() const {
        return start_.to_string();
    }
};

}   // namespace ast
