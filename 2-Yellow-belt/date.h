#pragma once

#include <sstream>

using namespace std;

class Date {
public:
    Date(const int& newYear, const int& newMonth, const int& newDay);

    int GetYear() const;

    int GetMonth() const;

    int GetDay() const;

private:
    int year, month, day;
};

ostream& operator<<(ostream& stream, const Date& date);

bool operator<(const Date& lhs, const Date& rhs);

bool operator==(const Date& lhs, const Date& rhs);

bool operator!=(const Date& lhs, const Date& rhs);

Date ParseDate(istream& dateStream);

void TestParseDate();