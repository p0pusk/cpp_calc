
include_directories(../headers/)
set(
    SOURCES
    main.cpp
    operations.cpp
    parser.cpp
    pluginManager.cpp
    calculator.cpp
    )

set(
    HEADERS
    ../headers/operations.h
    ../headers/parser.h
    ../headers/pluginManager.h
    ../headers/calculator.h
    )

add_executable(calc ${SOURCES} ${HEADERS})

set_property(TARGET calc PROPERTY CXX_STANDARD 20)
