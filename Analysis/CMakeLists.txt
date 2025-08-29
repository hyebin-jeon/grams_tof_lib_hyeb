project(GRAMS_TOF_Analysis)

message("${BMagenta}[============ Entering Analysis Subdirectory ============]${ColReset}")

set(GRAMS_TOF_ANALYSIS_FILES
	TOF_ChannelConversion.cpp
	TOF_TdcQdcCalibration.cpp
	TOF_TreeData.cpp
	TOF_Attributes.cpp
	TOF_Fitting.cpp
)

set(GRAMS_TOF_ANALYSIS_CLASSES
	TOF_ChannelConversion
	TOF_TdcQdcCalibration
	TOF_TreeData
	TOF_Struct
	TOF_Attributes
	TOF_Fitting
)

set(GRAMS_TOF_ANALYSIS_HEADERS
	TOF_ChannelConversion.h
	TOF_TdcQdcCalibration.h
	TOF_TreeData.h
	TOF_Constants.h
	TOF_Struct.h
	TOF_Attributes.h
	TOF_Fitting.h
)

# ROOT dictionary
root_generate_dictionary( 
	GramsTofAnalysisDict 
	${GRAMS_TOF_ANALYSIS_HEADERS} 
	LINKDEF TOF_Linkdef.h 
	MODULE GramsTofAnalysisLib
)

## the generated dictionary files
set(GRAMS_TOF_DICT
    ${CMAKE_CURRENT_BINARY_DIR}/GramsTofAnalysisDict.cxx
)

## Build a shared lib
## Add the generated dictionary *.cxx files to lib
add_library(GramsTofAnalysisLib SHARED
    ${GRAMS_TOF_ANALYSIS_FILES}
    ${GRAMS_TOF_ANALYSIS_HEADERS}  ### diff
    ${GRAMS_TOF_DICT}
)


target_include_directories(GramsTofAnalysisLib PUBLIC . ${ROOT_INCLUDE_DIRS})
target_link_libraries(GramsTofAnalysisLib PUBLIC ${ROOT_LIBRARIES})

# 빌드 순서 보장: dictionary 먼저 생성
#add_dependencies(GramsTofAnalysisLib GramsTofAnalysisDict)


# 라이브러리 설치
install(TARGETS GramsTofAnalysisLib
        EXPORT GramsTofAnalysisLibTargets ### diff
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
				PUBLIC_HEADER DESTINATION include
)

# header files are copied to include
install(FILES
		${GRAMS_TOF_ANALYSIS_HEADERS}
    DESTINATION include
		)

