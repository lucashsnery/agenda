#pragma once
#include <string>

namespace agenda {

class Time_x {
public:
    int hour;
    int minute;

    std::string str() const;
    bool operator<(Time_x const& o) const noexcept;
    bool operator==(Time_x const& o) const noexcept;
};

} // namespace agenda