# README - CLI (Etapa 2)

Como compilar e executar (linha de comando):

```bash
cd design
mkdir build 
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
./agenda_cli   # ou agenda_cli.exe no Windows
```

O executável `agenda_cli` apresenta um menu simples com:
1) Criar evento (pede título, data YYYY-MM-DD, início HH:MM, fim HH:MM)
2) Listar eventos
3) Sair

Observações:
- O projeto usa C++17 e smart pointers (`std::unique_ptr`/`std::shared_ptr`).
- Persistência JSON não está implementada nessa etapa (lança exceção se usada).
- Entrada simples; não valida data além do formato e horários básicos.