#!/bin/bash

# Detener la ejecución si ocurre cualquier error
set -e

# Eliminar los directorios 'build' y 'xcode' si existen
echo "Eliminando los directorios existentes..."
[ -d build ] && rm -rf build
[ -d xcode ] && rm -rf xcode
[ -d bin ] && rm -rf bin
echo "Directorios eliminados."