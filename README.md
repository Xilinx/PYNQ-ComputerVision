# PYNQ - Computer Vision

All PYNQ releases ship with the popular [OpenCV](https://opencv.org/) library pre-installed. The PYNQ computer vision overlays enable accelerating OpenCV components in Programmable Logic (PL). These overlays expose a subset Xilinx' [xfOpenCV](https://github.com/Xilinx/xfopencv) library (a part of Xilinx' [reVISION solution](https://www.xilinx.com/products/design-tools/embedded-vision-zone.html)) at the Python level, combined with the support for HDMI input/output. Note that webcam, stream or file based input/output remains available through the pre-installed SW OpenCV.

Currently this package is compatible with PYNQ image v2.3.

![](./block_diagram.png)

A number of prebuilt overlays are available for your PYNQ-enabled board. For example:
  + 2D filter & dilate: accelerated 3x3 2D filter and 3x3 dilate. Open the `filter2d_and_dilate.ipynb` notebook for demo and more details.
  + 2D filter & remap: accelerated 3x3 2D filter and remap. Open the `filter2d_and_remap.ipynb` notebook for demo and more details. 


## Quick Start

To install the computer vision overlay example on your board, open a terminal and run:

   ```bash
   $ sudo pip3 install --upgrade git+https://github.com/Xilinx/PYNQ-ComputerVision.git
   ```
   
After the setup, new Jupyter notebooks will be added under the computer_vision folder, ready to try out, no additional steps are needed. 

## Building Your Own Overlay

When after profiling, the critical OpenCV modules in an application are identified, the generation of a tailored overlay is easily done by specifying those modules as a list in a CMake project file. After that, building the tailored overlay only requires running cmake to generate the Makefiles followed by make to build the overlay.

For detailed instructions:  [Building xfOpenCV Overlays for Pynq: CMake based sds++ cross-compilation](overlays/README.md).

## License

The source for this project is licensed under the [3-Clause BSD License](LICENSE)
