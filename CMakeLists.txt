cmake_minimum_required(VERSION 3.20)

project(grams_tof_lib VERSION 1.0)

## Kim's
#Changes default install path to be a subdirectory of the build dir.
#Can set build dir at configure time with -DCMAKE_INSTALL_PREFIX=/install/path
if(CMAKE_INSTALL_PREFIX STREQUAL "" OR CMAKE_INSTALL_PREFIX STREQUAL
  "/usr/local")
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}")
elseif(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}")
endif()
###

include(GNUInstallDirs)
#include(ExternalProject)

# Add some global compiler options
add_compile_options (-fdiagnostics-color=always)

# Configure
set(CMAKE_BUILD_TYPE "RelWithDebInfo")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

##set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install")

# RPATH setting (macOS + Linux)
set(CMAKE_INSTALL_RPATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
#set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_MACOSX_RPATH ON)

## Check if the platform is Darwin (macOS) and add the definition only for that target
#if(APPLE)
#  message(STATUS "macOS is detected.")
#  add_definitions(-D_Darwin_)
#elseif(UNIX AND NOT APPLE)
#  execute_process(
#    COMMAND uname -r
#    OUTPUT_VARIABLE LINUX_KERNEL_VERSION
#    OUTPUT_STRIP_TRAILING_WHITESPACE
#  )
#  if(LINUX_KERNEL_VERSION VERSION_LESS "2.6")
#    message(STATUS "Old Linux Kernel is detected.")
#    add_definitions(-D_Old_Linux_Kernel_)
#  else()
#    message(STATUS "Modern Linux Kernel is detected.")
#    add_definitions(-D_Modern_Linux_Kernel_)
#  endif()
#else() 
#  message(STATUS "Unknown OS is detected.")
#endif()
#
#if(DEFINED ENV{ROOTSYS})
#  message("Found ROOTSYS at $ENV{ROOTSYS}")
#else()
#  message("ROOTSYS not found.  Searching in /cern.")
#  message("  => This will match the first ROOT install it finds, which might not be what you want!")
#  list(APPEND CMAKE_PREFIX_PATH /cern)     # enable searches in old-school cern paths
#endif()

find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})     
message(STATUS "ROOT_LIBRARIES = ${ROOT_LIBRARIES}")
message(STATUS "ROOT_INCLUDE_DIRS = ${ROOT_INCLUDE_DIRS}")

add_subdirectory(Analysis)
