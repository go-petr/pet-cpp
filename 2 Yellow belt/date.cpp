#include "date.h"
#include <string>
#include <istream>
#include <iomanip>

using namespace std;


Date::Date(const int &new_year, const int &new_month, const int &new_day) {
    if (new_month < 1 || new_month > 12) {
        throw runtime_error("Month value is invalid: " + to_string(new_month));
    }
    if (new_day < 1 || new_day > 31) {
        throw runtime_error("Day value is invalid: " + to_string(new_day));
    }
    year = new_year;
    month = new_month;
    day = new_day;
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


ostream &operator<<(ostream &stream, const Date &date) {
    stream << setw(4) << setfill('0') << date.GetYear() <<
           "-" << setw(2) << setfill('0') << date.GetMonth() <<
           "-" << setw(2) << setfill('0') << date.GetDay();
    return stream;
}

bool operator<(const Date &lhs, const Date &rhs) {
    return tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()) < tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
};


bool operator==(const Date &lhs, const Date &rhs) {
    return tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()) == tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
};


Date ParseDate(istringstream &date_stream) {

    bool ok = true;

    int year;
    ok = ok && (date_stream >> year);
    ok = ok && (date_stream.peek() == '-');
    date_stream.ignore(1);

    int month;
    ok = ok && (date_stream >> month);
    ok = ok && (date_stream.peek() == '-');
    date_stream.ignore(1);

    int day;
    ok = ok && (date_stream >> day);
    ok = ok && date_stream.eof();

    if (!ok) {
        throw logic_error("Wrong date format: " + date_stream.str());
    }
    return {year, month, day};
}

