# Author: Lucas Vilas-Boas
# Year: 2023
# Repo: https://github.com/lucoiso/cpp-submodules

# ----------- Global Definitions -----------
SET(LIBRARY_NAME Timer)

# ------------- Library Setup --------------
SET(PRIVATE_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/TimerModule.h
)

SET(PRIVATE_MODULES
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/Manager.mxx
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/Object.mxx
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/Object.ixx
)

SET(PUBLIC_MODULES
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Public/Manager.ixx
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Public/Parameters.ixx
)

ADD_LIBRARY(${LIBRARY_NAME} SHARED)

TARGET_SOURCES(${LIBRARY_NAME}
			   PRIVATE
			   FILE_SET cxx_private_headers_file_set
			   TYPE HEADERS
			   FILES ${PRIVATE_HEADERS}
)

TARGET_SOURCES(${LIBRARY_NAME}
			   PRIVATE
			   FILE_SET cxx_private_modules_file_set
			   TYPE CXX_MODULES
			   FILES ${PRIVATE_MODULES}
)

TARGET_SOURCES(${LIBRARY_NAME}
			   PUBLIC
			   FILE_SET cxx_public_modules_file_set
			   TYPE CXX_MODULES
			   FILES ${PUBLIC_MODULES}
)

TARGET_INCLUDE_DIRECTORIES(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Modules/Public/)
TARGET_INCLUDE_DIRECTORIES(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/)

SET_TARGET_PROPERTIES(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)

TARGET_COMPILE_DEFINITIONS(${LIBRARY_NAME} PRIVATE BUILD_DLL)