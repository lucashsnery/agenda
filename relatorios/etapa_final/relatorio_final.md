# Agenda POO — Projeto Final

Projeto de POO com interface gráfica em Qt6, implementando uma agenda de eventos com recorrência, tags e persistência em JSON.

## Demonstração

Assista à demonstração da aplicação clicando no link abaixo:  
[Link para o vídeo](https://imgur.com/ZP7l7Dk)

## Mapa de Requisitos POO → Implementação

| Requisito | Onde e como foi usado |
| --- | --- |
| **1. Abstração & Encapsulamento** | - Separação `.h`/`.cpp` em todas as classes (`Event`, `Calendar`, `GUI`, `Time_x`, `User`)  <br> - Campos privados: `_start`, `_end`, `_tags`, `_recurrence` em `Event`; `_events`, `_owner` em `Calendar`  <br> - Getters/Setters com validação (`Event::set_time()` lança exceção para horário inválido) |
| **2. Classes e Objetos** | - Modelo-Domínio: `Event`, `Time_x`, `User`  <br> - Controller/Interface: `Calendar`, `GUI`  <br> - Uso de instâncias: `GUI` instancia `Calendar` e widgets Qt; `Calendar` armazena `Event` via `std::unique_ptr` |
| **3. Herança & Polimorfismo** | - `GUI` herda de `QMainWindow`; `CreateEventDialog` herda de `QDialog`  <br> - Polimorfismo de Qt: slots/sinais (`connect`) usando ponteiros de função  <br> - `IPersistence` é interface abstrata (métodos virtuais puros `save/load`) e `JsonPersistence` implementa concretamente |
| **4. Composição vs Herança** | - Composição: `Calendar` possui múltiplos `Event`  <br> - Composição: `Event` possui `Time_x` para início/fim e `RecurrenceRule`  <br> - Composição: `GUI` possui `Calendar` e widgets Qt, não herda deles |
| **5. Polimorfismo dinâmico** | - `m_persistence` em `GUI` é `std::unique_ptr<IPersistence>` (polimórfico)  <br> - Permite trocar implementação sem alterar GUI |
| **6. Gerenciamento de recursos** | - RAII com smart pointers: `std::unique_ptr<Event>` em `Calendar`  <br> - `std::unique_ptr<IPersistence>` em `GUI`  <br> - Qt gerencia widgets via parent-child (não precisa deletar manualmente) |
| **7. Templates e STL** | - Containers STL: `std::vector<Event>` em `Calendar`; `std::vector<Tag>` em `Event`  <br> - `std::string` e `std::ostringstream` para formatação  <br> - Algoritmos STL: `std::sort` usado em `GUI::load_events_table()` |
| **8. Sobrecarga de operadores** | - `Time_x::operator<` usado para comparar horários (ordenar e `ends_next_day()`) |
| **9. Tratamento de exceções** | - `Event::set_time()` lança `std::invalid_argument`  <br> - `try/catch` em `GUI::on_create_event()` e `GUI::on_save/on_load()` com mensagens via `QMessageBox` |
| **10. Documentação técnica e UML** | - Diagrama de classes (PlantUML abaixo)  <br> - Diagrama de sequência opcional: caso de uso “Criar evento” |
| **11. Build automatizado** | - `CMakeLists.txt` configurado  <br> - Qt6 Widgets encontrado, automoc/autouic/rcc ativados  <br> - Build reproduzível no Windows; instruções abaixo |

---