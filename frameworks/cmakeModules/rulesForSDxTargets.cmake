###############################################################################
#  Copyright (c) 2018, Xilinx, Inc.
#  All rights reserved.
# 
#  Redistribution and use in source and binary forms, with or without 
#  modification, are permitted provided that the following conditions are met:
#
#  1.  Redistributions of source code must retain the above copyright notice, 
#     this list of conditions and the following disclaimer.
#
#  2.  Redistributions in binary form must reproduce the above copyright 
#      notice, this list of conditions and the following disclaimer in the 
#      documentation and/or other materials provided with the distribution.
#
#  3.  Neither the name of the copyright holder nor the names of its 
#      contributors may be used to endorse or promote products derived from 
#      this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
#  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#  OR BUSINESS INTERRUPTION). HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
#  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
#  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
###############################################################################

#     Author: Kristof Denolf <kristof@xilinx.com>
#     Date:   2017/12/05

cmake_minimum_required(VERSION 2.8)

function(capFirstLetter smallName capName)
	string(SUBSTRING ${smallName} 0 1 FIRST_LETTER)
	string(TOUPPER ${FIRST_LETTER} FIRST_LETTER)
	string(REGEX REPLACE "^.(.*)" "${FIRST_LETTER}\\1" capNameLocal "${smallName}")
	set(${capName} ${capNameLocal} PARENT_SCOPE)
endfunction()

function (printPackageStatusOpenCV)
	message(STATUS "OpenCV library status:")
	message(STATUS "    version: ${OpenCV_VERSION}")
	message(STATUS "    libraries: ${OpenCV_LIBS}")
	message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
endfunction()

function (printPackageStatusVivadoHLS)
	message(STATUS "VivadoHLS status:")
	message(STATUS "    found: ${VivadoHLS_FOUND}")
	message(STATUS "    include path: ${VivadoHLS_INCLUDE_DIRS}")
endfunction()

macro (requireOpenCVAndVivadoHLS)
find_package(OpenCV REQUIRED) # OpenCV uses environment variable OpenCV_DIR
printPackageStatusOpenCV()
find_package(VivadoHLS REQUIRED) # HLS uses environment variable VIVADO_HLS
printPackageStatusVivadoHLS()
endmacro()

macro (requireOpenCV)
find_package(OpenCV REQUIRED) # OpenCV uses environment variable OpenCV_DIR
printPackageStatusOpenCV()
endmacro()

macro (setupForOpenCVUtilsAndHRTimerAndXFMat subdirLevels)
find_package(OpenCV REQUIRED) # OpenCV uses environment variable OpenCV_DIR
printPackageStatusOpenCV()
find_package(VivadoHLS QUIET) # HLS uses environment variable VIVADO_HLS
printPackageStatusVivadoHLS()

add_subdirectory(${PROJECT_SOURCE_DIR}/${subdirLevels}/frameworks/utilities/OpenCVUtils ${CMAKE_CURRENT_BINARY_DIR}/OpenCVUtils)
add_subdirectory(${PROJECT_SOURCE_DIR}/${subdirLevels}/frameworks/utilities/HRTimer ${CMAKE_CURRENT_BINARY_DIR}/HRTimer)
add_subdirectory(${PROJECT_SOURCE_DIR}/${subdirLevels}/frameworks/utilities/xF/Mat ${CMAKE_CURRENT_BINARY_DIR}/xFMat)

include_directories(
	${PROJECT_SOURCE_DIR}/${subdirLevels}/frameworks/utilities/OpenCVUtils/inc #redundant but needed to force order with VIVADO_HLS includes
	${PROJECT_SOURCE_DIR}/inc
	${OpenCV_INCLUDE_DIRS}
)

link_libraries(
	OpenCVUtils
	HRTimer
	xFMat
	${OpenCV_LIBS}
)
endmacro()

function (compilerDependentVivadoHLSInclude targetName)
if (${CMAKE_C_COMPILER_ID} STREQUAL "SDSCC")
	target_include_directories (${targetName} PUBLIC ${VivadoHLS_INCDIR_HLS})
else (${CMAKE_C_COMPILER_ID} STREQUAL "SDSCC") #native compilation
	target_include_directories (${targetName} PUBLIC ${VivadoHLS_INCLUDE_DIRS})
endif (${CMAKE_C_COMPILER_ID} STREQUAL "SDSCC")
endfunction ()

function (sdxTest componentName componentFolder subdirLevels)
	capFirstLetter(${componentName} componentNameCap)
	SET (currentTarget testSDSOC${componentNameCap})
	message(STATUS "ADDING SDSoC target ${currentTarget}")
	
	# set directives and configure compile flags
	add_definitions(-DHLS_NO_XIL_FPO_LIB) 
	if (${CMAKE_C_COMPILER_ID} STREQUAL "SDSCC")
		SET(CompileFlags "-sds-hw sdsoc${componentNameCap} SDSOC${componentNameCap}.cpp -clkid ${SDxClockID} -sds-end")
	else (${CMAKE_C_COMPILER_ID} STREQUAL "SDSCC") #native compilation
		SET(CompileFlags "-O3")
	endif (${CMAKE_C_COMPILER_ID} STREQUAL "SDSCC")
	
	add_executable(${currentTarget} src/testSDSOC${componentNameCap}.cpp 
		${PROJECT_SOURCE_DIR}/${subdirLevels}/${componentFolder}/${componentName}/SDSOCKernel/src/SDSOC${componentNameCap}.cpp)
	
	set_target_properties (${currentTarget} PROPERTIES COMPILE_FLAGS ${CompileFlags})	#FLAGS for compile only	
	
	target_include_directories (${currentTarget} PUBLIC
		${PROJECT_SOURCE_DIR}/${subdirLevels}/${componentFolder}/${componentName}/SDSOCKernel/inc
	)
	
	compilerDependentVivadoHLSInclude(${currentTarget})
	
endfunction()