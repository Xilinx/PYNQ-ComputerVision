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

# cmake .. -DCMAKE_TOOLCHAIN_FILE=toolchain_sdx2017.4.cmake
#  -DSDxPlatform="<absolute path to platform or predefined sdx platform name>"
#  -DSDxClockID="clock ID number"
#  -DSDxArch="arm32 or arm64"
#  -DSDxSysroot="absolute path to the sysroot folder"

set(SDxPlatform "zcu102" CACHE STRING "SDx platform")
message(STATUS "IN TOOLCHAINFILE: SDx platform: ${SDxPlatform} ")

if (${SDxPlatform} STREQUAL "zcu102") 		#zcu102 clock ID def (MHz): 0=100, 1=200, 2=300, 3=400
  set(SDxClockID "3" CACHE STRING "SDx clock ID")
  message(STATUS "zcu102 clock def in MHz: 0=100, 1=200, 2=300, 3=400")
  set(SDxArch "arm64")
elseif (${SDxPlatform} STREQUAL "zc706") 	#zc706 clock ID def (MHz): 0=166, 1=143, 2=100, 3=200
  set(SDxClockID "3" CACHE STRING "SDx clock ID")
  message(STATUS "zc706 clock def in MHz: 0=100, 1=200, 2=300, 3=400")
  set(SDxArch "arm32")
elseif (${SDxPlatform} STREQUAL "zc702") 	#zc702 clock ID def (MHz): 0=166, 1=143, 2=100, 3=200
  set(SDxClockID "3" CACHE STRING "SDx clock ID")
  message(STATUS "zc702 clock def in MHz:  0=166, 1=143, 2=100, 3=200")
  set(SDxArch "arm32")
else (${SDxPlatform} STREQUAL "zcu102")
  message(STATUS "non default platform, please also set SDx clock ID (default clock ID is 0) and SDx arch (default is 64 bit)")
  set(SDxClockID "0" CACHE STRING "SDx clock ID")
  set(SDxArch "arm64" CACHE STRING "SDx aarch")
endif (${SDxPlatform} STREQUAL "zcu102")

message(STATUS "SDx platform: ${SDxPlatform} , clock ID: ${SDxClockID}, aarch: ${SDxArch} bit")

#set general sdx compiler and linker flags
UNSET(CMAKE_CXX_FLAGS)
set(CMAKE_CXX_FLAGS "-sds-pf ${SDxPlatform} -dmclkid ${SDxClockID}" CACHE STRING "" FORCE)

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

if (${SDxArch} STREQUAL "arm64") # 64 bit toolchain
  SET (CMAKE_FIND_ROOT_PATH ${XSDK_PARENT}/gnu/aarch64/${SDxHostSystemName}/aarch64-linux/aarch64-linux-gnu)
  SET (CMAKE_SYSTEM_PROCESSOR aarch64)

  #extra compilation flags
  #NONE
else (${SDxArch} STREQUAL "arm64") #32 bit toolchain
  SET (CMAKE_FIND_ROOT_PATH ${XSDK_PARENT}/gnu/aarch32/${SDxHostSystemName}/gcc-arm-linux-gnueabi/arm-linux-gnueabihf)
  SET (CMAKE_SYSTEM_PROCESSOR arm)

  #extra compilation flags
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__ARM_PCS_VFP")
endif (${SDxArch} STREQUAL "arm64")


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
SET (CMAKE_INSTALL_RPATH ${SDxSysroot}/lib)
SET (CMAKE_LIBRARY_PATH ${SDxSysroot}/lib)
set (CMAKE_INCLUDE_PATH ${SDxSysroot}/usr/)
SET (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

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
		${SDxSysroot}/usr/include/c++/6.2.1 
		${SDxSysroot}/usr/include/c++/6.2.1/aarch64-xilinx-linux 
		${SDxSysroot}/usr/include/c++/6.2.1/backward 
		${SDxSysroot}/usr/include 
		${SDxSysroot}/usr/include/glib-2.0 
		${SDxSysroot}/usr/lib/glib-2.0/include 

		${XSDK_PARENT}/gnu/aarch64/nt/aarch64-linux/aarch64-linux-gnu/include/c++/6.2.1 
		${XSDK_PARENT}/gnu/aarch64/nt/aarch64-linux/aarch64-linux-gnu/include/c++/6.2.1/aarch64-linux-gnu 
		${XSDK_PARENT}/gnu/aarch64/nt/aarch64-linux/aarch64-linux-gnu/include/c++/6.2.1/backward 
		${XSDK_PARENT}/gnu/aarch64/nt/aarch64-linux/lib/gcc/aarch64-linux-gnu/6.2.1/include 
		${XSDK_PARENT}/gnu/aarch64/nt/aarch64-linux/lib/gcc/aarch64-linux-gnu/6.2.1/include-fixed 
		${XSDK_PARENT}/gnu/aarch64/nt/aarch64-linux/aarch64-linux-gnu/include
	)
endif (WIN32)