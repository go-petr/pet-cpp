#include "node.h"
#include "date.h"

//using namespace std;

DateComparisonNode::DateComparisonNode(const Comparison &cmp, const Date &date_to_compare)
        : cmp_(cmp), date_to_compare_(date_to_compare) {}

bool DateComparisonNode::Evaluate(const Date &date, const string &event) const {

    switch (cmp_) {
        case Comparison::Less:
            return date < date_to_compare_;
        case Comparison::LessOrEqual:
            return date < date_to_compare_ || date == date_to_compare_;
        case Comparison::Greater:
            return !(date < date_to_compare_) && date != date_to_compare_;
        case Comparison::GreaterOrEqual:
            return !(date < date_to_compare_);
        case Comparison::Equal:
            return date == date_to_compare_;
        case Comparison::NotEqual:
            return date != date_to_compare_;
    }
}


EventComparisonNode::EventComparisonNode(const Comparison &cmp, const string &event_to_compare)
        : cmp_(cmp), event_to_compare_(event_to_compare) {}

bool EventComparisonNode::Evaluate(const Date &date, const string &event) const {
    switch (cmp_) {
        case Comparison::Less:
            return event < event_to_compare_;
        case Comparison::LessOrEqual:
            return event < event_to_compare_ || event == event_to_compare_;
        case Comparison::Greater:
            return !(event < event_to_compare_) && event != event_to_compare_;
        case Comparison::GreaterOrEqual:
            return !(event < event_to_compare_);
        case Comparison::Equal:
            return event == event_to_compare_;
        case Comparison::NotEqual:
            return event != event_to_compare_;
    }
}


LogicalOperationNode::LogicalOperationNode(const LogicalOperation &logical_operation,
                                           const shared_ptr<Node> &left_expression,
                                           const shared_ptr<Node> &right_expression)
        : logical_operation_(logical_operation),
          left_expression_(left_expression),
          right_expression_(right_expression) {}

bool LogicalOperationNode::Evaluate(const Date &date, const string &event) const {
    switch (logical_operation_) {
        case LogicalOperation::Or:
            return left_expression_->Evaluate(date, event) || right_expression_->Evaluate(date, event);
        case LogicalOperation::And:
            return left_expression_->Evaluate(date, event) && right_expression_->Evaluate(date, event);
    }
}

