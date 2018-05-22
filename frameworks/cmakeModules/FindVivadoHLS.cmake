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

######################################################################
## Find VivadoHLS - variables set:
##  VivadoHLS_FOUND
##  VivadoHLS_INCLUDE_DIRS
##
## This script is a combination from multiple sources that use
## different variable names; the names are reconciled at the end
## of the script.

###########################################################
#
## 1: Setup:
# The following variables are optionally searched for defaults
#  VIVADO_HLS:            Base directory of Vivado HLS to use.
#
## 2: Variable
# The following are set after configuration is done: 
#  
#  VivadoHLS_FOUND
#  VivadoHLS_INCLUDE_DIR
#----------------------------------------------------------


##====================================================
## Find Vivado HLS libraries
##----------------------------------------------------
MESSAGE(STATUS "VIVADO HLS base path: $ENV{VIVADO_HLS}")

find_path(VivadoHLS_INCDIR "hls_opencv.h" PATHS "$ENV{VIVADO_HLS}" PATH_SUFFIXES "include" NO_CMAKE_FIND_ROOT_PATH)
find_path(VivadoHLS_INCDIR_HLS "hls_video_core.h" PATHS "$ENV{VIVADO_HLS}" PATH_SUFFIXES "include/hls" NO_CMAKE_FIND_ROOT_PATH)
	
set(VivadoHLS_INCLUDE_DIR ${VivadoHLS_INCDIR} ${VivadoHLS_INCDIR_HLS})
	
if(EXISTS ${VivadoHLS_INCDIR} AND EXISTS ${VivadoHLS_INCDIR_HLS})
	MESSAGE(STATUS "VIVADO HLS FOUND SO ON")
	set(VivadoHLS_FOUND ON)
else(EXISTS ${VivadoHLS_INCDIR} AND EXISTS ${VivadoHLS_INCDIR_HLS})
	MESSAGE(STATUS "VIVADO HLS NOT FOUND SO OFF")
	set(VivadoHLS_FOUND OFF)
endif(EXISTS ${VivadoHLS_INCDIR} AND EXISTS ${VivadoHLS_INCDIR_HLS})
	
set(VivadoHLS_INCLUDE_DIRS ${VivadoHLS_INCLUDE_DIR})
mark_as_advanced(VivadoHLS_INCLUDE_DIR)
