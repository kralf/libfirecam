remake_find_package(libpng CONFIG)

remake_include(
  ../lib
  ${LIBPNG_INCLUDE_DIRS}
)
remake_add_executables(LINK firecam ${LIBPNG_LIBRARIES})
