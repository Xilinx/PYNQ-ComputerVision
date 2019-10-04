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

# cmake needs this line
cmake_minimum_required(VERSION 2.8)

# Define library name
SET (libraryName xFMat)
SET (SDS_SHARED_LIB "libsds_lib.so")

# Find OpenCV, you may need to set OpenCV_DIR variable
find_package(OpenCV REQUIRED)

add_library(${libraryName} src/mat.cpp inc/mat.hpp)

#compilerDependentVivadoHLSInclude(${libraryName})
set_target_properties (${libraryName} PROPERTIES COMPILE_FLAGS "-O3 -fPIC")

target_include_directories (${libraryName} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${OpenCV_INCLUDE_DIRS}
	)

#lines 54-64 are outdated? Need to test to confirm
find_path(SDS_SHARED_LIBPATH ${SDS_SHARED_LIB} PATHS ${CMAKE_LIBRARY_PATH})

if (${SDS_SHARED_LIBPATH} STREQUAL "SDS_SHARED_LIBPATH-NOTFOUND")
	message(STATUS "WARNING: ${SDS_SHARED_LIB} not found") 
else (${SDS_SHARED_LIBPATH} STREQUAL "SDS_SHARED_LIBPATH-NOTFOUND")
	target_link_libraries (${libraryName} 
		${SDS_SHARED_LIBPATH}/${SDS_SHARED_LIB}
	)
	message(STATUS "${SDS_SHARED_LIB} found. Adding as link library.") 
endif (${SDS_SHARED_LIBPATH} STREQUAL "SDS_SHARED_LIBPATH-NOTFOUND")
message(STATUS "SDS_SHARED_LIBPATH: ${SDS_SHARED_LIBPATH}")

