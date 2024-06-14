# Introduction

This is a quick implementation of the life game in C++.

It was designed and tested for Linux, specifically Ubuntu 22.04. All instructions
provided in this file expect a similar platform.

# Installation

LifeGame relies on CMake to build the project. If not already installed, please run:

```
sudo apt-get install cmake
```

This repository provides a set of tests that can be ran as a CI process. Tests are handled
by the Catch2 library, which must be downloaded and installed if necessary:

```
git clone https://github.com/catchorg/Catch2.git
cd Catch2
cmake -B build -S . -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX="../catch2_install"
cmake --build build/ --target install
cd ..
rm -r Catch2
```

Build the project:

```
mkdir build
cd ./build
cmake ../ && make
```

All done !

# How to use

Please prepare an input file. Examples can be found in the test folder.
Then run:

```
./LifeGame --input <path_to_your_input_file> --iterations <nb_of_iter> --all
```

**Results will be available in a `lifegame_output` folder that will be created wherever the executable was called from.**

For more information about the different CLI options, please run:

```
./LifeGame --help
```

**WARNING**: If you use the option --all, it will generate as many files as there are iterations,
please make sure to use a reasonable number of iterations to avoid breaking your filesystem.
You will be asked for confirmation when trying to generate more than 20 files.

NB: As a safeguard, the program will stop if the board reaches a size > 256*256 cells.
If required, this can be removed by using the option --noLimit, at the risk of reaching memory overflow.

# CI

CMake builds a test executable as well in order to detect any breaking changes.
To run the tests, please run:

```
./LifeGameTest
```

# Documentation

Code is documented in Doxygen format.
Since this is a very small project, and code is not a library destined to be reused,
HTML documentation was not generated.

# Further improvements:

Here is a list of improvements that could be made:

- Keep track of live cells to only compute changes for those and their neighbours would drastically improve performance for big boards
- Parallelizing the iteration process would also give a big performance leap
- More extensive CI with calls to the final executable would be great
- Rendering outputs as images or GIFs would make visualisation easier
- Allow user to choose destination folder
