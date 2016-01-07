# Mathematical Methods of Image Processing
Repository with image processing library for C++  
Wiki with small description in Russian available [here](https://github.com/igormunkin/MMIP/wiki)

## Image I/O
* ```bmpio.h``` - module for reading and writing ```bmp``` files. Implemented for uncompressed 8-bit, 24-bit or 32-bit files.  
* ```imageio.h``` - header with wrappers for unified interaction with image container.  

## Transform
Within this module were implemented following features:  
1. `invert`     Inversion of image pixel values  
2. `mirror {x|y}`       Reflection of the image vertically and horizontally  
3. `rotate {cw|ccw} (angle)`        Rotation of images clockwise and counter-clockwise at any angle  
4. `up_bilinear {s}`        Resampling of image using bilinear interpolation  
5. `up_bicubic {s}`     Resampling of image using bicubic interpolation  
6. `downsample {s}`     Downsampling of image using bilinear interpolation  

**Usage:**  
```%programname% (input_image) (output_image) (command) [parameters...]```  

## Metric
Within this module were implemented following image quality metrics:  
1. `mse`        Mean Square Error (MSE)  
2. `psnr`       Peak Signal-to-Noise Ratio (PSNR)  
3. `ssim`       Structural SIMilarity index (SSIM)  
4. `mssim`      Mean Structural SIMilarity index (MSSIM)  

**Usage:**  
```%programname% (input_image_1) (input_image_2) metric (parameter)```  
