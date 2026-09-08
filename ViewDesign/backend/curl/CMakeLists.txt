find_package(CURL QUIET)

if (NOT CURL_FOUND)
    return()
endif()

file(GLOB_RECURSE LIB_SOURCES *.h *.cpp)
target_sources(ViewDesign PRIVATE ${LIB_SOURCES})
