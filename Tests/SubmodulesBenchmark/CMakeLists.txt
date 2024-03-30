# Author: Lucas Vilas-Boas
# Year: 2024
# Repo: https://github.com/lucoiso/cpp-submodules

# ----------- Global Definitions -----------
SET(LIBRARY_NAME SubmodulesBenchmark)

# ------------- Library Setup --------------
SET(PRIVATE_MODULES_BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Source/Private)

SET(PRIVATE_MODULES
        ${PRIVATE_MODULES_BASE_DIRECTORY}/SubmodulesBenchmark.cpp
        ${PRIVATE_MODULES_BASE_DIRECTORY}/Configuration.hpp
        ${PRIVATE_MODULES_BASE_DIRECTORY}/Timer.hpp
)

ADD_EXECUTABLE(${LIBRARY_NAME} ${PRIVATE_MODULES})
SET_TARGET_PROPERTIES(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)

TARGET_INCLUDE_DIRECTORIES(${LIBRARY_NAME} PRIVATE ${PRIVATE_MODULES_BASE_DIRECTORY})

FIND_PACKAGE(benchmark CONFIG REQUIRED)

TARGET_LINK_LIBRARIES(${LIBRARY_NAME} PRIVATE
        benchmark::benchmark_main
        Configuration
        Timer
)
