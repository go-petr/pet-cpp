#pragma once

#include "token.h"
#include "date.h"
#include <iostream>
#include <memory>

using namespace std;

struct Node { // базовый класс
    virtual bool Evaluate(const Date& date, const string& event) const = 0;
};

struct EmptyNode : public Node {
    bool Evaluate(const Date& date, const string& event) const override;
};


struct DateComparisonNode : public Node {
public:
    DateComparisonNode(const Comparison& cmp, const Date& dateToCompare);

    bool Evaluate(const Date& date, const string& event) const override;

private:
    const Comparison cmp;
    const Date dateToCompare;
};

struct EventComparisonNode : public Node {
public:
    EventComparisonNode(const Comparison& cmp, const string& eventToCompare);

    bool Evaluate(const Date& date, const string& event) const override;

private:
    const Comparison cmp;
    const string eventToCompare;
};

struct LogicalOperationNode : public Node {
public:
    LogicalOperationNode(const LogicalOperation& logicalOperation, const shared_ptr<Node>& leftExpression,
            const shared_ptr<Node>& rightExpression);

    bool Evaluate(const Date& date, const string& event) const override;

private:
    const LogicalOperation logicalOperation;
    const shared_ptr<Node> leftExpression;
    const shared_ptr<Node> rightExpression;
};