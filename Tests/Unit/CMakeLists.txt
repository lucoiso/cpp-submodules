# Copyright Notices: [...]

# ----------- Global Definitions -----------
set(LIBRARY_NAME Unit)

# ------------- Library Setup --------------
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Unit.cpp
)

set(INCLUDE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/Unit/Configuration.h
)

add_executable(${LIBRARY_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/Unit)

find_package(Catch2 REQUIRED CONFIG COMPONENTS Catch2WithMain)

target_link_libraries(${LIBRARY_NAME} PUBLIC
    Catch2::Catch2WithMain
    Configuration
)
