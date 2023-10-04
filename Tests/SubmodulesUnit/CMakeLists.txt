# Author: Lucas Vilas-Boas
# Year: 2023
# Repo: https://github.com/lucoiso/cpp-submodules

# ----------- Global Definitions -----------
SET(LIBRARY_NAME SubmodulesUnit)

# ------------- Library Setup --------------
SET(PRIVATE_MODULES
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/SubmodulesUnit.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/Configuration.ixx
)

ADD_EXECUTABLE(${LIBRARY_NAME} ${PRIVATE_MODULES})
SET_TARGET_PROPERTIES(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)

TARGET_INCLUDE_DIRECTORIES(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Modules/Public/)
TARGET_INCLUDE_DIRECTORIES(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Modules/Private/)

ADD_TEST(NAME ${LIBRARY_NAME} COMMAND $<TARGET_FILE:${LIBRARY_NAME}>)

FIND_PACKAGE(Boost REQUIRED COMPONENTS system)

TARGET_LINK_LIBRARIES(${LIBRARY_NAME} PRIVATE
					  Boost::system
					  Configuration
)