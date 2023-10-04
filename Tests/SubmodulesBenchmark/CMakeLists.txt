# Author: Lucas Vilas-Boas
# Year: 2023
# Repo: https://github.com/lucoiso/cpp-submodules

# ----------- Global Definitions -----------
SET(LIBRARY_NAME SubmodulesBenchmark)

# ------------- Library Setup --------------
SET(PRIVATE_MODULES
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/SubmodulesBenchmark.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/Configuration.ixx
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/Timer.ixx
)

ADD_EXECUTABLE(${LIBRARY_NAME} ${PRIVATE_MODULES})
SET_TARGET_PROPERTIES(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)

FIND_PACKAGE(benchmark CONFIG REQUIRED)

TARGET_LINK_LIBRARIES(${LIBRARY_NAME} PRIVATE
					  benchmark::benchmark_main
					  Configuration
					  Timer
)
