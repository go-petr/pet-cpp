#pragma once

#include <sstream>

using namespace std;

class Date {
public:
    Date(const int &new_year, const int &new_month, const int &new_day);

    int GetYear() const;

    int GetMonth() const;

    int GetDay() const;

private:
    int year, month, day;
};

ostream &operator<<(ostream &stream, const Date &date);

bool operator<(const Date &lhs, const Date &rhs);

bool operator==(const Date &lhs, const Date &rhs);

Date ParseDate(istringstream &is);

void TestParseDate();