#include <ctime>   
#include "Time_x.h"
#include <iomanip>
#include <sstream>

using namespace std; 

namespace agenda {

std::string Time_x::str() const {
    std::ostringstream ss;
    ss << std::setw(2) << std::setfill('0') << hour << ":"
       << std::setw(2) << std::setfill('0') << minute;
    return ss.str();
}

bool Time_x::operator<(Time_x const& o) const noexcept {
    if (hour != o.hour) return hour < o.hour;
    return minute < o.minute;
}

bool Time_x::operator==(Time_x const& o) const noexcept {
    return hour == o.hour && minute == o.minute;
}

} // namespace agenda