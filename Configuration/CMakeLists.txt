# Author: Lucas Vilas-Boas
# Year: 2023
# Repo: https://github.com/lucoiso/cpp-submodules

# ----------- Global Definitions -----------
set(LIBRARY_NAME Configuration)

# ------------- Library Setup --------------
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Manager.cpp
)

set(INCLUDE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/${LIBRARY_NAME}/Manager.h
)

add_library(${LIBRARY_NAME} SHARED ${SOURCE_FILES} ${INCLUDE_FILES})

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/${LIBRARY_NAME})
target_compile_definitions(${LIBRARY_NAME} PRIVATE BUILD_DLL)

find_package(Boost CONFIG REQUIRED COMPONENTS json)
target_link_libraries(${LIBRARY_NAME} PUBLIC Boost::json)