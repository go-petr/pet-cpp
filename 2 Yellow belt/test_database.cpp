#include "../test_runner.h"
#include "../database.h"
#include "../date.h"
#include "../condition_parser.h"
#include <string>


function<bool(const Date &, const string &)> GetPredicateFromStr(const string &s) {
    istringstream is(s);
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date &date, const string &event) {
        return condition->Evaluate(date, event);
    };
    return predicate;
}


void TestDatabaseAdd() {
    // Test Add and Print methods
    {
        Database db;
        db.Add(Date(1, 1, 1), "event1");
        db.Add(Date(1, 1, 1), "event2");
        ostringstream os;
        db.Print(os);

        AssertEqual(os.str(), "0001-01-01 event1\n0001-01-01 event2\n", "Add events");
    }
    {
        Database db;
        db.Add(Date(1, 1, 1), "event1");
        db.Add(Date(1, 1, 1), "event1");
        db.Add(Date(1, 1, 3), "event1");
        db.Add(Date(1, 1, 3), "event1");
        ostringstream os;
        db.Print(os);

        AssertEqual(os.str(), "0001-01-01 event1\n0001-01-03 event1\n", "Add events twice");
    }
    {
        Database db;
        db.Add(Date(1, 1, 1), "event1");
        db.Add(Date(1, 1, 3), "event1");
        db.Add(Date(1, 1, 2), "event2");
        db.Add(Date(1, 1, 2), "event1");
        ostringstream os;
        db.Print(os);

        AssertEqual(os.str(), "0001-01-01 event1\n0001-01-02 event2\n"
                              "0001-01-02 event1\n0001-01-03 event1\n",
                    "Print data ascending, events by insertion order");
    }
}

void TestDatabaseRemoveIf() {
    // Test RemoveIf method
    {
        Database db;
        db.Add(Date(1, 1, 1), "event1");
        db.Add(Date(1, 1, 2), "event2");
        db.Add(Date(1, 2, 1), "event2");
        db.Add(Date(2, 1, 1), "event2");
        db.Add(Date(1, 1, 3), "event2");

        auto predicate = GetPredicateFromStr("event != \"holiday\"");
        AssertEqual(db.RemoveIf(predicate), 5, "Delete all if !=");
    }
    {
        Database db;
        db.Add(Date(1, 1, 1), "event1");
        auto predicate = GetPredicateFromStr("event != \"holiday\"");
        db.RemoveIf(predicate);
        ostringstream os;
        db.Print(os);

        AssertEqual(os.str(), "", "Check db emptiness");
    }
    {
        Database db;
        db.Add(Date(1, 1, 1), "event1");
        auto predicate = GetPredicateFromStr("event != \"holiday\"");
        db.RemoveIf(predicate);
        db.Add(Date(1, 1, 1), "event1");
        ostringstream os;
        db.Print(os);

        AssertEqual(os.str(), "0001-01-01 event1\n", "Add the deleted event");
    }
    {
        Database db;
        db.Add(Date(1000, 1, 1), "event1");
        db.Add(Date(2000, 1, 1), "holiday");
        db.Add(Date(2000, 1, 1), "sport event");
        db.Add(Date(3000, 1, 1), "holiday");
        db.Add(Date(3000, 1, 1), "sport event");

        auto predicate = GetPredicateFromStr(
                "date < 2017-01-01 AND (event == \"holiday\" OR event == \"sport event\")");
        AssertEqual(db.RemoveIf(predicate), 2, "Delete using complex condition");

        ostringstream os;
        db.Print(os);

        AssertEqual(os.str(), "1000-01-01 event1\n3000-01-01 holiday\n3000-01-01 sport event\n",
                    "Print after complex condition");
    }
};

void TestDatabaseFindIf() {
    {
        Database db;
        db.Add(Date(1, 1, 1), "event1");
        auto predicate = GetPredicateFromStr("");

        vector<string> expected = {"0001-01-01 event1"};
        vector<string> result = db.FindIf(predicate);
        AssertEqual(result, expected, "FindIf with empty condition");
    }
    {
        Database db;
        db.Add(Date(2017, 1, 1), "Holiday");
        db.Add(Date(2017, 3,8), "Holiday");
        db.Add(Date(2017, 1, 1), "New Year");
        auto predicate = GetPredicateFromStr("event != \"working day\"");
        vector<string> result = db.FindIf(predicate);
        db.Add(Date(2017, 05, 9), "Holiday");
        vector<string> expected = {"2017-01-01 Holiday", "2017-01-01 New Year", "2017-03-08 Holiday"};

        AssertEqual(result, expected, "FindIf with empty condition");
    }
};

void TestDatabaseLast() {
    {
        Database db;
        db.Add(Date(2017, 1, 1), "New Year");
        db.Add(Date(2017, 3,8), "Holiday");
        db.Add(Date(2017, 1, 1), "Holiday");
        string result;
        {
            istringstream is("2016-12-31");
            try {
                result = db.Last(ParseDate(is));
            } catch (invalid_argument &) {
                result = "No entries";
            }

            AssertEqual(result, "No entries", "Last with date less than all");
        }
        {
            istringstream is("2017-1-1");
            AssertEqual(db.Last(ParseDate(is)), "2017-01-01 Holiday", "Last event at the last date");
        }
        {
            istringstream is("2017-3-8");
            AssertEqual(db.Last(ParseDate(is)), "2017-03-08 Holiday", "Last event at the last date");
        }
    }

};