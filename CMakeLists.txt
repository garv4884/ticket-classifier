cmake_minimum_required(VERSION 3.16)
project(SupportTicketRouter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Everything except main.cpp goes into a library shared by the app and the tests.
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_library(ticketing_core STATIC ${CORE_SOURCES})
target_include_directories(ticketing_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ticketing_core PUBLIC -Wall -Wextra -Wpedantic)
endif()

add_executable(ticketing src/main.cpp)
target_link_libraries(ticketing PRIVATE ticketing_core)

add_executable(ticketing_tests tests/TestMain.cpp)
target_link_libraries(ticketing_tests PRIVATE ticketing_core)

# Executables land in <project>/bin so they can be run from the project root,
# where the default data/ paths resolve.
set_target_properties(ticketing ticketing_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if(MINGW)
    # Self-contained .exe files that do not need the MinGW runtime DLLs on PATH.
    target_link_options(ticketing PRIVATE -static)
    target_link_options(ticketing_tests PRIVATE -static)
endif()

enable_testing()
add_test(NAME unit_tests COMMAND ticketing_tests)
