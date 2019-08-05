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
#     Date:   2019/01/25
#
#*****************************************************************************

import sys
import argparse
import os
import glob
import errno
from distutils.dir_util import mkpath
import shutil

toolchain_file  = "../../../../../frameworks/cmakeModules/cmakeModulesXilinx/toolchain_sds.cmake"
arch            = "arm64"
clockID         = "3"
platform        = "/group/xrlabs/projects/image_processing/platforms/Ultra96/ultra_v2.4/2018.3/ultra"
usePL           = "ON"
noBitstream     = "OFF"
noSDCardImage   = "ON"
unitTestFileDir = "unitTestFilesUltra96"
buildDir        = "buildUltra96"

#clockID         = "2"
#platform        = "/group/xrlabs/tools/reVISION/platforms/zcu102_rv_ss/2018.3/zcu102_rv_ss"
#platform        = "/group/xrlabs/tools/reVISION/platforms/zcu104_rv_min_2019.1"
#usePL           = "ON"
#noBitstream     = "OFF"
#noSDCardImage   = "ON"
#unitTestFileDir = "unitTestFilesZCU102"
#buildDir        = "buildzcu102"

#unitTestFileDir = "unitTestFilesCSIM"
#buildDir        = "build2019.1"

#*****************************************************************************
# Create a conversion .bif file
#*****************************************************************************
def createConvBif(component):
    #with open("conv.bif","w") as file:
    file = open("conv.bif","w")
    if(file):
        file.write("all:\n")
        file.write("{\n")
        file.write("    [destination_device = pl] "+component+"\n")
        file.write("}\n")
        return 0 # to match shell return code
    else:
        return 1 # to match shell return code

#*****************************************************************************
# Build HW unit test via SDx
#*****************************************************************************
def buildUnitTest(component):
    target = component.replace("test","testSDx")
    print('Building {0:32s} ... '.format(target), end='', flush=True)
    #print("Building "+target+" ...", end='')
    if (os.path.exists("./"+buildDir)):
        shutil.rmtree('./'+buildDir)
    mkpath(buildDir)
    os.chdir(buildDir)
    #run cmake here
    os.system("cmake .. -DCMAKE_TOOLCHAIN_FILE="+toolchain_file+" -DSDxArch="+arch+" -DSDxClockID="+clockID+" -DSDxPlatform="+platform+" -DusePL="+usePL+" -DnoBitstream="+noBitstream+" -DnoSDCardImage="+noSDCardImage+" >& cmake.log") 
    if '-- Build files have been written to' in open('cmake.log').read():
        print("\t CMake OK", end='', flush=True)
    else:
        print("\t CMake FAIL", end='', flush=True)
    #run make
    os.system("make "+target+" >&  make.log")
    if '[100%] Built target' in open('make.log').read():
        print("\t build OK", flush=True)
    else:
        print("\t build FAIL", flush=True)
        os.chdir("..")
        return
    #convert bitstream
    bitstream = target+".bit"
    res = createConvBif(bitstream)
    if(res):
        print("ERROR: Could not create .bif file.")
        os.chdir("..")
        return
    res = os.system("bootgen -image conv.bif -arch zynqmp -o "+bitstream+".bin -w >& bootgen.log")
    if(res):
        print("ERROR: bootgen error.")
        os.chdir("..")
        return
    #cp bitstream and elf file to unitTestFiles
    shutil.copy2(target, '../../'+unitTestFileDir)
    shutil.copy2(bitstream+".bin", '../../'+unitTestFileDir)
    os.chdir("..")


#*****************************************************************************
# Build SW unit test via SDx (C simulation)
#*****************************************************************************
def buildUnitTestCSim(component):
    target = component.replace("test","testSDx")
    print('Building {0:32s} ... '.format(target), end='', flush=True)
    #print("Building "+target+" ...", end='')
    if (os.path.exists("./"+buildDir)):
        shutil.rmtree('./'+buildDir)
    mkpath(buildDir)
    os.chdir(buildDir)
    #run cmake here
    os.system("cmake .. >& cmake.log")
    if '-- Build files have been written to' in open('cmake.log').read():
        print("\t CMake OK", end='')
    else:
        print("\t CMake FAIL", end='')
    #run make
    os.system("make "+target+" >&  make.log")
    if '[100%] Built target' in open('make.log').read():
        print("\t build OK")
    else:
        print("\t build FAIL")
        
    os.chdir("..")


#*****************************************************************************
# Main function
#*****************************************************************************
desc = "Script to run build on test directories."
parser = argparse.ArgumentParser(description = desc)
parser.add_argument("-c","--csim", help="build for C simulation only", action="store_true")
parser.add_argument("-l","--list", help="file of tests diretories to build.")
args = parser.parse_args()

if(not args.list):
    path = './test*'
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
if not os.path.exists(unitTestFileDir):
    os.mkdir(unitTestFileDir)
for fullComponent in dirs:
    component = fullComponent.rstrip()
    if(os.path.isdir(component)):
        os.chdir(component)
        if(removePath):
            componentName = os.path.basename(component)
        else:
            componentName = component 
        if(args.csim):
            buildUnitTestCSim(componentName)
        else:
            buildUnitTest(componentName)
        os.chdir("..")
    else:
        print("ERROR: Directory "+component+" doesn't exist.")

