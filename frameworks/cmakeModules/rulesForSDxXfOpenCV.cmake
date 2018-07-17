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
include (rulesForSDxTargets)

macro (setXfOpenCVDefinitions)
set(XF_BORDER_CONSTANT  0)
set(XF_BORDER_REPLICATE 1)

set(XF_NPPC1   1)
set(XF_NPPC2   2)
set(XF_NPPC4   4)
set(XF_NPPC8   8)
set(XF_NPPC16  16)
set(XF_NPPC32  32)

set(XF_8UC1   0)
set(XF_16UC1  1)
set(XF_16SC1  2)
set(XF_32UC1  3)
set(XF_32FC1  4)
set(XF_32SC1  5)
set(XF_8UC2   6)
set(XF_8UC4   7)
set(XF_2UC1   8)
set(XF_8UC3   9)
set(XF_16UC3  10)

endmacro()

macro (setDefaultInstantiationParameters)
setXfOpenCVDefinitions()

#overall defaults
if(NOT DEFINED borderTypeCMakeParam)
	set(borderTypeCMakeParam ${XF_BORDER_REPLICATE} CACHE STRING "border type") #XF_BORDER_CONSTANT XF_BORDER_REPLICATE
endif()

if(NOT DEFINED MedianBorderTypeCMakeParam)
	set(MedianBorderTypeCMakeParam ${XF_BORDER_REPLICATE} CACHE STRING "medianBlur border type")   # only XF_BORDER_REPLICATE is supported
endif()
if(NOT DEFINED BoxFilterBorderTypeCMakeParam)
	set(BoxFilterBorderTypeCMakeParam ${XF_BORDER_CONSTANT} CACHE STRING "boxFilter border type") # only XF_BORDER_CONSTANT is supported
endif()
if(NOT DEFINED maxWidthCMakeParam)
	set(maxWidthCMakeParam 1920 CACHE STRING "maximum width")
endif()

if(NOT DEFINED maxHeightCMakeParam)
	set(maxHeightCMakeParam 1080 CACHE STRING "maximum height")
endif()

if(NOT DEFINED srcTypeCMakeParam)
	set(srcTypeCMakeParam ${XF_8UC1} CACHE STRING "src type")
endif()

if(NOT DEFINED dstTypeCMakeParam)
	set(dstTypeCMakeParam ${XF_8UC1} CACHE STRING "dst type")
endif()

if(NOT DEFINED NPCCMakeParam)
	set(NPCCMakeParam ${XF_NPPC1} CACHE STRING "number of pixels per clock")
endif()

#filter2D
if(NOT DEFINED kernelSizeCMakeParam)
	set(kernelSizeCMakeParam 3 CACHE STRING "kernel size")
endif()

if(NOT DEFINED XSHIFTCMakeParam)
	set(XSHIFTCMakeParam 13 CACHE STRING "shift for fixed point precision")
endif()

#remap
if(NOT DEFINED mapTypeCMakeParam)
	set(mapTypeCMakeParam ${XF_32FC1} CACHE STRING "map type")
endif()

if(NOT DEFINED windowRowsCMakeParam)
	set(windowRowsCMakeParam 64 CACHE STRING "linebuffer size")
endif()

#StereoBM
if(NOT DEFINED blockSizeCMakeParam)
	set(blockSizeCMakeParam 19 CACHE STRING "stereoBM block size")
endif()

if(NOT DEFINED numberOfDisparitiesCMakeParam)
	set(numberOfDisparitiesCMakeParam 64 CACHE STRING "number of disparities")
endif()

if(NOT DEFINED numberOfDisparityUnitsCMakeParam)
	set(numberOfDisparityUnitsCMakeParam 2 CACHE STRING "number of disparity units")
endif()

if(NOT DEFINED disparityTypeCMakeParam)
	set(disparityTypeCMakeParam ${XF_16UC1} CACHE STRING "disparity type")
endif()

#Canny
if(NOT DEFINED apertureSizeCMakeParam)
	set(apertureSizeCMakeParam 3 CACHE STRING "Sobel aperture size")
endif()

if(NOT DEFINED L2gradientCMakeParam)
	set(L2gradientCMakeParam 0 CACHE STRING "L2 gradient switch")
endif()

if(NOT DEFINED srcNPCCMakeParam)
	set(srcNPCCMakeParam ${XF_NPPC1} CACHE STRING "canny in number of pixels per clock")
