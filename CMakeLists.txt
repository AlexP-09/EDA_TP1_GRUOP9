cmake_minimum_required(VERSION 3.20)
project(orbitalsim)
set(CMAKE_CXX_STANDARD 11)

# Configurar vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "/home/alex/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Sanitizers para Linux
if(UNIX)
    add_compile_options(-fsanitize=address -fsanitize=undefined)
    add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

# Especificar la ruta donde buscar los archivos de configuración
set(CMAKE_PREFIX_PATH "/home/alex/dev/vcpkg/installed/x64-linux")

# Buscar Raylib y GLFW usando vcpkg
find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# Ejecutable
add_executable(orbitalsim main.cpp orbitalSim.cpp view.cpp)

# Linkear con Raylib y GLFW
target_link_libraries(orbitalsim PRIVATE raylib glfw)