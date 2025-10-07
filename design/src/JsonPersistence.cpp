#include "JsonPersistence.h"
#include "Calendar.h"
#include "Event.h"
#include "Time_x.h"
#include "RecurrenceRule.h"
#include "User.h"
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

namespace agenda {

// ---------------- Funções auxiliares ----------------

// Event → JSON
static json event_to_json(const Event& e) {
    json j;
    j["id"] = e.id();
    j["title"] = e.title();
    j["date"] = e.date();
    j["start_hour"] = e.start_time().hour;
    j["start_minute"] = e.start_time().minute;
    j["end_hour"] = e.end_time().hour;
    j["end_minute"] = e.end_time().minute;
    j["tags"] = e.tags();

    if (e.recurrence()) {
        j["recurrence_frequency"] = static_cast<int>(e.recurrence()->frequency());
        j["recurrence_interval"] = e.recurrence()->interval();
    } else {
        j["recurrence_frequency"] = nullptr;
        j["recurrence_interval"] = 0;
    }

    return j;
}

// JSON → Event
static std::unique_ptr<Event> json_to_event(const json& j) {
    Time_x start(j["start_hour"], j["start_minute"]);
    Time_x end(j["end_hour"], j["end_minute"]);
    auto ev = std::make_unique<Event>(
        j["id"].get<std::string>(),
        j["title"].get<std::string>(),
        j["date"].get<std::string>(),
        start,
        end
    );

    for (const auto& t : j["tags"]) ev->add_tag(t.get<std::string>());

    if (!j["recurrence_frequency"].is_null() && j["recurrence_interval"].get<int>() > 0) {
        auto rule = std::make_shared<RecurrenceRule>(
            static_cast<Frequency>(j["recurrence_frequency"].get<int>()),
            j["recurrence_interval"].get<int>()
        );
        ev->set_recurrence(rule);
    }

    return ev;
}

// User → JSON
static json user_to_json(const User& u) {
    json j;
    j["id"] = u.id();
    j["username"] = u.username();
    j["display_name"] = u.display_name();
    return j;
}

// JSON → User
static std::unique_ptr<User> json_to_user(const json& j) {
    return std::make_unique<User>(
        j["id"].get<std::string>(),
        j["username"].get<std::string>(),
        j["display_name"].get<std::string>()
    );
}

// ---------------- Métodos principais ----------------

void JsonPersistence::save(const Calendar& cal, const std::string& path) {
    json j;

    // Salvar eventos
    auto all = cal.all_events();
    for (const auto& e : all) {
        j["events"].push_back(event_to_json(*e));
    }

    // Salvar usuários (se existirem)
    if (!cal.all_users().empty()) {
        for (const auto& u : cal.all_users()) {
            j["users"].push_back(user_to_json(*u));
        }
    }

    std::ofstream ofs(path);
    if (!ofs.is_open()) throw std::runtime_error("Não foi possível abrir arquivo para salvar: " + path);
    ofs << j.dump(4);
}

std::unique_ptr<Calendar> JsonPersistence::load(const std::string& path) {
    std::ifstream ifs(path);
    if (!ifs.is_open()) throw std::runtime_error("Não foi possível abrir arquivo para ler: " + path);

    json j;
    ifs >> j;

    auto cal = std::make_unique<Calendar>();

    // Carregar eventos
    if (j.contains("events")) {
        for (const auto& je : j["events"]) {
            cal->add_event(json_to_event(je));
        }
    }

    // Carregar usuários
    if (j.contains("users")) {
        for (const auto& ju : j["users"]) {
            cal->add_user(json_to_user(ju));
        }
    }

    return cal;
}

} // namespace agenda