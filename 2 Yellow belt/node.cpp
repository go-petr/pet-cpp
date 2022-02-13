#include "node.h"
#include "date.h"
#include <memory>

using namespace std;

bool EmptyNode::Evaluate(const Date& date, const string& event) const {
    return true;
}

DateComparisonNode::DateComparisonNode(const Comparison& cmp, const Date& dateToCompare)
        : cmp(cmp), dateToCompare(dateToCompare) {
}

bool DateComparisonNode::Evaluate(const Date& date, const string& event) const {

    switch (cmp) {
    case Comparison::Less:
        return date < dateToCompare;
    case Comparison::LessOrEqual:
        return date < dateToCompare || date == dateToCompare;
    case Comparison::Greater:
        return !(date < dateToCompare) && date != dateToCompare;
    case Comparison::GreaterOrEqual:
        return !(date < dateToCompare);
    case Comparison::Equal:
        return date == dateToCompare;
    case Comparison::NotEqual:
        return date != dateToCompare;
    }
}


EventComparisonNode::EventComparisonNode(const Comparison& cmp, const string& eventToCompare)
        : cmp(cmp), eventToCompare(eventToCompare) {
}

bool EventComparisonNode::Evaluate(const Date& date, const string& event) const {
    switch (cmp) {
    case Comparison::Less:
        return event < eventToCompare;
    case Comparison::LessOrEqual:
        return event <= eventToCompare;
    case Comparison::Greater:
        return event > eventToCompare;
    case Comparison::GreaterOrEqual:
        return event >= eventToCompare;
    case Comparison::Equal:
        return event == eventToCompare;
    case Comparison::NotEqual:
        return event != eventToCompare;
    }
}


LogicalOperationNode::LogicalOperationNode(const enum LogicalOperation& logicalOperation,
        const shared_ptr<Node>& leftExpression,
        const shared_ptr<Node>& rightExpression)
        : logicalOperation(logicalOperation),
          leftExpression(leftExpression),
          rightExpression(rightExpression) {
}

bool LogicalOperationNode::Evaluate(const Date& date, const string& event) const {
    switch (logicalOperation) {
    case LogicalOperation::Or:
        return leftExpression->Evaluate(date, event) || rightExpression->Evaluate(date, event);
    case LogicalOperation::And:
        return leftExpression->Evaluate(date, event) && rightExpression->Evaluate(date, event);
    }
}

