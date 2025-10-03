#pragma once
#include <vector>
#include <memory>
#include <optional>
#include <string>
#include "Event.h"

namespace agenda {

class Calendar {
public:
    Calendar() = default;
    ~Calendar();

    void add_event(std::unique_ptr<Event> ev);
    std::vector<Event*> events_on_date(const std::string& iso_date) const;
    std::vector<Event*> all_events() const;
    std::vector<Event*> find_by_tag(const std::string& tag) const;
    bool remove_event(const std::string& id);

private:
    std::vector<std::unique_ptr<Event>> _events;
};

} // namespace agenda