endif()

if(NOT DEFINED dstNPCCMakeParam)
	set(dstNPCCMakeParam ${XF_NPPC8} CACHE STRING "edge trace out number of pixels per clock")
endif()

if(NOT DEFINED dstIntermedTypeCMakeParam)
	set(dstIntermedTypeCMakeParam ${XF_2UC1} CACHE STRING "canny intermediate dst type")
endif()

if(NOT DEFINED srcIntermedTypeCMakeParam)
	set(srcIntermedTypeCMakeParam ${XF_2UC1} CACHE STRING "edge tracing intermediate src type")
endif()

if(NOT DEFINED dstIntermedNPCCMakeParam)
	set(dstIntermedNPCCMakeParam ${XF_NPPC4} CACHE STRING "canny intermediate dst number of pixels per clock")
endif()

if(NOT DEFINED srcIntermedNPCCMakeParam)
	set(srcIntermedNPCCMakeParam ${XF_NPPC32} CACHE STRING "edge tracing intermediate src number of pixels per clock")
endif()



#CornerHarris
if(NOT DEFINED filterSizeCMakeParam)
	set(filterSizeCMakeParam 3 CACHE STRING "Filter size (3,5 and 7 supported)")
endif()
if(NOT DEFINED blockWidthCMakeParam)
	set(blockWidthCMakeParam 3 CACHE STRING "Block width (3,5 and 7 supported)")
endif()
if(NOT DEFINED NMSRadiusCMakeParam)
	set(NMSRadiusCMakeParam 1 CACHE STRING "Radius for non-maximum suppression (1 and 2 supported)")
endif()

#Normalization
if(NOT DEFINED ID0)
	set(ID0 0 CACHE STRING "normalization type")
endif()
if(NOT DEFINED ID1)
	set(ID1 1 CACHE STRING "normalization type")
endif()

# wrapAffine 
if(NOT DEFINED interpolationTypeCMakeParam)
	set(interpolationTypeCMakeParam 0 CACHE STRING "interpolation Type") # NN or Bilinear
endif()

# phase 
if(NOT DEFINED retTypeCMakeParam)
	set(retTypeCMakeParam ${XF_RADIANS} CACHE STRING "phase format Type") # XF_RADIANs or XF_DEGREES
endif()

# boxFilter 
if(NOT DEFINED boxFilterSizeCMakeParam)
	set(boxFilterSizeCMakeParam 3 CACHE STRING "box filter size 3,5,7") 
endif()
 
#Threshold
set(thresholdTypeCMakeParam 0)
set(inRangeTypeCMakeParam 1)

#arithmatic ops 
set(policyTypeCMakeParam 0) # XF_CONVERT_POLICY_SATURATE or XF_CONVERT_POLICY_TRUNCATE

#resize
if(NOT DEFINED srcWidthCMakeParam)
	set(srcWidthCMakeParam 1920 CACHE STRING "src maximum width")
endif()

if(NOT DEFINED srcHeightCMakeParam)
	set(srcHeightCMakeParam 1080 CACHE STRING "src maximum height")
endif()
if(NOT DEFINED dstWidthCMakeParam)
	set(dstWidthCMakeParam 500 CACHE STRING "dst maximum width")
endif()

if(NOT DEFINED dstHeightCMakeParam)
	set(dstHeightCMakeParam 500 CACHE STRING "dst maximum height")
endif()

endmacro()

