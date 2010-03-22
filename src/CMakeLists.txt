remake_find_package(libdc1394-2 CONFIG)

remake_include(${LIBDC1394_2_INCLUDE_DIRS})

remake_add_directories(lib)
remake_add_directories(bin COMPONENT utils)
