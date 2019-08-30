#!/usr/bin/python3.6

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

import sys
import os
import numpy as np
import cv2

#def listFirstDifferenceTwoMatrices(test,golden,channels,epsilon):
def listDifferenceTwoMatrices(test,golden,epsilon,displayResult):
    #print("running listFirstDifferenceTwoMatrices ...")
    foundFirstDifference = False
    maxGolden = 0
    maxTest   = 0
    print("image shape:"+str(golden.shape))
    if(len(golden.shape) > 2):
        channels = golden.shape[2]
    else:
        channels = 1
    for c in range(channels): # channels
        for i in range(golden.shape[0]): # height 
            for j in range(golden.shape[1]): # width 
                if(channels > 1):
                    pGolden = golden[i,j,c]
                    pTest   = test[i,j,c]
                else:
                    pGolden = golden[i,j]
                    pTest   = test[i,j]

                if(abs(int(pGolden) - int(pTest)) > epsilon):
                    print("Mismatch at ("+str(i)+","+str(j)+","+str(c)+") golden: "+str(pGolden)+" test: "+str(pTest))
                    foundFirstDifference = True
                #for k in range(channels):
                #    goldenValue = pGolden[k]
                #    testValue = pTest[k]
                #    if(abs(goldenValue - testValue) > epsilon):
                #        print("Mismatch at ("+str(i)+","+str(j)+") channel: "+str(k)+" golden: "+goldenValue+" test: "+testValue)
                #        foundFirstDifference = True
                if(foundFirstDifference and not displayResult):
                    print("Errors found in width loop. Exiting compare.")
                    return True
            if(foundFirstDifference):
                break
    return foundFirstDifference

#def imageCompare(test,golden,numberOfDifferences,error,listPositionFirstDifference,displayResult,epsilon):
def imageCompare(test,golden,listPositionFirstDifference,displayResult,epsilon):
    #print("running imageCompare ...")
    identical = True
    numberOfDifferences = -1   
    error = -1

    if(len(golden.shape) > 2):
        channels = golden.shape[2]
    else:
        channels = 1

    if test.shape[0] != golden.shape[0] or test.shape[1] != golden.shape[1]:
        # test matching channels and depths too
        identical = False
        print("Error: image sizes do no match, golden: "+golden.shape+" test: "+test.shape)
    else:
        print("Comparing image shape ("+str(golden.shape)+"), channels: "+str(channels))
        #difference = golden
        error = cv2.norm(test,golden,cv2.NORM_L1)
        error = error / (golden.shape[0]*golden.shape[1])
        #np.absdiff(test,golden,difference)
        difference = cv2.absdiff(test,golden)

        numberOfDifferences = 0
        if(channels == 1):
            numberOfDifferences += cv2.countNonZero(difference)
        else: 
            for k in range(channels):
                differenceChannel = cv2.extractChannel(difference,k)
                numberOfDifferences += cv2.countNonZero(differenceChannel)
        if(numberOfDifferences != 0):
            identical = False
        #identical = False

        #if displayResult:
        #    differenceWindowName = "difference"
            # imshow difference in window 

        if listPositionFirstDifference and not identical:
            #print("calling listFirstDifferenceTwoMatrices...")
            identical = not listDifferenceTwoMatrices(test,golden,epsilon,displayResult)

    if identical:
        print("Success! Images match!")
    else:
        print("Failure! Images do no match!")

    #return identical
    return numberOfDifferences,error

def makeMapCircleZoom(width, height, cx, cy, radius, zoom):
    mapY, mapX = np.indices((height,width),dtype=np.float32)
    for (j,i),x in np.ndenumerate(mapX[cy-radius:cy+radius,cx-radius:cx+radius]):
        x = i - radius
        y = j - radius
        i += cx-radius
        j += cy-radius
    mapX[(j,i)] = (cx + x/zoom) if (np.sqrt(x*x+y*y)<radius) else i
    mapY[(j,i)] = (cy + y/zoom) if (np.sqrt(x*x+y*y)<radius) else j 
    return(mapX,mapY)

def colorizeDisparity(disp):
    min,max,minLoc,maxLoc = cv2.minMaxLoc(disp)
    if(min < 0):
        disp_shift = 255.0 - ((disp - min) * (255.0/(max-min)))
    else:
        disp_shift = 255.0 - (disp * (255.0/max))
    norm_disp_shift = disp_shift.astype(np.uint8)
    disp_color = cv2.applyColorMap(norm_disp_shift, cv2.COLORMAP_JET);
    return disp_color
