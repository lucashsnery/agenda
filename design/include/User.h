#pragma once
#include <string>

namespace agenda {

class User {
public:
    User() = default;
    User(std::string username, std::string display_name);
    const std::string& username() const noexcept;
    const std::string& display_name() const noexcept;
    void set_display_name(const std::string&);

private:
    std::string _username;
    std::string _display_name;
};

} // namespace agenda
