#!/bin/bash

# Detener la ejecución si ocurre cualquier error
set -e

# Eliminar los directorios 'build' y 'xcode' si existen
echo "Eliminando los directorios existentes..."
[ -d build ] && rm -rf build
[ -d xcode ] && rm -rf xcode
[ -d bin ] && rm -rf bin
echo "Directorios eliminados."

# Ejecutar los comandos de cmake
echo "Ejecutando cmake para configurar el proyecto..."
cmake -S . -B build -DCMAKE_CXX_STANDARD=17
echo "Compilación del proyecto con cmake..."
cmake --build build -j$(sysctl -n hw.logicalcpu)

# Crear y configurar el proyecto Xcode
echo "Configurando el proyecto Xcode..."
mkdir xcode
cd xcode
cmake -G Xcode ..
