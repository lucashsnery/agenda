#pragma once
#include <string>

namespace agenda {

class User {
public:
    User() = default;
    User(std::string id, std::string username, std::string display_name);

    // Getters
    const std::string& id() const noexcept;
    const std::string& username() const noexcept;
    const std::string& display_name() const noexcept;

    // Setters
    void set_display_name(const std::string&);
    void set_username(const std::string&);
    void set_id(const std::string&);

private:
    std::string _id;             // identificador único do usuário
    std::string _username;       // login
    std::string _display_name;   // nome exibido
};

} // namespace agenda