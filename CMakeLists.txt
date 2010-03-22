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
  HOME "http://aslforge.ethz.ch/projects/libfirecam"
  LICENSE "GNU Lesser General Public License (LGPL)"
  PREFIX firecam-
  INSTALL /usr
)

remake_doc(
  man INSTALL share
  html
)

remake_pack_deb()
remake_pack_deb(COMPONENT utils libfirecam)
remake_pack_deb(COMPONENT dev)
remake_pack_deb(ARCH all COMPONENT doc)
