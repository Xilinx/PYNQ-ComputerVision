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
## Find xfOpenCV - variables set:
##  xfOpenCV_FOUND
##  xfOpenCV_INCLUDE_DIRS
##
## This script is a combination from multiple sources that use
## different variable names; the names are reconciled at the end
## of the script.

###########################################################
#
## 1: Setup:
# The following variables are optionally searched for defaults
#  XFOPENCV_PATH:            Base directory of Vivado HLS to use.
#
## 2: Variable
# The following are set after configuration is done: 
#  
#  xfOpenCV_FOUND
#  xfOpenCV_INCLUDE_DIRS
#----------------------------------------------------------


##====================================================
## Find xfOpenCV CV libraries
##----------------------------------------------------
MESSAGE(STATUS "xfOpenCV CV base path: $ENV{XFOPENCV_PATH}")
if(EXISTS "$ENV{XFOPENCV_PATH}")
 	find_path(XFOPENCV_DIR "common/xf_common.h" PATHS "$ENV{XFOPENCV_PATH}" PATH_SUFFIXES "include")	
	
	set(xfOpenCV_INCLUDE_DIRS ${XFOPENCV_DIR})
	
	if(EXISTS ${XFOPENCV_DIR})
		MESSAGE(STATUS "xfOpenCV FOUND SO ON")
		MESSAGE(STATUS "xfOpenCV include dirs: ${xfOpenCV_INCLUDE_DIRS}")
		set(xfOpenCV_FOUND ON)
	else(EXISTS ${XFOPENCV_DIR})
		MESSAGE(STATUS "xfOpenCV NOT FOUND SO OFF")
		set(xfOpenCV_FOUND OFF)
	endif(EXISTS ${XFOPENCV_DIR})
endif()
