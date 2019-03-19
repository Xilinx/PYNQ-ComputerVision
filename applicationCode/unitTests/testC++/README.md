# Building C++ Unit Tests for PYNQ: CMake based sds++ cross-compilation

## List of Supported Components 

| Image Arithmetic      | Filters       |   Geometric Transform |    Features    | Input Processing	| Analysis 	   | Flow and Depts|
| ---------             | ---------     |   ---------           |    ---------   |    ---------     |  ---------   | --------- 	   |
| absdiff               |  filter2D     |   remap               |      canny     | * split 	 	      |	calcHist     | stereoBM      |  
| bitwise_and           |  dilate    	  |   resize         | cornerHarris |	* merge |	equalizeHist | calcOpticalFlowDenseNonPyrLK  |
| bitwise_or            |   erode       |   warpAffine          |     fast       |	      	        | integral	   |               |
| bitwise_xor           |   medianBlur  |  warpPerspective      |                |			            | meanStdDev	 |               | 
| bitwise_not           |   boxFilter   |                       |                |					        | minMaxLoc		 |               |   
| threshold             |   pyrUp       |                       |                |                  | LUT          |               |
| add                   |   pyrDown     |                       |                |                  |              |               | 
| subtract              |||||||
| multiply              |||||||
| accumulate            |||||||
| accumulateWeighted    |||||||
| accumulateSquare      |||||||
| magnitude             |||||||
| phase                 |||||||

\* These designs require a patch to xfopencv (tag: 2018.2_release) in order to build properly. Please clone https://github.com/denolf/xfopencv.git and checkout the v2018.2Fixed branch to pick up a temporary fix.

## Setup Environment on Host

  + clone [Pynq-ComputerVision](https://github.com/Xilinx/PYNQ-ComputerVision) repository:
    ```commandline
    $ git clone https://github.com/Xilinx/PYNQ-ComputerVision.git <your_pynqcv_folder>
    ``` 
  + clone [xfOpenCV](https://github.com/Xilinx/xfopencv) repository and checkout the correct version (e.g. 2018.2_release):
    ```commandline
    $ git clone https://github.com/Xilinx/xfopencv.git <your_xfopencv_folder>
    $ cd <your_xfopencv_folder>
    $ git checkout <release_number>
    ``` 
  + Prepare the Ultra96 bare platform package in /your_PynqPlatform_folder. 
    + download [ultra96_platform_sysroot_2018.2.tar.gz](https://www.xilinx.com/member/forms/download/xef.html?filename=ultra96_platform_sysroot_2018.2.tar.gz) in /your_PynqPlatform_folder
    + untar the package. This will create an ultra subfolder in /your_PynqPlatform_folder
      ```commandline
      $ tar -zxvf ultra96_platform_sysroot_2018.2.tar.gz
      ```
    + Note that you can adapt /your_PynqPlatform_folder to your preference, but the deepest subfolder should match the platform name, in this case 'ultra'. 
  + set an environmental variable to xfOpenCV
    ```commandline
    $ setenv XFOPENCV_PATH <your_xfopencv_folder>
    ```
  + set up Xilinx SDx tools, version 2018.2 by running its setup script


## Building unit tests
  ### Individual unit tests
  + For an indidivual unit test, navigate to the the test you wish to build (e.g. testXfFilter2D), create a folder there and run cmake.
    ```commandline
    $ cd /<your_pynqcv_folder>/applicationCode/unitTests/testC++/testXfFilter2D
    $ mkdir build; cd build
    $ cmake .. -DCMAKE_TOOLCHAIN_FILE=../../../frameworks/cmakeModules/toolchain_sdx2018.2.cmake -DSDxPlatform=/your_PynqPlatform_folder/ultra -DSDxClockID=1 -DusePL=ON -DnoBitstream=OFF -DnoSDCardImage=ON -DSDxArch=arm64
    ```
  + Note that the clock ID for the platform selects the desired clock frequency for the design. In the case of the Ultra96 platform, the following IDs are available: 0=100MHz, 1=150MHz, 2=250MHz, 3=300MHz.
  + run make for the unit test
    ```commandline
    $ make testSDxXfFiter2D
    ```
  + The bitstream needs to be reformatted to the format used to program the FPGA on the board. This is done by first creating a simple .bif file (conv.bif) as shown below: 
    ```commandline
    all:
    {
         [destination_device = pl] testSDxXfFilter2D.bit
    }
    ```
    Then you run bootgen as follows:
    ```commandline
    $ bootgen -image conv.bif -arch zynqmp -o ./testSDxXfFilter2D.bit.bin -w
    ```
  + Copy the reformatted bitstream and executable to a test folder (for instance ~/proj/test) on your pynq board:
    ```commandline
    $  scp testSDxXfFilter2D.bit.bin xilinx@<pynq-board-ip>:/home/xilinx/proj/test
    $  scp testSDxXfFilter2D xilinx@<pynq-board-ip>:/home/xilinx/proj/test
    ```
  ### Entire unit test suite
  + To build all the available tests, you can run the build script under applicationCode/unitTests/testC++. Be sure to edit the buildUnitTest.py so the platform points to <your platform folder>/ultra.
    ```commandline
    $  cd /<your_pynqcv_folder>/applicationCode/unitTests/testC++/
    $  ./buildUnitTest.py
    ```
  + This creates individual reformatted bitstream and executables for each test and copies them to applicationCode/unitTests/testC++/unitTestFiles. You can then the files onto the board using the same method as in the individual test case
  + You can also pass in the name of text file that newline separate list the units you wish to build as follows:
    ```commandline
    $  ./buildUnitTest.py -l myTestList
    ```
  
    
## Running unit test on Board

  + To run the unit tests, program the FPGA with the target bitstream (.bin) with the sudo command on the board
    ```commandline
    $ cd ~/proj/test
    $ su
    $ echo 0 > /sys/class/fpga_manager/fpga0/flags
    $ cp testSDxXfFilter2D.bit.bin /lib/firmware
    $ echo testSDxXfFilter2D.bit.bin > /sys/class/fpga_manager/fpga0/firmware
    ```
  + Prior to running the tests, the required image files must first be present (e.g. bigBunny_1080.png). These files can be copied to the local directory on the board such as ~/proj/images. Then, the individual tests can be executed in sudo mode or while still under su mode from before.
    ```commandline
    $ sudo ./testSDxXfFilter2D ~/proj/images/bigBunny_1080.png
    ```
    you should see ~140fps measured result.
 
