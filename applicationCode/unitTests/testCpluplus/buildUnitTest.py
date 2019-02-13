#!/usr/bin/python

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
import os
import glob
import errno
from distutils.dir_util import mkpath
import shutil

toolchain_file = "../../../../../frameworks/cmakeModules/toolchain_sdx2018.2.cmake"
arch           = "arm64"
clockID        = "1"
#platform       = "/platforms/Ultra96/bare/2018.2/ultra"
platform       = "/group/xrlabs/projects/image_processing/platforms/Ultra96/bare_v2.3/2018.3_jackl/ultra"
usePL          = "ON"
noBitstream    = "OFF"
noSDCardImage  = "ON"
unitTestFileDir = "unitTestFiles"

def buildOverlay(component):
    target = component.replace("test","testSDx")
    print("Building "+target+"...")
    mkpath("build")
    os.chdir("build")
    #run cmake here
    os.system("time cmake .. -DCMAKE_TOOLCHAIN_FILE="+toolchain_file+" -DSDxArch="+arch+" -DSDxClockID="+clockID+" -DSDxPlatform="+platform+" -DusePL="+usePL+" -DnoBitstream="+noBitstream+" -DnoSDCardImage="+noSDCardImage+" |& tee cmake.log")
    #run make
    os.system("time make "+target+"|& tee make.log")
    bitstream = target+".bit"
    createConvBif(bitstream)
    os.system("bootgen -image conv.bif -arch zynqmp -o "+bitstream+".bin -w")
    #cp bitstream and elf file to unitTestFiles
    shutil.copy2(target, '../../unitTestFiles')
    shutil.copy2(bitstream+".bin", '../../unitTestFiles')
    os.chdir("..")

def createConvBif(component):
    with open("conv.bif","w") as file:
        file.write("all:\n")
        file.write("{\n")
        file.write("    [destination_device = pl] "+component+"\n")
        file.write("}\n")
        

# Main function

if(len(sys.argv) == 1):
    path = './*'
    dirs = glob.glob(path)
    removePath = True
elif(len(sys.argv) == 2):
    removePath = False
    try:
        with open(sys.argv[1],'r') as myfile:
            dirs = myfile.readlines()
    except IOError as e:
        if e.errno == errno.EACCES:
            print("file exists, but isn't readable")
        elif e.errno == errno.ENOENT:
            print("files isn't readable because it isn't there")
else:
    print("buildUnitTest.py [text file list of unitTests to build]")
    quit()

print("DIRS:"+str(dirs))
if not os.path.exists(unitTestFileDir):
    os.mkdir(unitTestFileDir)
for fullComponent in dirs:
    try:
        component = fullComponent.rstrip()
        os.chdir(component)
        if(removePath):
            componentName = os.path.basename(component)
        else:
            componentName = component 
        buildOverlay(componentName)
        os.chdir("..")
    except IOError as exc:
        if exc.errno != errno.EISDIR:
            raise

