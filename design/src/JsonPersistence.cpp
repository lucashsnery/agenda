#include "JsonPersistence.h"
#include <stdexcept>

namespace agenda {

void JsonPersistence::save(const Calendar& cal, const std::string& path) {
    (void)cal; (void)path;
    throw std::runtime_error("JsonPersistence::save not implemented yet");
}

std::unique_ptr<Calendar> JsonPersistence::load(const std::string& path) {
    (void)path;
    throw std::runtime_error("JsonPersistence::load not implemented yet");
}

} // namespace agenda