function(buildSDxCompilerFlags componentList SDxCompileFlags)
	set(SDxCompileFlagsLocal "-fpic")

	foreach(componentNameLocal ${componentList})
		message(STATUS "working on flags for component ${componentNameLocal}")
		capFirstLetter(${componentNameLocal} componentNameLocalCap)
		if (${componentNameLocal} STREQUAL "filter2D")
			message(STATUS "generating flags for filter2D")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${borderTypeCMakeParam},${kernelSizeCMakeParam},${kernelSizeCMakeParam},${srcTypeCMakeParam},${dstTypeCMakeParam},${maxHeightCMakeParam},${maxWidthCMakeParam},${NPCCMakeParam}>\" xf${componentNameLocalCap}.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_custom_convolution.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "dilate")
			message(STATUS "generating flags for dilate")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${borderTypeCMakeParam},${srcTypeCMakeParam},${maxHeightCMakeParam},${maxWidthCMakeParam},${NPCCMakeParam}>\" xf${componentNameLocalCap}.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_dilation.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "erode")
			message(STATUS "generating flags for erode")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${borderTypeCMakeParam},${srcTypeCMakeParam},${maxHeightCMakeParam},${maxWidthCMakeParam},${NPCCMakeParam}>\" xf${componentNameLocalCap}.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_erosion.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "remap")
			message(STATUS "generating flags for remap")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${windowRowsCMakeParam},${srcTypeCMakeParam},${mapTypeCMakeParam},${srcTypeCMakeParam},${maxHeightCMakeParam},${maxWidthCMakeParam},${NPCCMakeParam}>\" xf${componentNameLocalCap}.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_remap.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "stereoBM")
			message(STATUS "generating flags for stereoBM")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocalCap}<${blockSizeCMakeParam},${numberOfDisparitiesCMakeParam},${numberOfDisparityUnitsCMakeParam},${srcTypeCMakeParam},${disparityTypeCMakeParam},${maxHeightCMakeParam},${maxWidthCMakeParam},${NPCCMakeParam}>\" xf${componentNameLocalCap}.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_stereoBM.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "canny")
			message(STATUS "generating flags for canny")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocalCap}<${apertureSizeCMakeParam},${L2gradientCMakeParam},${srcTypeCMakeParam},${dstIntermedTypeCMakeParam},${maxHeightCMakeParam},${maxWidthCMakeParam},${srcNPCCMakeParam},${dstIntermedNPCCMakeParam}>\" xf${componentNameLocalCap}.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_canny.hpp -clkid ${SDxClockID} -sds-end -sds-hw \"xf::EdgeTracing<${srcIntermedTypeCMakeParam},${dstTypeCMakeParam},${maxHeightCMakeParam},${maxWidthCMakeParam},${srcIntermedNPCCMakeParam},${dstNPCCMakeParam}>\" xf${componentNameLocalCap}.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_edge_tracing.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "bitwise_and")
			message(STATUS "generating flags for bitwise_and")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParam},${maxHeightCMakeParam},${maxWidthCMakeParam},${NPCCMakeParam}>\" xf${componentNameLocalCap}.cpp -files ${xfOpenCV_INCLUDE_DIRS}/core/xf_arithm.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "bitwise_or")
			message(STATUS "generating flags for bitwise_or")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParam},${maxHeightCMakeParam},${maxWidthCMakeParam},${NPCCMakeParam}>\" xf${componentNameLocalCap}.cpp -files ${xfOpenCV_INCLUDE_DIRS}/core/xf_arithm.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")	
		elseif (${componentNameLocal} STREQUAL "bitwise_xor")
			message(STATUS "generating flags for bitwise_xor")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParam},${maxHeightCMakeParam},${maxWidthCMakeParam},${NPCCMakeParam}>\" xf${componentNameLocalCap}.cpp -files ${xfOpenCV_INCLUDE_DIRS}/core/xf_arithm.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")	
		elseif (${componentNameLocal} STREQUAL "bitwise_not")
			message(STATUS "generating flags for bitwise_not")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${srcTypeCMakeParam},${maxHeightCMakeParam},${maxWidthCMakeParam},${NPCCMakeParam}>\" xf${componentNameLocalCap}.cpp -files ${xfOpenCV_INCLUDE_DIRS}/core/xf_arithm.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")		
		elseif (${componentNameLocal} STREQUAL "subtract")
			message(STATUS "generating flags for subtract")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${policyTypeCMakeParam},${srcTypeCMakeParam},${maxHeightCMakeParam},${maxWidthCMakeParam},${NPCCMakeParam}>\" xf${componentNameLocalCap}.cpp -files ${xfOpenCV_INCLUDE_DIRS}/core/xf_arithm.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		elseif (${componentNameLocal} STREQUAL "threshold")
			message(STATUS "generating flags for threshold")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocalCap}<${thresholdTypeCMakeParam},${srcTypeCMakeParam},${maxHeightCMakeParam},${maxWidthCMakeParam},${NPCCMakeParam}>\" xf${componentNameLocalCap}.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_threshold.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")		
		elseif (${componentNameLocal} STREQUAL "medianBlur")
			message(STATUS "generating flags for medianBlur")
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${filterSizeCMakeParam},${MedianBorderTypeCMakeParam},${srcTypeCMakeParam},${maxHeightCMakeParam},${maxWidthCMakeParam},${NPCCMakeParam}>\" xf${componentNameLocalCap}.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_median_blur.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")	
		elseif (${componentNameLocal} STREQUAL "boxFilter")
			message(STATUS "generating flags for boxFilter") 
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${BoxFilterBorderTypeCMakeParam},${boxFilterSizeCMakeParam},${srcTypeCMakeParam},${maxHeightCMakeParam},${maxWidthCMakeParam},${NPCCMakeParam}>\" xf${componentNameLocalCap}.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_box_filter.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}") 		
		elseif (${componentNameLocal} STREQUAL "resize")
			message(STATUS "generating flags for resize")  
			SET(SDxCompileFlagsLocal "-sds-hw \"xf::${componentNameLocal}<${interpolationTypeCMakeParam},${srcTypeCMakeParam},${srcHeightCMakeParam},${srcWidthCMakeParam},${dstHeightCMakeParam},${dstWidthCMakeParam},${NPCCMakeParam}>\" xf${componentNameLocalCap}.cpp -files ${xfOpenCV_INCLUDE_DIRS}/imgproc/xf_resize.hpp -clkid ${SDxClockID} -sds-end ${SDxCompileFlagsLocal}")
		
		
		else()
		endif()
	endforeach()

	set(${SDxCompileFlags} ${SDxCompileFlagsLocal} PARENT_SCOPE)
