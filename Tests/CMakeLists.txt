# Copyright Notices: [...]

# ------------ Module Settings -------------
include(CTest)

# -------------- Directories ---------------
if (BUILD_TESTING)
    add_subdirectory(Benchmark)
    add_subdirectory(Unit)
endif()
