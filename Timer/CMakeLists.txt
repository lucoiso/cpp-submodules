# Author: Lucas Vilas-Boas
# Year: 2023
# Repo: https://github.com/lucoiso/cpp-submodules

# ----------- Global Definitions -----------
set(LIBRARY_NAME Timer)

# ------------- Library Setup --------------
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/TimerManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/TimerObject.cpp
)

set(INCLUDE_FILES
    # Public Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/${LIBRARY_NAME}/TimerManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/${LIBRARY_NAME}/TimerParameters.h
    
    # Private Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/TimerObject.h
)

add_library(${LIBRARY_NAME} SHARED ${SOURCE_FILES} ${INCLUDE_FILES})

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/${LIBRARY_NAME})
target_compile_definitions(${LIBRARY_NAME} PRIVATE BUILD_DLL)