endfunction()

function (createWrapperCPPFiles componentList subDirLevels)
	foreach(componentNameLocal ${componentList})
	capFirstLetter(${componentNameLocal} componentNameLocalCap)
		message(STATUS "Creating xf${componentNameLocalCap}.cpp")
		configure_file(${PROJECT_SOURCE_DIR}/${subDirLevels}/components/${componentNameLocal}/xfSDxKernel/src/xf${componentNameLocalCap}.cpp.in src/xf${componentNameLocalCap}.cpp)	
	endforeach()
endfunction()

function (createWrapperCPPFilesList componentList subDirLevels wrapperCPPFileList)
	set(wrapperCPPFileListLocal "")
	foreach(componentNameLocal ${componentList})
		capFirstLetter(${componentNameLocal} componentNameLocalCap)
		list(APPEND wrapperCPPFileListLocal src/xf${componentNameLocalCap})
	endforeach()
	
	set(${wrapperCPPFileList} ${wrapperCPPFileListLocal} PARENT_SCOPE)
endfunction()

function (createPyOpenCVModulesInOverlayHeaderFile componentList incPath)
	
	set(fileName "${incPath}/xilinx_pyopencv_modules_in_overlay.h")
	file(WRITE ${fileName} "// CMake Automatically generated xilinx_pyopencv_modules_in_overlay.h\n\n")
	
	foreach(componentNameLocal ${componentList})
		capFirstLetter(${componentNameLocal} componentNameLocalCap)
		file(APPEND ${fileName} "#include <PythonBindingXfSDx${componentNameLocalCap}.h>\n")
	endforeach()
	file(APPEND ${fileName} "\n")
	file(APPEND ${fileName} "static PyMethodDef methods_cv[] = {\n")
	
	foreach(componentNameLocal ${componentList})
		capFirstLetter(${componentNameLocal} componentNameLocalCap)
		
		if (${componentNameLocal} STREQUAL "filter2D")
			file(APPEND ${fileName} "\t{\"filter2D\", (PyCFunction)pyopencv_cv_filter2D, METH_VARARGS | METH_KEYWORDS, \"filter2D(src, ddepth, kernel[, dst[, anchor[, delta[, borderType]]]]) -> dst\"},\n")
		elseif (${componentNameLocal} STREQUAL "dilate")
			file(APPEND ${fileName} "\t{\"dilate\", (PyCFunction)pyopencv_cv_dilate, METH_VARARGS | METH_KEYWORDS, \"dilate(src, kernel[, dst[, anchor[, iterations[, borderType[, borderValue]]]]]) -> dst\"},\n")
		elseif (${componentNameLocal} STREQUAL "erode")
			file(APPEND ${fileName} "\t{\"erode\", (PyCFunction)pyopencv_cv_erode, METH_VARARGS | METH_KEYWORDS, \"erode(src, kernel[, dst[, anchor[, iterations[, borderType[, borderValue]]]]]) -> dst\"},\n")
			elseif (${componentNameLocal} STREQUAL "remap")
			file(APPEND ${fileName} "\t{\"remap\", (PyCFunction)pyopencv_cv_remap, METH_VARARGS | METH_KEYWORDS, \"remap(src, map1, map2, interpolation[, dst[, borderMode[, borderValue]]]) -> dst\"},\n")
		elseif (${componentNameLocal} STREQUAL "stereoBM")
			file(APPEND ${fileName} "\t{\"StereoBM_create\", (PyCFunction)pyopencv_cv_StereoBM_create, METH_VARARGS | METH_KEYWORDS, \"StereoBM_create([, numDisparities[, blockSize]]) -> retval\"},\n")
			file(APPEND ${fileName} "\t{\"compute\", (PyCFunction)pyopencv_cv_StereoMatcher_compute, METH_VARARGS | METH_KEYWORDS, \"compute(left, right[, disparity]) -> disparity\"},\n")
		elseif (${componentNameLocal} STREQUAL "canny")
			file(APPEND ${fileName} "\t{\"Canny\", (PyCFunction)pyopencv_cv_Canny, METH_VARARGS | METH_KEYWORDS, \"Canny(image, threshold1, threshold2[, edges[, apertureSize[, L2gradient]]]) -> edges\"},\n")
		elseif (${componentNameLocal} STREQUAL "bitwise_and")
 			file(APPEND ${fileName} "\t{\"bitwise_and\", (PyCFunction)pyopencv_cv_bitwise_and, METH_VARARGS | METH_KEYWORDS, \"bitwise_and(src1, src2[, dst[, mask]]) -> dst\"},\n")
 		elseif (${componentNameLocal} STREQUAL "bitwise_or")
 			file(APPEND ${fileName} "\t{\"bitwise_or\", (PyCFunction)pyopencv_cv_bitwise_or, METH_VARARGS | METH_KEYWORDS, \"bitwise_or(src1, src2[, dst[, mask]]) -> dst\"},\n")
 		elseif (${componentNameLocal} STREQUAL "bitwise_xor")
 			file(APPEND ${fileName} "\t{\"bitwise_xor\", (PyCFunction)pyopencv_cv_bitwise_xor, METH_VARARGS | METH_KEYWORDS, \"bitwise_xor(src1, src2[, dst[, mask]]) -> dst\"},\n")
 		elseif (${componentNameLocal} STREQUAL "bitwise_not")
 			file(APPEND ${fileName} "\t{\"bitwise_not\", (PyCFunction)pyopencv_cv_bitwise_not, METH_VARARGS | METH_KEYWORDS, \"bitwise_not(src[, dst[, mask]]) -> dst\"},\n")
 		elseif (${componentNameLocal} STREQUAL "subtract")
			file(APPEND ${fileName} "\t{\"subtract\", (PyCFunction)pyopencv_cv_subtract, METH_VARARGS | METH_KEYWORDS, \"subtract(src1, src2[, dst[, mask[, dtype]]]) -> dst\"},\n")
 		elseif (${componentNameLocal} STREQUAL "threshold")
			file(APPEND ${fileName} "\t{\"threshold\", (PyCFunction)pyopencv_cv_threshold, METH_VARARGS | METH_KEYWORDS, \"threshold(src, thresh, maxval, type[, dst]) -> retval\"},\n")
		elseif (${componentNameLocal} STREQUAL "medianBlur")
 			file(APPEND ${fileName} "\t{\"medianBlur\", (PyCFunction)pyopencv_cv_medianBlur, METH_VARARGS | METH_KEYWORDS, \"medianBlur(src, ksize[, dst]) -> dst\"},\n")
 		elseif (${componentNameLocal} STREQUAL "boxFilter")
 			file(APPEND ${fileName} "\t{\"boxFilter\",(PyCFunction)pyopencv_cv_boxFilter, METH_VARARGS | METH_KEYWORDS, \"boxFilter(src, ddepth, ksize[, dst[, anchor[, normalize[,borderType]]]]) -> dst\"},\n") 
 		elseif (${componentNameLocal} STREQUAL "resize")
 			file(APPEND ${fileName} "\t{\"resize\",(PyCFunction)pyopencv_cv_resize, METH_VARARGS | METH_KEYWORDS, \"resize(src, dsize[, dst[, fx[, fy[, interpolation]]]]) -> dst\"},\n") 
 		
		else()
		endif()
    endforeach()
	file(APPEND ${fileName} "\t{NULL, NULL}\n")
	file(APPEND ${fileName} "};")

