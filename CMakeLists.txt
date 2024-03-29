# Version minimal de Cmake
cmake_minimum_required(VERSION 3.3)


# Nom du projet
project(APR)

# On indique où se trouvent les fichiers d'en-tête
include_directories(include)

# On construit la liste des fichiers sources à partir du contenu de src/
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

# On fixe un type de compilation par défaut
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()
# On indique les options de compilations
set(CMAKE_CXX_FLAGS "-std=c++11")
# Options de compilation pour le type Release
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
# Options de compilation pour le type Debug
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message("   Compiler CXX:         ${CMAKE_CXX_COMPILER}")
message("   Compiler CXX flags:         ${CMAKE_CXX_FLAGS}")
if(CMAKE_BUILD_TYPE MATCHES "DEBUG")
message("   Compiler CXX debug flags:   ${CMAKE_CXX_FLAGS_DEBUG}")
endif(CMAKE_BUILD_TYPE MATCHES "DEBUG")
if(CMAKE_BUILD_TYPE MATCHES "RELEASE")
message("   Compiler CXX release flags: ${CMAKE_CXX_FLAGS_RELEASE}")
endif(CMAKE_BUILD_TYPE MATCHES "RELEASE")

# On crée la bibliothèque libpolynom.a
add_library(APR ${SOURCE_FILES})

# On génère l'exécutable poly.e à partir du fichier src/main.cpp
add_executable(run.e src/main.cpp)

# On lie l'exécutable avec la bibliothèque poly.a
target_link_libraries(run.e APR)

