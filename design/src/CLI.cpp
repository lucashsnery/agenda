#include "CLI.h"
#include "Calendar.h"
#include "Event.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <iomanip>

namespace agenda {

// simples gerador de ids
static int g_id_counter = 1;
static std::string next_id() {
    return "evt-" + std::to_string(g_id_counter++);
}

static bool parse_time(const std::string& s, Time_x& t) {
    std::istringstream ss(s);
    int h,m; char c;
    if (!(ss >> h >> c >> m)) return false;
    if (c != ':') return false;
    if (h < 0 || h > 23 || m < 0 || m > 59) return false;
    t.hour = h; t.minute = m; return true;
}

void CLI::run_demo() {
    Calendar cal;
    std::cout << "Agenda CLI - Teste rapido (criar/listar)\n";

    while (true) {
        std::cout << "\nEscolha: 1) Criar evento  2) Listar eventos  3) Sair\n> ";
        std::string line;
        if (!std::getline(std::cin, line)) break;
        if (line == "1") {
            std::string title, date, start_s, end_s;
            std::cout << "Titulo: "; std::getline(std::cin, title);
            std::cout << "Data (YYYY-MM-DD): "; std::getline(std::cin, date);
            std::cout << "Inicio (HH:MM): "; std::getline(std::cin, start_s);
            std::cout << "Fim (HH:MM): "; std::getline(std::cin, end_s);
            Time_x s,e;
            if (!parse_time(start_s,s) || !parse_time(end_s,e)) {
                std::cout << "Horario invalido\n"; continue;
            }
            try {
                auto ev = std::make_unique<Event>(next_id(), title, date, s, e);
                cal.add_event(std::move(ev));
                std::cout << "Evento criado com sucesso.\n";
            } catch (const std::exception& ex) {
                std::cout << "Erro criando evento: " << ex.what() << "\n";
            }
        } else if (line == "2") {
            auto all = cal.all_events();
            if (all.empty()) { std::cout << "Sem eventos.\n"; continue; }
            std::cout << "Eventos:\n";
            for (auto e : all) {
                std::cout << "- [" << e->id() << "] " << e->date() << " "
                        << std::setw(2) << std::setfill('0') << e->start_time().hour << ":"
                        << std::setw(2) << std::setfill('0') << e->start_time().minute << "-"
                        << std::setw(2) << std::setfill('0') << e->end_time().hour << ":"
                        << std::setw(2) << std::setfill('0') << e->end_time().minute
                        << (e->ends_next_day() ? " (dia seguinte)" : "")
                        << " : " << e->title() << "\n";
            }
        } else if (line == "3") {
            break;
        } else {
            std::cout << "Opcao invalida\n";
        }
    }

    std::cout << "Saindo CLI.\n";
}

} // namespace agenda