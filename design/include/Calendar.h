#pragma once
#include <vector>
#include <memory>
#include <optional>
#include <string>
#include "Event.h"
#include "User.h"

namespace agenda {

class Calendar {
public:
    Calendar() = default;
    explicit Calendar(const User& user);
    ~Calendar();

    // --- Movable, mas não copiável ---
    Calendar(Calendar&&) = default;
    Calendar& operator=(Calendar&&) = default;

    Calendar(const Calendar&) = delete;
    Calendar& operator=(const Calendar&) = delete;

    // --- Eventos ---
    void add_event(std::unique_ptr<Event> ev);
    std::vector<Event*> events_on_date(const std::string& iso_date) const;
    std::vector<Event*> all_events() const;
    std::vector<Event*> find_by_tag(const std::string& tag) const;
    bool remove_event(const std::string& id);
    void expand_recurrent_events(int days_ahead);

    // --- Usuários ---
    void add_user(std::unique_ptr<User> user);
    bool remove_user(const std::string& username);
    std::vector<User*> all_users() const;
    User* find_user(const std::string& username) const;

    // --- Proprietário (user principal) ---
    const User& owner() const noexcept;
    void set_owner(const User& user);

private:
    User _owner;
    std::vector<std::unique_ptr<Event>> _events;
    std::vector<std::unique_ptr<User>> _users;
};

} // namespace agenda