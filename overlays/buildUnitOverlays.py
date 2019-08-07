#!/usr/bin/env python3

#*****************************************************************************
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
#*****************************************************************************

#*****************************************************************************
#
#     Author: Jack Lo <jackl@xilinx.com>
#     Date:   2019/08/02
#
#*****************************************************************************

import sys
import argparse
import os
import glob
import errno
from distutils.dir_util import mkpath
import shutil

toolchain_file  = "../../frameworks/cmakeModules/cmakeModulesXilinx/toolchain_sds.cmake"
arch            = "arm64"
clockID         = "3"
platform        = "/group/xrlabs/projects/image_processing/platforms/Ultra96/ultra_v2.4/2018.3/ultra"
usePL           = "ON"
noBitstream     = "OFF"
noSDCardImage   = "ON"
overlayFileDir  = "overlaysUltra96"
buildDir        = "buildUltra96"

#clockID         = "2"
#platform        = "/group/xrlabs/tools/reVISION/platforms/zcu102_rv_ss/2018.3/zcu102_rv_ss"
#platform        = "/group/xrlabs/projects/image_processing/platforms/ZCU104/ultra_v2.4/2018.3/ultra/"
#sysroot         = "/group/xrlabs/projects/image_processing/platforms/ZCU104/sysroot/zcu104_sysroot_copy"
#usePL           = "ON"
#noBitstream     = "OFF"
#noSDCardImage   = "ON"
#overlayFileDir = "overlayFilesZCU104"
#buildDir        = "buildzcu104"

cmakeListFile   = "CMakeLists.txt"

try: sysroot
except NameError: sysroot = None

#*****************************************************************************
# Create a CMakeLists.txt file
#*****************************************************************************
def createCMakeFile(component):
    file = open(cmakeListFile,"w")
    if(file):
        file.write("###############################################################################\n")
        file.write("#  Copyright (c) 2019, Xilinx, Inc.\n")
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
        file.write("#     Author: Jack Lo <jackl@xilinx.com> and Kristof Denolf <kristof@xilinx.com>\n")
        file.write("#     Date:   2018/02/09\n")
        file.write("\n")
        file.write("\n")
        file.write("# cmake needs this line\n")
        file.write("cmake_minimum_required(VERSION 2.8)\n")
        file.write("# define the overlay name\n")
        file.write("SET (overlayName xv2"+component+")\n")
        file.write("\n")
        file.write("# define component list in overlay here\n")
        file.write("set(componentList \""+component+"\")\n")
        file.write("\n")
        file.write("#SETTING USER SPECIFIC INSTANTIATION PARAMETERS\n")
        file.write("set(maxWidthCMakeParam 1920)\n")
        file.write("set(maxHeightCMakeParam 1080)\n")
        file.write("\n")
        file.write("################## no changes needed after this line ##############################\n")
        file.write("\n")
        file.write("set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../frameworks/cmakeModules/cmakeModulesXilinx ${CMAKE_CURRENT_SOURCE_DIR}/../../frameworks/cmakeModules)\n")
        file.write("include(rulesForXfOpenCV)\n")
        file.write("\n")
        file.write("createOverlayWithPythonBindings(${overlayName} \"../..\" \"${componentList}\")\n")
        return 0 # to match shell return code
    else:
        return 1 # to match shell return code

#*****************************************************************************
# Build HW unit test via SDx
#*****************************************************************************
def buildOverlay(component):
    #target = component.replace("test","testSDx")
    #print('Building {0:32s} ... '.format(component), end='', flush=True)
    print('Building {0:32s} ... \n'.format(component), end='', flush=True)
    #print("Building "+target+" ...", end='')
    #crate overlay dir
    overlay = 'xv2'+component
    if (not os.path.exists("./"+overlay)):
        mkpath(overlay)
    os.chdir(overlay)

    #create CMakeLists.txt
    if (os.path.exists("./"+cmakeListFile)):
        os.remove('./'+cmakeListFile)
    createCMakeFile(component)

    #create build diretory 
    if (os.path.exists("./"+buildDir)):
        shutil.rmtree('./'+buildDir)
    mkpath(buildDir)
    os.chdir(buildDir)
    
	#run cmake here
    if sysroot is None:
        os.system("cmake .. -DCMAKE_TOOLCHAIN_FILE="+toolchain_file+" -DSDxArch="+arch+" -DSDxClockID="+clockID+" -DSDxPlatform="+platform+" -DusePL="+usePL+" -DnoBitstream="+noBitstream+" -DnoSDCardImage="+noSDCardImage+" >& cmake.log")
    else:
        os.system("cmake .. -DCMAKE_TOOLCHAIN_FILE="+toolchain_file+" -DSDxArch="+arch+" -DSDxClockID="+clockID+" -DSDxPlatform="+platform+" -DSDxSysroot="+sysroot+" -DusePL="+usePL+" -DnoBitstream="+noBitstream+" -DnoSDCardImage="+noSDCardImage+" >& cmake.log") 
    if '-- Build files have been written to' in open('cmake.log').read():
        print("\t CMake OK", end='', flush=True)
    else:
        print("\t CMake FAIL", end='', flush=True)

    #run make
    os.system("make install >&  make.log")
    if '[100%] Built target' in open('make.log').read():
        print("\t build OK", flush=True)
    else:
        print("\t build FAIL", flush=True)

    #copy generated files to overlayFilesDir
    os.system("cp ./libarm64/* ../../"+overlayFileDir)

    os.chdir("..") # leave build directory

    os.chdir("..") # leave overlay build directory

#*****************************************************************************
# Main function
#*****************************************************************************
desc = "Script to build overlays for all components."
parser = argparse.ArgumentParser(description = desc)
parser.add_argument("-l","--list", help="file of components to build.")
args = parser.parse_args()

if(not args.list):
    path = '../components/*'
    dirs = glob.glob(path)
    removePath = True
else:
    removePath = False
    try:
        with open(args.list,'r') as myfile:
            dirs = myfile.readlines()
    except IOError as e:
        if e.errno == errno.EACCES:
            print("file exists, but isn't readable")
        elif e.errno == errno.ENOENT:
            print("file isn't readable because it isn't there")

print("DIRS:"+str(dirs))
if not os.path.exists(overlayFileDir):
    os.mkdir(overlayFileDir)
for fullComponent in dirs:
    component = fullComponent.rstrip()
    if(removePath):
        componentName = os.path.basename(component)
    else:
        componentName = component 
    buildOverlay(componentName)

