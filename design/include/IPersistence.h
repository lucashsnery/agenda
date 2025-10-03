#pragma once
#include <string>
#include <memory>

namespace agenda {

class Calendar;

class IPersistence {
public:
    virtual ~IPersistence() = default;
    virtual void save(const Calendar& cal, const std::string& path) = 0;
    virtual std::unique_ptr<Calendar> load(const std::string& path) = 0;
};

} // namespace agenda
