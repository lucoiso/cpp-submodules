# Author: Lucas Vilas-Boas
# Year: 2023
# Repo: https://github.com/lucoiso/cpp-submodules

# -------------- Directories ---------------
ADD_SUBDIRECTORY(Configuration)
ADD_SUBDIRECTORY(Timer)
ADD_SUBDIRECTORY(Coroutine)

IF (BUILD_TESTS)
	ADD_SUBDIRECTORY(Tests)
ENDIF ()