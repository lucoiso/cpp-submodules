# Author: Lucas Vilas-Boas
# Year: 2023
# Repo: https://github.com/lucoiso/cpp-submodules

# ----------- Global Definitions -----------
SET(LIBRARY_NAME SubmodulesBenchmark)

# ------------- Library Setup --------------
SET(PRIVATE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/SubmodulesBenchmark.cpp
)

SET(PRIVATE_MODULES
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/Configuration.ixx
)

ADD_EXECUTABLE(${LIBRARY_NAME} ${PRIVATE_SOURCES})

TARGET_SOURCES(${LIBRARY_NAME}
			   PRIVATE
			   FILE_SET cxx_private_modules_file_set
			   TYPE CXX_MODULES
			   FILES ${PRIVATE_MODULES}
)

TARGET_INCLUDE_DIRECTORIES(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Modules/Public/)
TARGET_INCLUDE_DIRECTORIES(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/)

SET_TARGET_PROPERTIES(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)

FIND_PACKAGE(benchmark CONFIG REQUIRED)

TARGET_LINK_LIBRARIES(${LIBRARY_NAME} PRIVATE
					  benchmark::benchmark_main
					  Configuration
)
