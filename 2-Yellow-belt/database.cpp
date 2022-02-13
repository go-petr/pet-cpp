#include "database.h"
#include "date.h"
#include <iostream>
#include <vector>

using namespace std;

void Database::Add(const Date& date, const string& event) {
    if (uniqueEvents.count(pair(date, event)) == 0) {
        uniqueEvents.insert(pair(date, event));
        storage[date].push_back(event);
    }
}

void Database::Print(ostream& os) const {
    for (const auto &[date, events]: storage) {
        for (const string& event: events) {
            os << date << ' ' << event << endl;
        }
    }
}

int Database::RemoveIf(function<bool(const Date&, const string&)> predicate) {
    int removedCnt = 0;
    set<Date> datesToDelete;
    // remove events
    for (auto &[date, events]: storage) {
        auto rmPredicate = [&, &date = date](const auto& event) {
            if (predicate(date, event)) {
                uniqueEvents.erase(pair(date, event));
                return true;
            }
            return false;
        };
        // partition all the events to be removed to the first part of the vector
        auto it = stable_partition(events.begin(), events.end(), rmPredicate);
        removedCnt += it - events.begin();
        events.erase(events.begin(), it);
        if (events.empty()) {
            datesToDelete.insert(date);
        }
    }
    // remove dates
    for (auto date: datesToDelete) {
        storage.erase(date);
    }
    return removedCnt;
}

vector<string> Database::FindIf(function<bool(const Date&, const string&)> predicate) const {
    vector<string> result;
    for (const auto &[date, events]: storage) {
        for (const string& event: events) {
            if (predicate(date, event)) {
                ostringstream os;
                os << date << " " << event;
                result.push_back(os.str());
            }
        }
    }
    return result;
}

string Database::Last(const Date& date) const {
    if (storage.empty() || date < begin(storage)->first) {
        throw invalid_argument("");
    }
    auto it = storage.upper_bound(date);
    --it;
    ostringstream os;
    os << it->first << " " << it->second.back();
    return os.str();
}
