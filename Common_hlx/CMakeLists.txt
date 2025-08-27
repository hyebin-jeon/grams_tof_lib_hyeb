project(HELIX_Flight_Software_Common)
message("${BMagenta}[============ Entering Common Subdirectory =============]${ColReset}")
########################################################################################################################
# List all our classes.  Stuff in DICT_CLASSES will get dictionaries made.  Stuff in NORM_CLASSES won't
# The source files have to be named *.cpp and live in src/, the headers in include/
########################################################################################################################
# These are classes that are used by both flight and ground daq ops, but aren't of general use beyond that.
set(NORM_CLASSES)

# These are daq-related classes that need dictionaries.
set(DICT_CLASSES HLX_ObjectFile HLX_RunHeader)

# dictionaries for header-only classes
set(HDR_ONLY_DICT_CLASSES)
set(HDR_ONLY_DICT_FILES)

# These are lighter-weight standalone classes that we can bundle with other libs, like loggers, for any code.
# Note the different naming convention here - full filenames.  We don't use the inferring scheme used below.
set(UTIL_SOURCES
        src/HLX_Stopwatch.cpp
        src/HLX_Logger.cpp
        include/HLX_TimeStamp.h
        include/HLX_HexDumper.h
        include/HLX_CLI.h
        include/HLX_Getch.h
        include/HLX_Util.h
        include/HLX_TOML_Table.h
        include/HLX_StatTools.h
        include/HLX_AttribManager.h)

# Infer the source/header names & paths from the class names for use in ROOT dict generation.
# this fails if you have a filename with ; in it.  But, if you do that, you are stupid
# this makes 4 new cmake-lists (;-delimited)
string(REGEX REPLACE "([^;]+)" "src/\\1.cpp" DICT_SOURCES "${DICT_CLASSES}")  #quote here interprets last arg as list
string(REGEX REPLACE "([^;]+)" "\\1.h"       DICT_HEADERS "${DICT_CLASSES}")  #quote here interprets last arg as list
string(REGEX REPLACE "([^;]+)" "src/\\1.cpp" NORM_SOURCES "${NORM_CLASSES}")  #quote here interprets last arg as list
string(REGEX REPLACE "([^;]+)" "\\1.h"       NORM_HEADERS "${NORM_CLASSES}")  #quote here interprets last arg as list
########################################################################################################################

########################################################################################################################
# INCLUDE_DIRECTORIES - ROOT generator needs this to be set
########################################################################################################################
include_directories(include)

########################################################################################################################
# CLI11 - command-line parser (header only)
# https://github.com/CLIUtils/CLI11
########################################################################################################################
message("${BYellow}-- Adding CLI11 --${ColReset}")
add_library(cli11 INTERFACE)
target_include_directories(cli11 INTERFACE extern/CLI11-2.2.0)
# due to limitations in ROOT_GENERATE_DICTIONARY, this line is needed if any classes in the ROOT dict require this lib
include_directories(extern/CLI11-2.2.0)

########################################################################################################################
# fmt - modern print formatting lib
# https://github.com/fmtlib/fmt
# If your project is in Debug mode, it will make a libfmtd.a
########################################################################################################################
message("${BYellow}-- Adding libfmt --${ColReset}")
add_subdirectory(extern/fmt-7.1.3)
target_compile_definitions(fmt PUBLIC FMT_EXCEPTIONS=0)
set_target_properties(fmt PROPERTIES POSITION_INDEPENDENT_CODE ON)
# due to limitations in ROOT_GENERATE_DICTIONARY, this line is needed if any classes in the ROOT dict require this lib
include_directories(extern/fmt-7.1.3/include)

########################################################################################################################
# spdlog - logging lib
# https://github.com/gabime/spdlog
########################################################################################################################
message("${BYellow}-- Adding libspdlog --${ColReset}")
option(SPDLOG_FMT_EXTERNAL "" ON)     #if you turn this on, you need to link to fmt above
add_subdirectory(extern/spdlog-1.8.5)
set_target_properties(spdlog PROPERTIES POSITION_INDEPENDENT_CODE ON)
# due to limitations in ROOT_GENERATE_DICTIONARY, this line is needed if any classes in the ROOT dict require this lib
include_directories(extern/spdlog-1.8.5/include)

########################################################################################################################
# magic_enum - fancy enumeration magic
# https://github.com/Neargye/magic_enum
# Needs to be added as a public target include below
########################################################################################################################
message("${BYellow}-- Adding magic_enum --${ColReset}")
add_subdirectory(extern/magic_enum-0.7.2)
## due to limitations in ROOT_GENERATE_DICTIONARY, this line is needed if any classes in the ROOT dict require this lib
include_directories(extern/magic_enum-0.7.2/include)

