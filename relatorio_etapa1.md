# Relatório Etapa 1 – Projeto Agenda (POO)

## Objetivo
Este projeto implementa uma aplicação de Agenda, permitindo cadastrar e consultar eventos com horário, data, tags e regras de recorrência, além de persistência em arquivo.

## Decisões de Design

### Abstração e Encapsulamento
Cada classe possui atributos privados com acesso via getters e setters. Exemplo: `Event` encapsula título, data, horário e tags.

### Composição vs. Herança
- **Composição:** `Calendar` possui (`std::unique_ptr<Event>`) seus eventos.
- **Herança/Polimorfismo:** `IPersistence` (interface de persistência) e `RecurrenceRule` (regra de recorrência abstrata).

### Smart Pointers e RAII
- `std::unique_ptr<Event>` garante propriedade exclusiva no `Calendar`.
- `std::shared_ptr<RecurrenceRule>` permite que múltiplos eventos compartilhem a mesma regra de recorrência.
- RAII assegura liberação automática de memória.

### STL e Templates
Uso de `std::vector`, `std::string`, `std::optional`.  
Prevemos funções templates auxiliares para serialização e busca genérica.

### Tratamento de Exceções
Persistência (`JsonPersistence`) lançará exceções em caso de erro de I/O.  
Essas exceções serão capturadas e tratadas na camada de interface.

## Arquitetura
- **Domínio:** `Event`, `Calendar`, `User`, `RecurrenceRule`.
- **Persistência:** `IPersistence` + `JsonPersistence`.
- **Interface:** `CLI` (mínima, para testes).

## Conclusão
A arquitetura proposta é modular, extensível e atende aos requisitos de POO: encapsulamento, herança, polimorfismo, composição, uso de STL e smart pointers. 