# README - GUI (Etapa Final)

Como compilar e executar a versão com interface gráfica:

```bash
cd design
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt/6.9.3/mingw_64/lib/cmake" ..
cmake --build .
./agenda_gui   # ou agenda_gui.exe no Windows