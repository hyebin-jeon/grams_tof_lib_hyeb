project(HELIX_Flight_Software_Common_Test)
message("${BMagenta}[============ Entering Common/Test Subdirectory =============]${ColReset}")

# Test executables for the common library
add_executable(test_toml.exe EXCLUDE_FROM_ALL test_toml.cpp)
target_link_libraries(test_toml.exe HelixCommonUtilLib)

add_executable(test_common.exe EXCLUDE_FROM_ALL test_common.cpp)
target_link_libraries(test_common.exe HelixCommonLib)

add_executable(test_enums.exe EXCLUDE_FROM_ALL test_magic_enum.cpp)
target_link_libraries(test_enums.exe HelixCommonUtilLib)

add_executable(test_cli.exe EXCLUDE_FROM_ALL test_cli.cpp)
target_link_libraries(test_cli.exe HelixCommonUtilLib)

find_package(Threads REQUIRED)
add_executable(test_log.exe EXCLUDE_FROM_ALL test_log.cpp)
target_link_libraries(test_log.exe HelixCommonUtilLib)
target_link_libraries(test_log.exe Threads::Threads)

add_executable(test_disk_writer.exe EXCLUDE_FROM_ALL test_disk_writer.cpp)
target_link_libraries(test_disk_writer.exe HelixCommonLib)

add_executable(test_stopwatch.exe EXCLUDE_FROM_ALL test_stopwatch.cpp)
target_link_libraries(test_stopwatch.exe HelixCommonLib)

add_executable(test_header.exe EXCLUDE_FROM_ALL test_header.cpp)
target_link_libraries(test_header.exe HelixCommonLib)
add_dependencies(test_header.exe TarSourceTree)

add_executable(test_json.exe EXCLUDE_FROM_ALL test_json.cpp)
target_link_libraries(test_json.exe HelixCommonLib)
target_link_libraries(test_json.exe HelixDataHSKLib)
