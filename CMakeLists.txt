# Author: Lucas Vilas-Boas
# Year: 2023
# Repo: https://github.com/lucoiso/cpp-submodules

# ----------- Global Definitions -----------
PROJECT(cpp-submodules VERSION 0.1.0 LANGUAGES CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 3.28)

SET(BUILD_TESTS $ENV{BUILD_TESTS})

# -------------- Directories ---------------
ADD_SUBDIRECTORY(Configuration)
ADD_SUBDIRECTORY(Timer)
ADD_SUBDIRECTORY(Coroutine)

IF (BUILD_TESTS)
	ADD_SUBDIRECTORY(Tests)
ENDIF ()