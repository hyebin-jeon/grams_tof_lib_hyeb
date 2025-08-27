# This function makes a target that includes all the non-excluded targets in a directory
# make_dir_target(DIR_PATH my_dir TARGET_NAME my_target) will make a target called my_target that includes all the non-excluded targets in my_dir
function(hlx_make_target_of_directory)
    set(flags)
    set(args DIR_PATH TARGET_NAME)
    set(listArgs)
    cmake_parse_arguments(argPrefix "${flags}" "${args}" "${listArgs}" ${ARGN})

    set(dir ${argPrefix_DIR_PATH})
    set(target_name ${argPrefix_TARGET_NAME})

    #message(STATUS "Adding Target ${target_name} for path ${dir}")

    set(all_targets)
    get_all_targets_recursive(all_targets ${dir})

    add_custom_target(${target_name})
    foreach (target ${all_targets})
        add_dependencies(${target_name} ${target})
    endforeach()
    #message(STATUS "${target_name} ==> ${all_targets}")
endfunction()

# Recursively get all Non-excluded targets in a directory.  USes current dir if not passed
function(get_all_targets var dir)
    set(targets)
    if(EXISTS ${dir})
        get_all_targets_recursive(targets ${dir})
    else()
        get_all_targets_recursive(targets ${CMAKE_CURRENT_SOURCE_DIR})
    endif()
    set(${var} ${targets} PARENT_SCOPE)
endfunction()

macro(get_all_targets_recursive targets dir)
    get_property(subdirectories DIRECTORY ${dir} PROPERTY SUBDIRECTORIES)
    foreach(subdir ${subdirectories})
        get_property(is_excluded DIRECTORY "${subdir}" PROPERTY EXCLUDE_FROM_ALL)
        if("${is_excluded}" STREQUAL "")
            get_all_targets_recursive(${targets} ${subdir})
        endif()
        #get_all_targets_recursive(${targets} ${subdir})
    endforeach()

    get_property(current_targets DIRECTORY ${dir} PROPERTY BUILDSYSTEM_TARGETS)

    #include only the non-excluded ones.
    foreach (target ${current_targets})
        get_property(is_excluded TARGET "${target}" PROPERTY EXCLUDE_FROM_ALL)
        if("${is_excluded}" STREQUAL "")
            list(APPEND ${targets} ${target})
        endif()
    endforeach()
    #list(APPEND ${targets} ${current_targets})
endmacro()

