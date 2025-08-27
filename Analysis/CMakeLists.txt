project(GRAMS_TOF_Analysis)
message("${BMagenta}[============ Entering Analysis Subdirectory ============]${ColReset}")

set(GRAMS_TOF_ANALYSIS_FILES
	TOF_ChannelConversion.cpp
	TOF_TdcQdcCalibration.cpp
	TOF_TreeData.cpp
	TOF_Attributes.cpp
)

set(GRAMS_TOF_ANALYSIS_CLASSES
	TOF_ChannelConversion
	TOF_TdcQdcCalibration
	TOF_TreeData
	TOF_Struct
	TOF_Attributes
)

set(GRAMS_TOF_ANALYSIS_HEADERS
	TOF_ChannelConversion.h
	TOF_TdcQdcCalibration.h
	TOF_TreeData.h
	TOF_Constants.h
	TOF_Struct.h
	TOF_Attributes.h
)

# ROOT dictionary
root_generate_dictionary( 
	GramsTofAnalysisDict 
	${GRAMS_TOF_ANALYSIS_HEADERS} 
	LINKDEF TOF_Linkdef.h 
)

## the generated dictionary files
set(GRAMS_TOF_DICT
    ${CMAKE_CURRENT_BINARY_DIR}/GramsTofAnalysisDict.cxx
)

## Add the generated dictionary *.cxx files to lib
add_library(GramsTofAnalysisLib SHARED
    ${GRAMS_TOF_ANALYSIS_FILES}
    ${GRAMS_TOF_ANALYSIS_HEADERS}
    ${GRAMS_TOF_DICT}
#${CMAKE_CURRENT_BINARY_DIR}/GramsTofAnalysisDict.cxx
)


target_include_directories(GramsTofAnalysisLib PUBLIC . ${ROOT_INCLUDE_DIRS})
target_link_libraries(GramsTofAnalysisLib PUBLIC ${ROOT_LIBRARIES})

# 빌드 순서 보장: dictionary 먼저 생성
add_dependencies(GramsTofAnalysisLib GramsTofAnalysisDict)



########################################################################################################################
# Make a lib for all the stuff
#add_library(HelixAnalysisCalibLib SHARED ${HXA_CALIB_FILES})
#target_include_directories(HelixAnalysisCalibLib PUBLIC .)
#target_link_libraries(GramsTofAnalysisLib PUBLIC HelixCommonUtilLib)
#target_link_libraries(GramsTofAnalysisLib PUBLIC ${ROOT_LIBRARIES})

# Preferred way to make dictionary
#helix_root_dictionary_builder(
#        DICT_NAME   GramsTofAnalysisDictGenerated
#        DEST_TARGET GramsTofAnalysisLib
#        CLASS_LIST  ${GRAMS_TOF_ANALYSIS_CLASSES}
#        HEADER_LIST ${GRAMS_TOF_ANALYSIS_HEADERS}
#        DEPS        HelixCommonUtilLib
#       #DEPS        HelixGeometryLib HelixCommonUtilLib
#)

#add_dependencies(HelixAnalysisCalibLib copy-cal-files)
#add_custom_target(copy-cal-files)
#add_custom_target(clobber-cal-files) # only for calling by hand
#add_custom_command(TARGET copy-cal-files    POST_BUILD COMMAND bash -c "cp -rn ${CMAKE_CURRENT_SOURCE_DIR}/calibration_files ${HLX_CONFIG_DIR}" COMMENT "=> Cal files Copied!")
#add_custom_command(TARGET clobber-cal-files POST_BUILD COMMAND bash -c "cp -r  ${CMAKE_CURRENT_SOURCE_DIR}/calibration_files ${HLX_CONFIG_DIR}" COMMENT "=> Cal files Clobbered!")

########################################################################################################################

