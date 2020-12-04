# Advent-Of-Code-2020
My solutions to the Advent of Code 2020 challenges

# Notes:
For most days, a C++ version and a Python version is present. So far, each is kept to a single file for simplicity; if this changes, the files for each day will be grouped together, and if necessary, a separate subdirectory for the C++ and Python version may be created.

For my own convenience, an input filename of "input.txt" is assumed on later days. However, the first 2 days get it from the command line. In the C++ versions of these days, TCLAP is used (which was probably overkill, really, but it does make the error checking less tedious). The Python versions have no such dependencies.

C++ versions should be compatible with the C++17 standard or later (well, assuming no breaking changes affecting this code get introduced, of course). These shouldn't be platform dependent, but they're only tested on Windows using MSVC. If something doesn't compile on gcc, feel free to make a bug report about it.

Python versions should be compatible with Python 3.8 or later. It's *probably* compatible with earlier verisons of Python 3, but no promises.

# References
https://adventofcode.com/2020

http://tclap.sourceforge.net/
