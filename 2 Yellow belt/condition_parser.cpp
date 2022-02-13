#include "condition_parser.h"
#include "token.h"

#include <map>

using namespace std;

template<class It>
shared_ptr<Node> ParseComparison(It& current, It end) {
    if (current == end) {
        throw logic_error("Expected column name: date or event");
    }

    Token& column = *current;
    if (column.Type != TokenType::COLUMN) {
        throw logic_error("Expected column name: date or event");
    }
    ++current;

    if (current == end) {
        throw logic_error("Expected comparison operation");
    }

    Token& op = *current;
    if (op.Type != TokenType::COMPARE_OP) {
        throw logic_error("Expected comparison operation");
    }
    ++current;

    if (current == end) {
        throw logic_error("Expected right Value of comparison");
    }

    Comparison cmp;
    if (op.Value == "<") {
        cmp = Comparison::Less;
    }
    else if (op.Value == "<=") {
        cmp = Comparison::LessOrEqual;
    }
    else if (op.Value == ">") {
        cmp = Comparison::Greater;
    }
    else if (op.Value == ">=") {
        cmp = Comparison::GreaterOrEqual;
    }
    else if (op.Value == "==") {
        cmp = Comparison::Equal;
    }
    else if (op.Value == "!=") {
        cmp = Comparison::NotEqual;
    }
    else {
        throw logic_error("Unknown comparison token: " + op.Value);
    }

    const string& value = current->Value;
    ++current;

    if (column.Value == "date") {
        istringstream is(value);
        return make_shared<DateComparisonNode>(cmp, ParseDate(is));
    }
    else {
        return make_shared<EventComparisonNode>(cmp, value);
    }
}

template<class It>
shared_ptr<Node> ParseExpression(It& current, It end, unsigned precedence) {
    if (current == end) {
        return shared_ptr<Node>();
    }

    shared_ptr<Node> left;

    if (current->Type == TokenType::PAREN_LEFT) {
        ++current; // consume '('
        left = ParseExpression(current, end, 0u);
        if (current == end || current->Type != TokenType::PAREN_RIGHT) {
            throw logic_error("Missing right paren");
        }
        ++current; // consume ')'
    }
    else {
        left = ParseComparison(current, end);
    }

    const map<LogicalOperation, unsigned> precedences = {
            { LogicalOperation::Or,  1 },
            { LogicalOperation::And, 2 }
    };

    while (current != end && current->Type != TokenType::PAREN_RIGHT) {
        if (current->Type != TokenType::LOGICAL_OP) {
            throw logic_error("Expected logic operation");
        }

        const auto logicalOperation = current->Value == "AND" ? LogicalOperation::And
                                                              : LogicalOperation::Or;
        const auto currentPrecedence = precedences.at(logicalOperation);
        if (currentPrecedence <= precedence) {
            break;
        }

        ++current; // consume op

        left = make_shared<LogicalOperationNode>(
                logicalOperation, left, ParseExpression(current, end, currentPrecedence)
        );
    }

    return left;
}

shared_ptr<Node> ParseCondition(istream& is) {
    auto tokens = Tokenize(is);
    auto current = tokens.begin();
    auto topNode = ParseExpression(current, tokens.end(), 0u);

    if (!topNode) {
        topNode = make_shared<EmptyNode>();
    }

    if (current != tokens.end()) {
        throw logic_error("Unexpected tokens after condition");
    }

    return topNode;
}