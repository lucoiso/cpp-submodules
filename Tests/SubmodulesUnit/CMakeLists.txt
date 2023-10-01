# Author: Lucas Vilas-Boas
# Year: 2023
# Repo: https://github.com/lucoiso/cpp-submodules

# ----------- Global Definitions -----------
SET(LIBRARY_NAME SubmodulesUnit)

# ------------- Library Setup --------------
SET(SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Source/SubmodulesUnit.cpp
)

SET(INCLUDE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Include/${LIBRARY_NAME}/Configuration.h
)

ADD_EXECUTABLE(${LIBRARY_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})
TARGET_INCLUDE_DIRECTORIES(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/${LIBRARY_NAME})
ADD_TEST(NAME ${LIBRARY_NAME} COMMAND $<TARGET_FILE:${LIBRARY_NAME}>)

FIND_PACKAGE(Boost REQUIRED COMPONENTS system)

TARGET_LINK_LIBRARIES(${LIBRARY_NAME} PRIVATE
					  Boost::system
					  Configuration
)