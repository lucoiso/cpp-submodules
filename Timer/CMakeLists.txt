# Author: Lucas Vilas-Boas
# Year: 2023
# Repo: https://github.com/lucoiso/cpp-submodules

# ----------- Global Definitions -----------
set(LIBRARY_NAME Timer)

# ------------- Library Setup --------------
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Object.cpp
)

set(INCLUDE_FILES
    # Public Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/${LIBRARY_NAME}/Manager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/${LIBRARY_NAME}/Parameters.h
    
    # Private Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Object.h
)

add_library(${LIBRARY_NAME} SHARED ${SOURCE_FILES} ${INCLUDE_FILES})

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/${LIBRARY_NAME})
target_compile_definitions(${LIBRARY_NAME} PRIVATE BUILD_DLL)