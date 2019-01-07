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
#     Date:   2018/07/23

# cmake .. -DCMAKE_TOOLCHAIN_FILE=toolchain_sdx2018.2.cmake
#  -DSDxPlatform="<absolute path to platform or predefined sdx platform name>"
#  -DSDxClockID="core clock ID number"
#  -DSDxDMClockID="data motion network clock ID (optional, if not specified, set to SDxClockID)"
#  -DSDxArch="arm32 or arm64"
#  -DSDxSysroot="absolute path to the sysroot folder"
#  Options (set ON or OFF):
#    . usePL: enable PL offloading (default ON)
#    . noBitstream: do not generate PL bitstream (default OFF)
#    . noSDCardImage: do not generate SD card image (default OFF)

cmake_policy(SET CMP0012 NEW)

set(SDxPlatform "zcu102" CACHE STRING "SDx platform")
set(SDxClockID "0" CACHE STRING "SDx clock ID")
set(SDxDMClockID ${SDxClockID} CACHE STRING "SDx clock ID")
set(SDxArch "arm64" CACHE STRING "SDx aarch")

message(STATUS "SDx platform: ${SDxPlatform} , clock ID: ${SDxClockID}, aarch: ${SDxArch} bit")

#Option to enable moving blocks to PL, enabled by default
OPTION(usePL "enable PL offloading" ON) # enabled by default
OPTION(noBitstream "do not generate PL bitstream" OFF)
OPTION(noSDCardImage "do not generate SD card image" OFF)
message(STATUS "Selected Options: usePL=${usePL}, noBitstream: ${noBitstream}, noSDCardImage=${noSDCardImage}")

#set general sdx compiler and linker flags
UNSET(CMAKE_CXX_FLAGS)
IF (${noBitstream})
set (noBitstreamCXXFlag -mno-bitstream)
ENDIF (${noBitstream})

IF (${noSDCardImage})
set (noSDCardImageFlag -mno-boot-files)
ENDIF (${noSDCardImage})

set(CMAKE_CXX_FLAGS "-sds-pf ${SDxPlatform} -dmclkid ${SDxDMClockID} ${noBitstreamCXXFlag} ${noSDCardImageFlag}" CACHE STRING "" FORCE)

# workaround for SDx: for *.{cpp,c} to *.o instead of *.{c,cpp}.o
set(CMAKE_C_OUTPUT_EXTENSION_REPLACE 1)
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

#preparation for XSDK path
execute_process(COMMAND which xsdk OUTPUT_VARIABLE XSDK)
get_filename_component(XSDK_PARENT ${XSDK} PATH)
get_filename_component(XSDK_PARENT ${XSDK_PARENT} PATH)

#preparation for sds++ Compiler path
execute_process(COMMAND which sds++ OUTPUT_VARIABLE SDSOC)
get_filename_component(SDSOC_PARENT ${SDSOC} PATH)

# give the system information
SET (CMAKE_SYSTEM_NAME Linux)

# specify the sdsoc cross compiler
INCLUDE(CMakeForceCompiler)
CMAKE_FORCE_C_COMPILER(${SDSOC_PARENT}/sdscc SDSCC)
CMAKE_FORCE_CXX_COMPILER(${SDSOC_PARENT}/sds++ SDSCXX)

if (WIN32)
  SET(SDxHostSystemName "nt")
else (WIN32)
  SET(SDxHostSystemName "lin")
endif (WIN32)

#SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -mstrict-align -hls-target 1")
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mstrict-align -hls-target 1")

if (${SDxArch} STREQUAL "arm64") # 64 bit toolchain
  #SET (CMAKE_FIND_ROOT_PATH ${XSDK_PARENT}/gnu/aarch64/${SDxHostSystemName}/aarch64-linux/aarch64-linux-gnu)
  SET (CMAKE_SYSTEM_PROCESSOR aarch64)
  SET (gnuPrefix1 aarch64-linux)
  SET (gnuPrefix2 aarch64-linux-gnu)
  SET (gnuArch aarch64)

  #extra compilation flags
  #NONE
else (${SDxArch} STREQUAL "arm64") #32 bit toolchain
  #SET (CMAKE_FIND_ROOT_PATH ${XSDK_PARENT}/gnu/aarch32/${SDxHostSystemName}/gcc-arm-linux-gnueabi/arm-linux-gnueabihf)
  SET (CMAKE_SYSTEM_PROCESSOR arm)
  SET (gnuPrefix1 gcc-arm-linux-gnueabi)
  SET (gnuPrefix2 arm-linux-gnueabihf)
  SET (gnuArch aarch32)
  
  #extra compilation flags
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__ARM_PCS_VFP")
endif (${SDxArch} STREQUAL "arm64")

UNSET(CMAKE_C_FLAGS)
SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "" FORCE)
MESSAGE(STATUS "CMAKE C FLAGS ${CMAKE_C_FLAGS}")

SET (CMAKE_FIND_ROOT_PATH ${XSDK_PARENT}/gnu/${gnuArch}/${SDxHostSystemName}/${gnuPrefix1}/${gnuPrefix2})


