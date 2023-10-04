# Author: Lucas Vilas-Boas
# Year: 2023
# Repo: https://github.com/lucoiso/cpp-submodules

# ----------- Global Definitions -----------
SET(LIBRARY_NAME Timer)

# ------------- Library Setup --------------
SET(PRIVATE_MODULES
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/TimerModule.h

	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/Manager.cppm
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/Object.cppm

	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/Object.ixx
)

SET(PUBLIC_MODULES
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Public/Manager.ixx
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Public/Parameters.ixx
)

ADD_LIBRARY(${LIBRARY_NAME} SHARED ${PRIVATE_MODULES} ${PUBLIC_MODULES})
SET_TARGET_PROPERTIES(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)

TARGET_INCLUDE_DIRECTORIES(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/)
TARGET_COMPILE_DEFINITIONS(${LIBRARY_NAME} PRIVATE BUILD_DLL)
