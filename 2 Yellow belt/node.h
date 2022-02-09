#pragma once

#include "token.h"
#include "date.h"

struct Node { // базовый класс
    virtual bool Evaluate(const Date &date, const string &event) const = 0;
};

struct EmptyNode : public Node {
    bool Evaluate(const Date &date, const string &event) const override {
        return false;
    };
};


struct DateComparisonNode : public Node {
public:
    DateComparisonNode(const Comparison &cmp, const Date &date_to_compare);

    bool Evaluate(const Date &date, const string &event) const override;

private:
    const Comparison cmp_;
    const Date date_to_compare_;
};

struct EventComparisonNode : public Node {
public:
    EventComparisonNode(const Comparison &cmp, const string &event_to_compare);

    bool Evaluate(const Date &date, const string &event) const override;

private:
    const Comparison cmp_;
    const string event_to_compare_;
};

struct LogicalOperationNode : public Node {
public:
    LogicalOperationNode(const LogicalOperation &logical_operation, const shared_ptr<Node> &left_expression,
                         const shared_ptr<Node> &right_expression);

    bool Evaluate(const Date &date, const string &event) const override;

private:
    const LogicalOperation logical_operation_;
    const shared_ptr<Node> left_expression_;
    const shared_ptr<Node> right_expression_;
};