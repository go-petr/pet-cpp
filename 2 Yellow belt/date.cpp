#include "date.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <tuple>

using namespace std;


Date::Date(const int& newYear, const int& newMonth, const int& newDay) {
    if (newMonth < 1 || newMonth > 12) {
        throw runtime_error("Month value is invalid: " + to_string(newMonth));
    }
    if (newDay < 1 || newDay > 31) {
        throw runtime_error("Day value is invalid: " + to_string(newDay));
    }
    year = newYear;
    month = newMonth;
    day = newDay;
}

int Date::GetYear() const {
    return year;
}

int Date::GetMonth() const {
    return month;
}

int Date::GetDay() const {
    return day;
}


ostream& operator<<(ostream& stream, const Date& date) {
    stream << setw(4) << setfill('0') << date.GetYear() <<
           "-" << setw(2) << setfill('0') << date.GetMonth() <<
           "-" << setw(2) << setfill('0') << date.GetDay();
    return stream;
}

bool operator<(const Date& lhs, const Date& rhs) {
    return make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()) <
           make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}


bool operator==(const Date& lhs, const Date& rhs) {
    return make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()) ==
           make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

bool operator!=(const Date& lhs, const Date& rhs) {
    return !(lhs == rhs);
}


Date ParseDate(istream& dateStream) {
    auto p = dateStream.tellg();

    bool ok = true;

    int year;
    ok = ok && (dateStream >> year);
    ok = ok && (dateStream.peek() == '-');
    dateStream.ignore(1);

    int month;
    ok = ok && (dateStream >> month);
    ok = ok && (dateStream.peek() == '-');
    dateStream.ignore(1);

    int day;
    ok = ok && (dateStream >> day);

    if (!ok) {
        string input;
        dateStream.clear();
        dateStream.seekg(p, std::ios_base::beg);
        getline(dateStream, input);
        throw logic_error("Wrong date format: " + input);
    }
    return { year, month, day };
}

