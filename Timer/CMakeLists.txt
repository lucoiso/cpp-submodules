# Author: Lucas Vilas-Boas
# Year: 2023
# Repo: https://github.com/lucoiso/cpp-submodules

# ----------- Global Definitions -----------
SET(LIBRARY_NAME Timer)

# ------------- Library Setup --------------
SET(SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Manager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Object.cpp
)

SET(INCLUDE_FILES
	# Public Headers
	${CMAKE_CURRENT_SOURCE_DIR}/Include/${LIBRARY_NAME}/Manager.h
	${CMAKE_CURRENT_SOURCE_DIR}/Include/${LIBRARY_NAME}/Parameters.h

	# Private Headers
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Object.h
)

ADD_LIBRARY(${LIBRARY_NAME} SHARED ${SOURCE_FILES} ${INCLUDE_FILES})

TARGET_INCLUDE_DIRECTORIES(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
TARGET_INCLUDE_DIRECTORIES(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/${LIBRARY_NAME})
TARGET_COMPILE_DEFINITIONS(${LIBRARY_NAME} PRIVATE BUILD_DLL)