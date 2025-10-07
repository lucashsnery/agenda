#include "User.h"
#include <stdexcept>

namespace agenda {

User::User(std::string id, std::string username, std::string display_name)
    : _id(std::move(id)),
      _username(std::move(username)),
      _display_name(std::move(display_name)) {
    if (_id.empty()) throw std::invalid_argument("ID do usuário não pode ser vazio");
    if (_username.empty()) throw std::invalid_argument("Nome de usuário não pode ser vazio");
    if (_display_name.empty()) throw std::invalid_argument("Nome de exibição não pode ser vazio");
}

const std::string& User::id() const noexcept { return _id; }
const std::string& User::username() const noexcept { return _username; }
const std::string& User::display_name() const noexcept { return _display_name; }

void User::set_display_name(const std::string& name) {
    if (name.empty()) throw std::invalid_argument("Nome de exibição vazio");
    _display_name = name;
}

void User::set_username(const std::string& uname) {
    if (uname.empty()) throw std::invalid_argument("Nome de usuário vazio");
    _username = uname;
}

void User::set_id(const std::string& new_id) {
    if (new_id.empty()) throw std::invalid_argument("ID do usuário vazio");
    _id = new_id;
}

} // namespace agenda