#pragma once
#include <string>

namespace agenda {

enum class Frequency { DAILY, WEEKLY, MONTHLY, YEARLY };

class RecurrenceRule {
public:
    RecurrenceRule() = default;
    RecurrenceRule(Frequency freq, int interval = 1);
    ~RecurrenceRule() = default;

    Frequency frequency() const noexcept;
    int interval() const noexcept;

    bool applies_to(const std::string& iso_date) const;

private:
    Frequency _freq{Frequency::DAILY};
    int _interval{1};
};

} // namespace agenda