endfunction()

function (createPyOpenCVTypePublishHeaderFile componentList incPath)
	set(fileName "${incPath}/xilinx_pyopencv_generated_type_publish.h")
	file(WRITE ${fileName} "// CMake Automatically generated xilinx_pyopencv_generated_type_publish.h.h\n\n")
	
	foreach(componentNameLocal ${componentList})
		capFirstLetter(${componentNameLocal} componentNameLocalCap)
		
		if (${componentNameLocal} STREQUAL "stereoBM")
			file(APPEND ${fileName} "PUBLISH_OBJECT(\"${componentNameLocalCap}\", pyopencv_${componentNameLocalCap}_Type);")
		else()
		endif()
		
	endforeach()

endfunction()

function (createPyOpenCVTypeRegHeaderFile componentList incPath)
	set(fileName "${incPath}/xilinx_pyopencv_generated_type_reg.h")
	file(WRITE ${fileName} "// CMake Automatically generated xilinx_pyopencv_generated_type_reg.h.h\n\n")
	
	foreach(componentNameLocal ${componentList})
		capFirstLetter(${componentNameLocal} componentNameLocalCap)
		
		if (${componentNameLocal} STREQUAL "stereoBM")
			file(APPEND ${fileName} "MKTYPE2(${componentNameLocalCap});")
		else()
		endif()
		
	endforeach()
	
