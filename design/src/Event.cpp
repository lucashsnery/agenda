#include "Event.h"
#include <stdexcept>
#include <string>

using namespace std;

namespace agenda {

Event::Event(std::string id, std::string title, std::string date_iso, Time_x start, Time_x end)
    : _id(std::move(id)), _title(std::move(title)), _date(std::move(date_iso)), _start(start), _end(end) {}

Event::~Event() = default;

const std::string& Event::id() const noexcept { return _id; }
const std::string& Event::title() const noexcept { return _title; }
void Event::set_title(const std::string& t) { _title = t; }

const std::string& Event::date() const noexcept { return _date; }
void Event::set_date(const std::string& d) { _date = d; }

Time_x Event::start_time() const noexcept { return _start; }
Time_x Event::end_time() const noexcept { return _end; }
void Event::set_time(Time_x s, Time_x e) {
    _start = s; _end = e;
}
bool Event::ends_next_day() const {
    return _end < _start;
}

void Event::add_tag(const Tag& tag) { _tags.push_back(tag); }
const std::vector<Tag>& Event::tags() const noexcept { return _tags; }

void Event::set_recurrence(std::shared_ptr<RecurrenceRule> rule) { _recurrence = std::move(rule); }
std::shared_ptr<RecurrenceRule> Event::recurrence() const noexcept { return _recurrence; }

} // namespace agenda