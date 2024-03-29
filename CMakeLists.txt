# Author: Lucas Vilas-Boas
# Year: 2023
# Repo: https://github.com/lucoiso/cpp-submodules

# ----------- Global Definitions -----------
CMAKE_MINIMUM_REQUIRED(VERSION 3.28)
PROJECT(cpp-submodules VERSION 0.0.1 LANGUAGES CXX)

IF (POLICY CMP0048)
    CMAKE_POLICY(SET CMP0048 NEW)
ENDIF ()

SET(CMAKE_CXX_STANDARD 23)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_CXX_SCAN_FOR_MODULES ON)

IF (UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -fmodules-ts")
ENDIF (UNIX)

SET(Boost_NO_WARN_NEW_VERSIONS ON)
SET(BUILD_TESTS $ENV{BUILD_TESTS})

# -------------- Directories ---------------
ADD_SUBDIRECTORY(Configuration)
ADD_SUBDIRECTORY(Timer)
ADD_SUBDIRECTORY(Coroutine)
ADD_SUBDIRECTORY(RuntimeInfo)
ADD_SUBDIRECTORY(SocketService)

IF (BUILD_TESTS)
    INCLUDE(CTest)
    ADD_SUBDIRECTORY(Tests)
ENDIF ()