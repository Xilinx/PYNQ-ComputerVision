#!/usr/bin/python

import sys
import os
import glob
import errno
from distutils.dir_util import mkpath
import shutil

toolchain_file = "../../../../frameworks/cmakeModules/toolchain_sdx2018.2.cmake"
arch           = "arm64"
clockID        = "1"
platform       = "/group/xrlabs/projects/image_processing/platforms/Ultra96/legal/ultra"
usePL          = "ON"
noBitstream    = "OFF"
noSDCardImage  = "ON"

def buildOverlay(component):
    print("Building "+component+"...")
    overlayName = component+"Overlay"
    #os.mkdir(overlayName)
    mkpath(overlayName)
    os.chdir(overlayName)
    createCMakeList(component)
    #os.mkdir("build")
    mkpath("build")
    os.chdir("build")
    #run cmake here
    os.system("time cmake .. -DCMAKE_TOOLCHAIN_FILE="+toolchain_file+" -DSDxArch="+arch+" -DSDxClockID="+clockID+" -DSDxPlatform="+platform+" -DusePL="+usePL+" -DnoBitstream="+noBitstream+" -DnoSDCardImage="+noSDCardImage+" |& tee cmake.log")
    #run make
    os.system("time make install |& tee make.log")
    #cp overlays to ../overlays/."
    for filename in glob.glob('./libarm*/*'):
        shutil.copy2(filename, '../../overlayFiles')
    #shutil.copy2('./libarm*/*','../../overlayFiles/.')
    os.chdir("../..")
        

def createCMakeList(component):
    with open("CMakeLists.txt","w") as file:
		file.write("###############################################################################\n")
		file.write("#  Copyright (c) 2018, Xilinx, Inc.\n")
		file.write("#  All rights reserved.\n")
		file.write("# \n")
		file.write("#  Redistribution and use in source and binary forms, with or without \n")
		file.write("#  modification, are permitted provided that the following conditions are met:\n")
		file.write("#\n")
		file.write("#  1.  Redistributions of source code must retain the above copyright notice, \n")
		file.write("#     this list of conditions and the following disclaimer.\n")
		file.write("#\n")
		file.write("#  2.  Redistributions in binary form must reproduce the above copyright \n")
		file.write("#      notice, this list of conditions and the following disclaimer in the \n")
		file.write("#      documentation and/or other materials provided with the distribution.\n")
		file.write("#\n")
		file.write("#  3.  Neither the name of the copyright holder nor the names of its \n")
		file.write("#      contributors may be used to endorse or promote products derived from \n")
		file.write("#      this software without specific prior written permission.\n")
		file.write("#\n")
		file.write("#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n")
		file.write("#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, \n")
		file.write("#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR \n")
		file.write("#  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR \n")
		file.write("#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, \n")
		file.write("#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, \n")
		file.write("#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;\n")
		file.write("#  OR BUSINESS INTERRUPTION). HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, \n")
		file.write("#  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR \n")
		file.write("#  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF \n")
		file.write("#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n")
		file.write("#\n")
		file.write("###############################################################################\n")
		file.write("\n")
		file.write("#     Author: Jack Lo <jackl@xilinx.com>\n")
		file.write("#     Date:   2018/12/20\n")
		file.write("\n")
		file.write("\n")
		file.write("# cmake needs this line\n")
		file.write("cmake_minimum_required(VERSION 2.8)\n")
		file.write("\n")
		file.write("# define the overlay name\n")
		file.write("SET (overlayName xv2"+component+")\n")
		file.write("\n")
		file.write("# define component list in overlay here\n")
		file.write("set(componentList \""+component+"\")\n")
		file.write("\n")
		file.write("#SETTING USER SPECIFIC INSTANTIATION PARAMETERS\n")
		file.write("#set(maxWidthCMakeParam 1920) \n")
		file.write("#set(maxHeightCMakeParam 1080)\n")
		file.write("\n")
		file.write("################## no changes needed after this line ##############################\n")
		file.write("\n")
		file.write("set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../../frameworks/cmakeModules)\n")
		file.write("include(rulesForSDxXfOpenCV)\n")
		file.write("\n")
		file.write("createOverlayWithPythonBindings(${overlayName} \"../../..\") \n")

# Main function

path = '../components/*'
dirs = glob.glob(path)
#os.mkdir("overlays")
mkpath("unitOverlays")
os.chdir("unitOverlays")
mkpath("overlayFiles")
for component in dirs:
    try:
        buildOverlay(os.path.basename(component))
    except IOError as exc:
        if exc.errno != errno.EISDIR:
            raise