endfunction()


function(createModuleIncludeList componentList subDirLevels xfIncludeList)
	set(xfIncludeListLocal "")
	foreach(incElement ${componentList})
		if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${subDirLevels}/components/${incElement}/xfSDxKernel/inc/)
			list(APPEND xfIncludeListLocal ${PROJECT_SOURCE_DIR}/${subDirLevels}/components/${incElement}/xfSDxKernel/inc/)
		endif()
	endforeach()
	
	set(${xfIncludeList} ${xfIncludeListLocal} PARENT_SCOPE)
endfunction()

function (sysrootDependentLibSDSLink targetName)
	SET (SDS_SHARED_LIB "libsds_lib.so")
	find_path(SDS_SHARED_LIBPATH ${SDS_SHARED_LIB} PATHS ${CMAKE_LIBRARY_PATH})

	if (${SDS_SHARED_LIBPATH} STREQUAL "SDS_SHARED_LIBPATH-NOTFOUND")
		message(STATUS "INFO: ${SDS_SHARED_LIB} not found")
	else (${SDS_SHARED_LIBPATH} STREQUAL "SDS_SHARED_LIBPATH-NOTFOUND")
		target_link_libraries (${targetName} ${SDS_SHARED_LIBPATH}/${SDS_SHARED_LIB})
		message(STATUS "INFO: ${SDS_SHARED_LIB} found in ${SDS_SHARED_LIBPATH}, adding as link library to target: ${targetName}") 
	endif (${SDS_SHARED_LIBPATH} STREQUAL "SDS_SHARED_LIBPATH-NOTFOUND")
endfunction ()

