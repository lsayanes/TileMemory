# TileMemory

Este juego usa la libreria SDL3

## Compilar en Mac 

1. Instalar SDL3 usando Homebrew

```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
### Instalar SDL3
```
brew install sdl3
```
Homebrew va a instalar SDL3 en /opt/homebrew/Cellar/sdl3/

### instalar SDL3_Image
```
brew install sdl3_image
```
SDL3_image también va a estar en /opt/homebrew/lib y /opt/homebrew/include/SDL3

### Compilar
Usar el Makefile 🤷🏻‍♂️


## Compilar en Windows 

### 1. Descargar SDL3
Página oficial:
👉 https://github.com/libsdl-org/SDL/releases

Buscar la última versión estable para Visual Studio (normalmente dice "SDL3-devel-xxx-VC.zip").

### 2. Descomprimir SDL3

Configurar en Visual Studio

🔸 C/C++ → General → Additional Include Directories.

🔸 Linker → General → Additional Library Directories.

🔸 Linker → Input → Additional Dependencies.
   * Agregá SDL3.lib
     
🔸 Copiar DLL output.


## Windows ARM64 (mi caso) 
Si el ZIP no trae lib\arm64 hay que compilarlo:

🔸 Instalar CMake y Ninja
(CMake desde https://cmake.org/)

🔸 Abrir una consola de x64 Native Tools Command Prompt for VS2022 o ARM64 (como se tenga configurado Visual Studio)

🔸 Ir a donde se descomprimio SDL3:

```
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A ARM64
cmake --build . --config Release
```

Esto genera los .lib y .dll para ARM64


