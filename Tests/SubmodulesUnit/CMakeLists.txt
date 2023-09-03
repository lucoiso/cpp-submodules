# Author: Lucas Vilas-Boas
# Year: 2023
# Repo: https://github.com/lucoiso/cpp-submodules

# ----------- Global Definitions -----------
set(LIBRARY_NAME SubmodulesUnit)

# ------------- Library Setup --------------
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/SubmodulesUnit.cpp
)

set(INCLUDE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/${LIBRARY_NAME}/Configuration.h
)

add_executable(${LIBRARY_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/${LIBRARY_NAME})
add_test(NAME ${LIBRARY_NAME} COMMAND $<TARGET_FILE:${LIBRARY_NAME}>)

find_package(Boost REQUIRED CONFIG COMPONENTS test)

target_link_libraries(${LIBRARY_NAME} PRIVATE
    Boost::test
    Configuration
)