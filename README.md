# How to build parallel tempering

First, make sure that GMP library is installed (https://gmplib.org/) and GSL

```
git clone --recurse-submodules https://github.com/ot931/ParallelTemperingFinalVersion.git
cd ParallelTemperingFinalVersion
mkdir build && cd build
cmake ..
cmake --build .
```

# How to use

Just run `./metropolis --help`, and read.
