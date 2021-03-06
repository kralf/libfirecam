cmake_minimum_required(VERSION 2.6)

include(ReMake)

remake_project(
  libfirecam
  VERSION 0.1
  RELEASE alpha
  SUMMARY "A C++-based FireWire camera interfacing library."
  AUTHOR "Ralf Kaestner"
  AUTHOR "Luciano Spinello"
  CONTACT "ralf.kaestner@gmail.com"
  HOME "http://github.com/kralf/libfirecam"
  LICENSE "GNU Lesser General Public License (LGPL)"
  PREFIX firecam-
  INSTALL /usr
)

remake_doc(
  html
)

remake_pack_deb(
  DEPENDS libdc1394-22
)
remake_pack_deb(
  COMPONENT utils
  DESCRIPTION "utilities"
  DEPENDS libfirecam libpng12-0
)
remake_pack_deb(
  COMPONENT dev
  DESCRIPTION "development headers"
  DEPENDS libfirecam libdc1394-22-dev
)
remake_pack_deb(
  ARCH all
  COMPONENT doc
  DESCRIPTION "documentation"
)

remake_distribute_deb(
  DISTRIBUTION lucid
  SECTION libs
  UPLOAD ppa:kralf/asl
  DEPENDS remake libpng12-dev libdc1394-22-dev pkg-config doxygen
  PASS CMAKE_BUILD_TYPE LIBFIRECAM_GIT_REVISION
)
remake_distribute_deb(
  DISTRIBUTION precise
  SECTION libs
  UPLOAD ppa:kralf/asl
  DEPENDS remake libpng12-dev libdc1394-22-dev pkg-config doxygen
  PASS CMAKE_BUILD_TYPE LIBFIRECAM_GIT_REVISION
)
remake_distribute_deb(
  DISTRIBUTION trusty
  SECTION libs
  UPLOAD ppa:kralf/asl
  DEPENDS remake libpng12-dev libdc1394-22-dev pkg-config doxygen
  PASS CMAKE_BUILD_TYPE LIBFIRECAM_GIT_REVISION
)
