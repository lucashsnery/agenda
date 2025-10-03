#pragma once
#include <string>
#include <memory>
#include <vector>
#include <optional>
#include "Time.h"

namespace agenda {

using Tag = std::string;

class RecurrenceRule; // forward

class Event {
public:
    Event() = default;
    Event(std::string id, std::string title, std::string date_iso, Time start, Time end);
    ~Event();

    const std::string& id() const noexcept;
    const std::string& title() const noexcept;
    void set_title(const std::string& t);

    const std::string& date() const noexcept;
    void set_date(const std::string& d);

    Time start_time() const noexcept;
    Time end_time() const noexcept;
    void set_time(Time s, Time e);

    void add_tag(const Tag& tag);
    const std::vector<Tag>& tags() const noexcept;

    void set_recurrence(std::shared_ptr<RecurrenceRule> rule);
    std::shared_ptr<RecurrenceRule> recurrence() const noexcept;

private:
    std::string _id;
    std::string _title;
    std::string _date;
    Time _start;
    Time _end;
    std::vector<Tag> _tags;
    std::shared_ptr<RecurrenceRule> _recurrence;
};

} // namespace agenda
