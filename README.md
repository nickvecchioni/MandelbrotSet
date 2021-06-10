# Mandelbrot Fractal Zoomer
![Fractal Zoom](https://www.codingame.com/servlet/fileservlet?id=13848520651734)

Project from 61C that generates the Mandelbrot Fractal and lets you zoom in on areas of the graph iteratively based on given command-line arguments.
Makes extensive use of the Mandelbrot Function, complex numbers, and converting numerical values to different color palettes. 

All main code is written in C.

`colorPalette.c` and `ColorMapInput.c` contain the representation of colors (P3 and P6) and the file I/O for reading/writing color palettes, respectively.

`ComplexNumber.c` hosts a representation of complex numbers and operations, necessary for the Mandelbrot Set.

`Mandelbrot.c` generates the pixel-to-color mapping for a singular frame of the Mandelbrot Set.

`MandelMovie.c` strings together many calls to `Mandelbrot.c`, returning a video of the Mandelbrot Fractal Zoom.

Since this is a class project, only some files are provided as reference (in particular, to prevent blatant cheating.) Hence, this README does not provide instructions for running the program, nor does it go into great detail.
