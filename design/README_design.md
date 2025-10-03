# Design do Projeto Agenda (Etapa 1)

## Estrutura de diretórios
- `include/`: cabeçalhos das principais classes.
- `uml/`: diagrama de classes (PlantUML).
- `CMakeLists.txt`: configuração inicial do projeto.
- `relatorio_etapa1.md`: relatório curto justificando as decisões de design.

## Classes principais
- **Time**: representa horário (hora e minuto).
- **Event**: evento da agenda com título, data, horário, tags e regra de recorrência.
- **RecurrenceRule**: classe abstrata para definir periodicidade de eventos.
- **Calendar**: responsável por gerenciar os eventos (adicionar, remover, buscar).
- **User**: usuário dono da agenda.
- **IPersistence**: interface para persistência.
- **JsonPersistence**: implementação planejada usando JSON.
- **CLI**: interface de linha de comando (mínimo para Etapa 2).

## Dependências
- C++17
- [PlantUML](https://plantuml.com/) (para gerar o UML)
- Futuramente: [nlohmann/json](https://github.com/nlohmann/json) para persistência.