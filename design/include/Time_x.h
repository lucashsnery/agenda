#pragma once
#include <string>

namespace agenda {

class Time_x {
public:
    int hour = 0;
    int minute = 0;

    Time_x() = default;
    Time_x(int h, int m) : hour(h), minute(m) {}

    std::string str() const;
    bool operator<(Time_x const& o) const noexcept;
    bool operator==(Time_x const& o) const noexcept;
};

} // namespace agenda