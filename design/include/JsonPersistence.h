#pragma once
#include "IPersistence.h"

namespace agenda {

class JsonPersistence : public IPersistence {
public:
    JsonPersistence() = default;
    ~JsonPersistence() override = default;

    void save(const Calendar& cal, const std::string& path) override;
    std::unique_ptr<Calendar> load(const std::string& path) override;
};

} // namespace agenda
