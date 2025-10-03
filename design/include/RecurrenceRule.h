#pragma once
#include <string>
#include <memory>
#include <optional>

namespace agenda {

enum class Frequency { DAILY, WEEKLY, MONTHLY, YEARLY };

class RecurrenceRule {
public:
    RecurrenceRule() = default;
    RecurrenceRule(Frequency freq, int interval = 1);
    virtual ~RecurrenceRule() = default;

    Frequency frequency() const noexcept;
    int interval() const noexcept;

    virtual bool applies_to(const std::string& iso_date) const = 0;

protected:
    Frequency _freq{Frequency::DAILY};
    int _interval{1};
};

} // namespace agenda