#find sysroot first try the command line argument SDxSysroot, then try to find it as part of the platform or fall back to SDK for default SDx platforms 
SET (SDxTestCommandLineSysroot ${SDxSysroot})
UNSET (SDxSysroot CACHE)
find_path(SDxSysroot "usr/include/stdlib.h" PATHS ${SDxTestCommandLineSysroot} PATH_SUFFIXES "" NO_DEFAULT_PATH)
find_path(SDxSysroot "usr/include/stdlib.h" PATHS "${SDxPlatform}/sw/sysroot" PATH_SUFFIXES "" NO_DEFAULT_PATH)
find_path(SDxSysroot "usr/include/stdlib.h" PATHS "${CMAKE_FIND_ROOT_PATH}/libc" PATH_SUFFIXES "" NO_DEFAULT_PATH)
MESSAGE (STATUS "SDx sysroot: ${SDxSysroot}")
#change OpenCV_DIR to cross compiled libraries in sysroot 
SET (ENV{OpenCV_DIR} ${SDxSysroot}/usr)
SET (ENV{OPENCV_DIR} ${SDxSysroot}/usr)
SET (ENV{GSTREAMER_DIR} ${SDxSysroot}/usr) 

# set up cross compilation paths
SET (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET (CMAKE_SKIP_BUILD_RPATH FALSE)
SET (CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
SET (CMAKE_INSTALL_RPATH ${SDxSysroot}/lib;${SDxSysroot}/usr/lib;${SDxSysroot}/lib/${gnuPrefix2};${SDxSysroot}/usr/lib/${gnuPrefix2})
SET (CMAKE_LIBRARY_PATH ${SDxSysroot}/lib)
set (CMAKE_INCLUDE_PATH ${SDxSysroot}/usr/)

# Added includes for ultra96 build
include_directories(
  ${SDxSysroot}/usr/include/c++/7 
  ${SDxSysroot}/usr/include/c++/7/${gnuPrefix2} 
  ${SDxSysroot}/usr/include/c++/7/backward 
  ${SDxSysroot}/usr/include
  ${SDxSysroot}/usr/include/${gnuPrefix2}
  )

# -rpath-link needed in 2018.2 reVISION sysroots
SET (CMAKE_SHARED_LINKER_FLAGS "--sysroot=${SDxSysroot} -Wl,-rpath-link,${SDxSysroot}/lib:${SDxSysroot}/usr/lib:${SDxSysroot}/lib/${gnuPrefix2}:${SDxSysroot}/usr/lib/${gnuPrefix2}" CACHE STRING "" FORCE)
SET (CMAKE_EXE_LINKER_FLAGS "--sysroot=${SDxSysroot} -Wl,-rpath-link,${SDxSysroot}/lib:${SDxSysroot}/usr/lib:${SDxSysroot}/lib/${gnuPrefix2}:${SDxSysroot}/usr/lib/${gnuPrefix2}" CACHE STRING "" FORCE)


#Windows specifics to enable sds++ cross compilation
if (WIN32)

	SET (SDxHostSystemBinUtilsPath ${CMAKE_FIND_ROOT_PATH}/bin)
	SET (CMAKE_AR ${SDxHostSystemBinUtilsPath}/ar CACHE FILEPATH "Archiver")
	SET (CMAKE_LINKER ${SDxHostSystemBinUtilsPath}/ld)
	SET (CMAKE_NM ${SDxHostSystemBinUtilsPath}/nm)
	SET (CMAKE_OBJCOPY ${SDxHostSystemBinUtilsPath}/objcopy)
	SET (CMAKE_OBJDUMP ${SDxHostSystemBinUtilsPath}/objdump)
	SET (CMAKE_RANLIB ${SDxHostSystemBinUtilsPath}/ranlib)
	SET (CMAKE_STRIP ${SDxHostSystemBinUtilsPath}/strip)

	MESSAGE(STATUS "CMAKE_FIND_ROOT_PATH: ${CMAKE_FIND_ROOT_PATH}")
	MESSAGE(STATUS "CMAKE_AR: ${CMAKE_AR}")

	include_directories(
		${SDxSysroot}/usr/include/glib-2.0 
		${SDxSysroot}/usr/lib/glib-2.0/include 

		${XSDK_PARENT}/gnu/${gnuArch}/nt/${gnuPrefix1}/${gnuPrefix2}/include/c++/6.2.1 
		${XSDK_PARENT}/gnu/${gnuArch}/nt/${gnuPrefix1}/${gnuPrefix2}/include/c++/6.2.1/${gnuPrefix2} 
		${XSDK_PARENT}/gnu/${gnuArch}/nt/${gnuPrefix1}/${gnuPrefix2}/include/c++/6.2.1/backward 
		${XSDK_PARENT}/gnu/${gnuArch}/nt/${gnuPrefix1}/lib/gcc/${gnuPrefix2}/6.2.1/include 
		${XSDK_PARENT}/gnu/${gnuArch}/nt/${gnuPrefix1}/lib/gcc/${gnuPrefix2}/6.2.1/include-fixed 
		${XSDK_PARENT}/gnu/${gnuArch}/nt/${gnuPrefix1}/${gnuPrefix2}/include
	)
endif (WIN32)
