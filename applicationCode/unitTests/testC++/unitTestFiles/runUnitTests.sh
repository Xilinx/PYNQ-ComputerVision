#!/bin/sh

image1="/home/xilinx/proj/images/bigBunny_1080.png"
imageL="/home/xilinx/proj/images/000005_10_L.png" 
imageR="/home/xilinx/proj/images/000005_10_R.png"

resultsLog="resultsLog.txt"
echo `date` > ${resultsLog} 
echo 0 > /sys/class/fpga_manager/fpga0/flags

cp testSDxXfAbsdiff.bit.bin /lib/firmware
echo testSDxXfAbsdiff.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfAbsdiff ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfAbsdiff ${imageL} ${imageR} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfAccumulate.bit.bin /lib/firmware
echo testSDxXfAccumulate.bit.bin > /sys/class/fpga_manager/fpga0/firmware 
echo "\nRunning testSDxXfAccumulate ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfAccumulate ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfAccumulateSquare.bit.bin /lib/firmware
echo testSDxXfAccumulateSquare.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfAccumulateSquare ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfAccumulateSquare ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfAccumulateWeighted.bit.bin /lib/firmware
echo testSDxXfAccumulateWeighted.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfAccumulateWeighted ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfAccumulateWeighted ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfAdd.bit.bin /lib/firmware
echo testSDxXfAdd.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfAdd ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfAdd ${imageL} ${imageR} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfBitwise_and.bit.bin /lib/firmware
echo testSDxXfBitwise_and.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfBitwise_and ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfBitwise_and ${imageL} ${imageR} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfBitwise_not.bit.bin /lib/firmware
echo testSDxXfBitwise_not.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfBitwise_not ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfBitwise_not ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfBitwise_or.bit.bin /lib/firmware
echo testSDxXfBitwise_or.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfBitwise_or ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfBitwise_or ${imageL} ${imageR} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfBitwise_xor.bit.bin /lib/firmware
echo testSDxXfBitwise_xor.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfBitwise_xor ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfBitwise_xor ${imageL} ${imageR} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfCalcHist.bit.bin /lib/firmware
echo testSDxXfCalcHist.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfCalcHist ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfCalcHist ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfDilate.bit.bin /lib/firmware
echo testSDxXfDilate.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfDilate ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfDilate ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfEqualizeHist.bit.bin /lib/firmware
echo testSDxXfEqualizeHist.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfEqualizeHist ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfEqualizeHist ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfFilter2D.bit.bin /lib/firmware
echo testSDxXfFilter2D.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfFilter2D ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfFilter2D ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfMinMaxLoc.bit.bin /lib/firmware
echo testSDxXfMinMaxLoc.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfMinMaxLoc ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfMinMaxLoc ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfRemap.bit.bin /lib/firmware
echo testSDxXfRemap.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfRemap ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfRemap ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfSubtract.bit.bin /lib/firmware
echo testSDxXfSubtract.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfSubtract ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfSubtract ${imageL} ${imageR} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfCalcOpticalFlowDenseNonPyrLK.bit.bin /lib/firmware
echo testSDxXfCalcOpticalFlowDenseNonPyrLK.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfCalcOpticalFlowDenseNonPyrLK ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfCalcOpticalFlowDenseNonPyrLK ${imageL} ${imageR} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfStereoBM.bit.bin /lib/firmware
echo testSDxXfStereoBM.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfStereoBM ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfStereoBM ${imageL} ${imageR} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfBoxFilter.bit.bin /lib/firmware
echo testSDxXfBoxFilter.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfBoxFilter ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfBoxFilter ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfCanny.bit.bin /lib/firmware
echo testSDxXfCanny.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfCanny ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfCanny ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfCornerHarris.bit.bin /lib/firmware
echo testSDxXfCornerHarris.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfCornerHarris ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfCornerHarris ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfErode.bit.bin /lib/firmware
echo testSDxXfErode.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfErode ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfErode ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfFast.bit.bin /lib/firmware
echo testSDxXfFast.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfFast ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfFast ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfInitUndistortRectifyMap.bit.bin /lib/firmware
echo testSDxXfInitUndistortRectifyMap.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfInitUndistorbRectifyMap ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfInitUndistortRectifyMap ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfIntegral.bit.bin /lib/firmware
echo testSDxXfIntegral.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfIntegral ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfIntegral ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfLUT.bit.bin /lib/firmware
echo testSDxXfLUT.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfLUT ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfLUT ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfMagnitude.bit.bin /lib/firmware
echo testSDxXfMagnitude.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfMagnitude ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfMagnitude ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfMeanStdDev.bit.bin /lib/firmware
echo testSDxXfMeanStdDev.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfMeanStdDev ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfMeanStdDev ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfMedianBlur.bit.bin /lib/firmware
echo testSDxXfMedianBlur.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfMedianBlur ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfMedianBlur ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfMerge.bit.bin /lib/firmware
echo testSDxXfMerge.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfMerge ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfMerge ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfPhase.bit.bin /lib/firmware
echo testSDxXfPhase.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfPhase ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfPhase ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfPyrDown.bit.bin /lib/firmware
echo testSDxXfPyrDown.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfPyrDown ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfPyrDown ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfPyrUp.bit.bin /lib/firmware
echo testSDxXfPyrUp.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfPyrUp ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfPyrUp ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfResize.bit.bin /lib/firmware
echo testSDxXfResize.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfResize ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfResize ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfSplit.bit.bin /lib/firmware
echo testSDxXfSplit.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfSplit ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfSplit ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfThreshold.bit.bin /lib/firmware
echo testSDxXfThreshold.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfThreshold ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfThreshold ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfWarpAffine.bit.bin /lib/firmware
echo testSDxXfWarpAffine.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfWarpAffine ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfWarpAffine ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfWarpPerspective.bit.bin /lib/firmware
echo testSDxXfWarpPerspective.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfWarpPerspective ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfWarpPerspective ${image1} 2>&1 | tee -a ${resultsLog} 

cp testSDxXfMultiply.bit.bin /lib/firmware
echo testSDxXfMultiply.bit.bin > /sys/class/fpga_manager/fpga0/firmware
echo "\nRunning testSDxXfMultiply ..." 2>&1 | tee -a ${resultsLog} 
./testSDxXfMultiply ${imageL} ${imageR} 2>&1 | tee -a ${resultsLog} 