########################################################################################################################
# semver - fancy semver testing
# https://github.com/Neargye/semver
# Needs to be added as a public target include below
########################################################################################################################
message("${BYellow}-- Adding semver --${ColReset}")
add_subdirectory(extern/semver-0.3.0)
## due to limitations in ROOT_GENERATE_DICTIONARY, this line is needed if any classes in the ROOT dict require this lib
include_directories(extern/semver-0.3.0)

########################################################################################################################
# toml++ - config file lib
# https://github.com/marzer/tomlplusplus
########################################################################################################################
message("${BYellow}-- Adding toml++ --${ColReset}")
add_subdirectory(extern/toml++-3.3.0)
## due to limitations in ROOT_GENERATE_DICTIONARY, this line is needed if any classes in the ROOT dict require this lib
include_directories(extern/toml++-3.3.0)

########################################################################################################################
## Locate ROOT.  Since 6.16, much better cmake support
## The defined lib targets are ROOT::Core, ROOT::Tree, etc.
########################################################################################################################
find_package(ROOT REQUIRED COMPONENTS Physics Core)
#include(${ROOT_USE_FILE})  #include if you need to capture ROOT compile options

# import our own dictionary helper which makes the linkdefs
include(cmake/root_tools.cmake)

########################################################################################################################
# Build a lighter-weight utility lib.  No ROOT dependencies.
########################################################################################################################
add_library(HelixCommonUtilLib SHARED ${UTIL_SOURCES})
target_include_directories(HelixCommonUtilLib PUBLIC include) # Needed to have other targets get auto-header paths
target_link_libraries(HelixCommonUtilLib fmt)
target_link_libraries(HelixCommonUtilLib cli11)
target_link_libraries(HelixCommonUtilLib spdlog)
target_link_libraries(HelixCommonUtilLib magic_enum)
target_link_libraries(HelixCommonUtilLib semver)
target_link_libraries(HelixCommonUtilLib toml++)
target_link_libraries(HelixCommonUtilLib ${ROOT_LIBRARIES})

set(UTIL_DICT_HDR include/HLX_Stopwatch.h include/HLX_Logger.h include/HLX_HexDumper.h include/HLX_CLI.h include/HLX_TOML_Table.h include/HLX_TimeStamp.h)
set(UTIL_DICT_CLASSES HLX_Stopwatch  HLX_HexDumper HLX_TOML_Table)
set(UTIL_DICT_CLASSES_NV HLX_Logger HLX_CLI HLX_TimeStamp)
helix_root_dictionary_builder(
        DICT_NAME         HelixCommonUtilDictGenerated
        DEST_TARGET       HelixCommonUtilLib
        CLASS_LIST        ${UTIL_DICT_CLASSES}
        HEADER_LIST       ${UTIL_DICT_HDR}
        CLASS_LIST_NO_IO  ${UTIL_DICT_CLASSES_NV})

########################################################################################################################
# Set up the main Common lib and all its dependencies
# Do this properly and all the header and lib paths will be set magically
########################################################################################################################
add_library(HelixCommonLib SHARED ${NORM_SOURCES} ${DICT_SOURCES})
target_include_directories(HelixCommonLib PRIVATE src)  # can do this to skip constructing source paths
target_include_directories(HelixCommonLib PUBLIC include) # Needed to have other targets get auto-header paths
target_link_libraries(HelixCommonLib PUBLIC HelixCommonUtilLib)
target_link_libraries(HelixCommonLib PUBLIC HelixDataStage0Lib)
target_link_libraries(HelixCommonLib PUBLIC HelixDataStage1Lib)
target_link_libraries(HelixCommonLib PUBLIC ${ROOT_LIBRARIES})

# Build a single dictionary containing all the root bits, using our own wrappers.  Connect it to the Common lib
helix_root_dictionary_builder(
        DICT_NAME   HelixCommonDictGenerated
        DEST_TARGET HelixCommonLib
        CLASS_LIST  ${DICT_CLASSES} ${HDR_ONLY_DICT_CLASSES}
        HEADER_LIST ${DICT_HEADERS} ${HDR_ONLY_DICT_FILES}
)

########################################################################################################################
# Any other targets or tests below
########################################################################################################################
# Turn off by default to avoid cluttering target space
#add_subdirectory(test EXCLUDE_FROM_ALL)
