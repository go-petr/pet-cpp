#include "date.h"
#include "test_runner.h"
#include <string>
#include <tuple>


using namespace std;

string ParseDateException(istringstream& date_stream) {
    // Return string from the exception thrown in the ParseDate function.
    // If no exception was thrown, return string "no exception".
    try {
        ParseDate(date_stream);
    }
    catch (exception& e) {
        return static_cast<string>(e.what());
    }
    return "no exception";
}


void TestParseDate() {
    // Test "Wrong date format" type exceptions

    string input, expected;

    expected = "Wrong date format: ";

    {
        input = "X-1-1";
        istringstream istream(input);
        AssertEqual(ParseDateException(istream), expected + input, "Wrong year format");
    }

    {
        input = "1-X-1";
        istringstream istream(input);
        AssertEqual(ParseDateException(istream), expected + input, "Wrong month format");
    }

    {
        input = "1-1-X";
        istringstream istream(input);
        AssertEqual(ParseDateException(istream), expected + input, "Wrong day format");
    }

    {
        input = "*1-1-1";
        istringstream istream(input);
        AssertEqual(ParseDateException(istream), expected + input, "Wrong first char format");
    }

    {
        input = "1*1-1";
        istringstream istream(input);
        AssertEqual(ParseDateException(istream), expected + input, "Wrong second char format");
    }

    {
        input = "1-1*1";
        istringstream istream(input);
        AssertEqual(ParseDateException(istream), expected + input, "Wrong third char format");
    }

    {
        input = "1 1 1";
        istringstream istream(input);
        AssertEqual(ParseDateException(istream), expected + input, "Char absence");
    }

    {
        input = "-1-1";
        istringstream istream(input);
        AssertEqual(ParseDateException(istream), expected + input, "Year absence");
    }

    {
        input = "1--1";
        istringstream istream(input);
        AssertEqual(ParseDateException(istream), expected + input, "Month absence");
    }

    {
        input = "1-1-";
        istringstream istream(input);
        AssertEqual(ParseDateException(istream), expected + input, "Day absence");
    }

    {
        input = "1-20-";
        istringstream istream(input);
        AssertEqual(ParseDateException(istream), expected + input, "Wrong date format and month");
    }

    {
        input = "-100-1-1";
        istringstream istream(input);
        Date result = ParseDate(istream);
        AssertEqual(result, Date(-100, 1, 1), "Negative year");
    }

    {
        input = "+100-1-1";
        istringstream istream(input);
        Date result = ParseDate(istream);
        AssertEqual(result, Date(100, 1, 1), "+ sign before year");
    }

    {
        input = "1-+1-1";
        istringstream istream(input);
        Date result = ParseDate(istream);
        AssertEqual(result, Date(1, 1, 1), "+ sign before month");
    }

    {
        input = "1-1-+1";
        istringstream istream(input);
        Date result = ParseDate(istream);
        AssertEqual(result, Date(1, 1, 1), "+ sign before day");
    }

    {
        input = "1+-1-1";
        istringstream istream(input);
        AssertEqual(ParseDateException(istream), expected + input, "+ sign after year");
    }

    {
        input = "1-1+-1";
        istringstream istream(input);
        AssertEqual(ParseDateException(istream), expected + input, "+ sign after");
    }


    // Test month and day values
    {
        input = "1-20-1";
        istringstream istream(input);
        AssertEqual(ParseDateException(istream), "Month value is invalid: 20", "");
    }

    {
        input = "1-1-100";
        istringstream istream(input);
        AssertEqual(ParseDateException(istream), "Day value is invalid: 100", "");
    }

    {
        input = "1-20-100";
        istringstream istream(input);
        AssertEqual(ParseDateException(istream), "Month value is invalid: 20", "");
    }
}
