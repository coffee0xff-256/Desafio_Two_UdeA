cmake_minimum_required(VERSION 3.16)

project(Desafio_Two_UdeA VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON) # ACTIVADO: Fundamental para procesar el .qrc

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.10)

# ÚNICO qt_add_executable con todos los archivos necesarios
qt_add_executable(appDesafio_Two_UdeA
    main.cpp
    bombos.h
    bombos.qrc  # Esto compila la carpeta Resources dentro del ejecutable
)

set_target_properties(appDesafio_Two_UdeA PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appDesafio_Two_UdeA
    PRIVATE Qt6::Quick
)

install(TARGETS appDesafio_Two_UdeA
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)