#include "Calendar.h"
#include "RecurrenceRule.h"
#include "Time_x.h"
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <iomanip>

namespace agenda {

Calendar::Calendar(const User& user) : _owner(user) {}

Calendar::~Calendar() = default;

// ---------------- EVENTOS ----------------

void Calendar::add_event(std::unique_ptr<Event> ev) {
    if (!ev) throw std::invalid_argument("Evento nulo");
    _events.push_back(std::move(ev));
}

std::vector<Event*> Calendar::events_on_date(const std::string& iso_date) const {
    std::vector<Event*> res;
    for (const auto& up : _events)
        if (up->date() == iso_date) res.push_back(up.get());
    return res;
}

std::vector<Event*> Calendar::all_events() const {
    std::vector<Event*> res;
    for (const auto& up : _events) res.push_back(up.get());
    return res;
}

std::vector<Event*> Calendar::find_by_tag(const std::string& tag) const {
    std::vector<Event*> res;
    for (const auto& up : _events)
        for (const auto& t : up->tags())
            if (t == tag) { res.push_back(up.get()); break; }
    return res;
}

bool Calendar::remove_event(const std::string& id) {
    auto it = std::find_if(_events.begin(), _events.end(),
        [&](const std::unique_ptr<Event>& e) { return e->id() == id; });
    if (it == _events.end()) return false;
    _events.erase(it);
    return true;
}

void Calendar::expand_recurrent_events(int days_ahead) {
    std::vector<std::unique_ptr<Event>> new_events;

    for (auto& up : _events) {
        auto r = up->recurrence();
        if (!r) continue;

        std::tm tm{};
        std::istringstream ss(up->date());
        ss >> std::get_time(&tm, "%Y-%m-%d");
        if (ss.fail()) continue;

        for (int i = 1; i <= days_ahead; ++i) {
            std::tm next_tm = tm;
            switch (r->frequency()) {
                case Frequency::DAILY: next_tm.tm_mday += i * r->interval(); break;
                case Frequency::WEEKLY: next_tm.tm_mday += 7 * i * r->interval(); break;
                case Frequency::MONTHLY: next_tm.tm_mon += i * r->interval(); break;
                case Frequency::YEARLY: next_tm.tm_year += i * r->interval(); break;
            }
            std::mktime(&next_tm);
            char buf[11];
            std::strftime(buf, sizeof(buf), "%Y-%m-%d", &next_tm);

            auto ev = std::make_unique<Event>(
                up->id() + "_rec" + std::to_string(i),
                up->title(),
                buf,
                up->start_time(),
                up->end_time()
            );

            for (auto& t : up->tags()) ev->add_tag(t);
            ev->set_recurrence(r);
            new_events.push_back(std::move(ev));
        }
    }

    for (auto& ne : new_events)
        add_event(std::move(ne));
}

// ---------------- USUÁRIOS ----------------

void Calendar::add_user(std::unique_ptr<User> user) {
    if (!user) throw std::invalid_argument("Usuário nulo");
    if (find_user(user->username()))
        throw std::runtime_error("Usuário já existente: " + user->username());
    _users.push_back(std::move(user));
}

bool Calendar::remove_user(const std::string& username) {
    auto it = std::find_if(_users.begin(), _users.end(),
        [&](const std::unique_ptr<User>& u){ return u->username() == username; });
    if (it == _users.end()) return false;
    _users.erase(it);
    return true;
}

std::vector<User*> Calendar::all_users() const {
    std::vector<User*> res;
    for (const auto& u : _users) res.push_back(u.get());
    return res;
}

User* Calendar::find_user(const std::string& username) const {
    for (const auto& u : _users)
        if (u->username() == username) return u.get();
    return nullptr;
}

// ---------------- OWNER ----------------

const User& Calendar::owner() const noexcept { return _owner; }
void Calendar::set_owner(const User& user) { _owner = user; }

} // namespace agenda