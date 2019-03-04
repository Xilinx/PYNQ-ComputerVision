# Find OpenCV, you may need to set OpenCV_DIR variable
if (NOT OpenCV_FOUND)
	find_package(OpenCV REQUIRED)
endif (NOT OpenCV_FOUND)

# OpenCV Utils
SET (libraryName makeMapForRemap)
add_library(${libraryName} src/makeMapForRemap.cpp inc/makeMapForRemap.h)

target_include_directories (${libraryName} PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/inc
	${OpenCV_INCLUDE_DIRS}
	)

