#pragma once
#include <string>
#include <ostream>

namespace agenda {

struct Time {
    int hour{0};
    int minute{0};
    Time() = default;
    Time(int h, int m): hour(h), minute(m) {}
    std::string to_string() const;
    bool operator<(Time const& o) const noexcept;
    bool operator==(Time const& o) const noexcept;
};

} // namespace agenda