function (createOverlayWithPythonBindings overlayName subDirLevels)
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/${subDirLevels}/frameworks/cmakeModules)
	include (rulesForSDxTargets)
	
	# Define project name
	project(${overlayName})
	capFirstLetter(${overlayName} overlayNameCap)

	# Find packages and compile xF::Mat in static lib (with sds++ while generating overlay).
	requireOpenCVAndVivadoHLS()
	find_package(xfOpenCV QUIET)

	message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
	message(STATUS "CMAKE_INCLUDE_PATH: ${CMAKE_INCLUDE_PATH}")
	message(STATUS "CMAKE_LIBRARY_PATH: ${CMAKE_LIBRARY_PATH}")
	message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
	
	#find pythonlib C++ headers for Python bindings
	find_path(PYTHON_INCLUDE_DIR "Python.h" PATH_SUFFIXES "include/python3.6m" "include/python2.7")
	find_path(NUMPY_INCLUDE_DIR "numpy/arrayobject.h" PATH_SUFFIXES "lib/python3.6/site-packages/numpy/core/include" "lib/python2.7/site-packages/numpy/core/include")
	message(STATUS "Python    include path: ${PYTHON_INCLUDE_DIR}")
	message(STATUS "Numpy     include path: ${NUMPY_INCLUDE_DIR}")
	
	createModuleIncludeList("${componentList}" ${subDirLevels} xFIncList)

	include_directories(
		${xFIncList}
		${OpenCV_INCLUDE_DIRS}
		${xfOpenCV_INCLUDE_DIRS}
	)
	
	# ---- SDx overlay target ----
	if (${VivadoHLS_FOUND})
	
		SET (currentTarget ${overlayName})
		message(STATUS "ADDING ${currentTarget} target")
		
		# set directives
		add_definitions(-DHLS_NO_XIL_FPO_LIB)
		
		# Define PL instantiation parameters
		setDefaultInstantiationParameters()
			
		#create wrapper cpp file
		createWrapperCPPFiles("${componentList}" ${subDirLevels})
		createWrapperCPPFilesList("${componentList}" ${subDirLevels} wrapperCPPFileList)
		message (STATUS "wrapper cpp file list ${wrapperCPPFileList}")
		
		# generate code for building Python bindings
		createPyOpenCVModulesInOverlayHeaderFile("${componentList}" ${PROJECT_BINARY_DIR}/inc/)
		createPyOpenCVTypePublishHeaderFile("${componentList}" ${PROJECT_BINARY_DIR}/inc/)
		createPyOpenCVTypeRegHeaderFile("${componentList}" ${PROJECT_BINARY_DIR}/inc/)
		configure_file(${PROJECT_SOURCE_DIR}/${subDirLevels}/frameworks/utilities/OpenCVPythonBindings/cv_xilinx.cpp.in src/cv_xilinx.cpp)
		
		# set directives and configure compile flags
		add_definitions(-DHLS_NO_XIL_FPO_LIB) 
		if (${CMAKE_C_COMPILER_ID} STREQUAL "SDSCC")
			buildSDxCompilerFlags("${componentList}" CompileFlags)
			message(STATUS "SDxCompileFlags ${CompileFlags}")
		else (${CMAKE_C_COMPILER_ID} STREQUAL "SDSCC") #native compilation
			SET(CompileFlags "-O3")
		endif (${CMAKE_C_COMPILER_ID} STREQUAL "SDSCC")
		
		add_library(${currentTarget} SHARED
			${wrapperCPPFileList}
			#${PROJECT_SOURCE_DIR}/${subDirLevels}/frameworks/utilities/OpenCVPythonBindings/cma_utils.cpp
			${PROJECT_BINARY_DIR}/src/cv_xilinx.cpp
		)
		
		set_target_properties (${currentTarget} PROPERTIES COMPILE_FLAGS ${CompileFlags})	#FLAGS for compile only	
		#set_target_properties (xFMat PROPERTIES COMPILE_FLAGS "-fpic")
		
		compilerDependentVivadoHLSInclude(${currentTarget})
		
		target_include_directories (${currentTarget} PUBLIC
			${PROJECT_BINARY_DIR}/inc/
			${PROJECT_SOURCE_DIR}/${subDirLevels}/frameworks/utilities/xF/
			${PROJECT_SOURCE_DIR}/${subDirLevels}/frameworks/utilities/OpenCVPythonBindings/
			${PYTHON_INCLUDE_DIR}
			${NUMPY_INCLUDE_DIR}
		)
		
		sysrootDependentLibSDSLink(${currentTarget})
		target_link_libraries(${currentTarget}
			${OpenCV_LIBS}
		)
		
		#install rules
		install(FILES "${CMAKE_BINARY_DIR}/lib${currentTarget}.so" DESTINATION ${PROJECT_SOURCE_DIR}/lib${SDxArch} RENAME ${overlayName}.so)
		install(FILES "${CMAKE_BINARY_DIR}/lib${currentTarget}.so.bit" DESTINATION ${PROJECT_SOURCE_DIR}/lib${SDxArch}/ RENAME ${overlayName}.bit)
		install(FILES "${CMAKE_BINARY_DIR}/_sds/p0/_vpl/ipi/sources_1/bd/system/hw_handoff/system_bd.tcl" DESTINATION ${PROJECT_SOURCE_DIR}/lib${SDxArch} RENAME ${overlayName}.tcl)
	endif (${VivadoHLS_FOUND})
	
	
	
	
endfunction()
