# Author: Lucas Vilas-Boas
# Year: 2023
# Repo: https://github.com/lucoiso/cpp-submodules

# -------------- Directories ---------------
ADD_SUBDIRECTORY(Configuration)
ADD_SUBDIRECTORY(Timer)

IF (BUILD_TESTING)
	ADD_SUBDIRECTORY(Tests)
ENDIF ()