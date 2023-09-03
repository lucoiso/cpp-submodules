# Author: Lucas Vilas-Boas
# Year: 2023
# Repo: https://github.com/lucoiso/cpp-submodules

# -------------- Directories ---------------
add_subdirectory(Configuration)

if (BUILD_TESTING)
    add_subdirectory(Tests)
endif()