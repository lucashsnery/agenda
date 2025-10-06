#include "Calendar.h"
#include <algorithm>
#include <stdexcept>

namespace agenda {

Calendar::~Calendar() = default;

void Calendar::add_event(std::unique_ptr<Event> ev) {
    if (!ev) throw std::invalid_argument("null event");
    _events.push_back(std::move(ev));
}

std::vector<Event*> Calendar::events_on_date(const std::string& iso_date) const {
    std::vector<Event*> res;
    for (const auto& up : _events) {
        if (up->date() == iso_date) res.push_back(up.get());
    }
    return res;
}

std::vector<Event*> Calendar::all_events() const {
    std::vector<Event*> res;
    for (const auto& up : _events) res.push_back(up.get());
    return res;
}

std::vector<Event*> Calendar::find_by_tag(const std::string& tag) const {
    std::vector<Event*> res;
    for (const auto& up : _events) {
        for (const auto& t : up->tags()) {
            if (t == tag) { res.push_back(up.get()); break; }
        }
    }
    return res;
}

bool Calendar::remove_event(const std::string& id) {
    auto it = std::find_if(_events.begin(), _events.end(), [&](const std::unique_ptr<Event>& e){
        return e->id() == id;
    });
    if (it == _events.end()) return false;
    _events.erase(it);
    return true;
}

} // namespace agenda