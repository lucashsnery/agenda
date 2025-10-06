#include "RecurrenceRule.h"

namespace agenda {

RecurrenceRule::RecurrenceRule(Frequency freq, int interval)
    : _freq(freq), _interval(interval) {}

Frequency RecurrenceRule::frequency() const noexcept { return _freq; }
int RecurrenceRule::interval() const noexcept { return _interval; }

} // namespace agenda