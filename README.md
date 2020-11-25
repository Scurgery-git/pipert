# PipeRT

Real-Time meets High-Delay: A hybrid scheduling and dataflow framework for DSP applications, developed at [ELTE University](https://www.elte.hu/), Budapest.

## Features

High performance:
  - Multithreaded scheduler with worker pool
  - In-place running in memory, allocations are only made during the setup phase, none are made during the running phase
  - Uses mostly non-blocking algorithms avoiding OS locks

Easy to use:
  - Users don't need to have mutexes or any kind of synchronization primitives
  - Nice, compact OOP API with templates to support custom data types
  - Cross-platform C++11 code tested on gcc and clang

Versatile:
  - Configurable buffers to support low- and high delay units
  - Supports stateless and stateful (accumulate) tasks
  - Supports user controlled threads besides own thread pool to support UI, I/O or special thread affinity
  - Supports circles (feedback) in the pipeline chain
  - Supports multiple entry and exit (even reentry) points for distributed workflows

Measurable (coming soon):
  - Supports measurement-driven development via its own, minimal impact autoprofiling system
  - Has a measurement API which exposes autoprofiling capabilities of the whole pipeline
  - Pluggable measurements: The user can fit any logging mechanism to the profiler

## Overview

The scheduler manages a pool of worker threads to complete work in user-defined nodes on user-defined processing functions. Initially, when the scheduler is instantiated, it enters the setup phase. In this phase, channels should be created that represent the data processing stages in the pipeline using packet processing functions defined in the user's nodes. There can be two types of channels, scheduled or polled. The former channel type's packets are automatically scheduled to be processed by the worker pool, and the latter type's packets can be received manually be the user's own threads. This can be useful when the use case requires a dedicated thread for certain actions, such as when you need to process data using non-thread-safe librares, or when you need to make an exit point from the DSP pipeline to transmit some results out. When all channels are created, the scheduler can be started, at which point packets can start entering the pipeline.

The processing functions defined in the user's nodes accept a packet to be processed as a parameter. After they are done processing a packet, packets can be sent onto the next channel (one that the user node has knowledge of) in the pipeline by acquiring a packet to be filled from that channel and filling it with the processed data. The filled packet is automatically queued for processing in the next channel upon the destruction of the stub beloning to the filled packet. The space in memory belonging to the processed packet is freed up for reuse automatically upon the destruction of the stub belonging to the packet.

At any point after the scheduler was started, it may be stopped, which means it will instruct all worker threads to finish their processing, wait for them to complete their current tasks, and re-enter the setup/preparation phase.

For a more detailed description on how each component in the system works, please check out our documentation, which you can build using Doxygen following the steps outlined below.

## Normal Build for Your Project

You need the following packages installed on your system to build the library:
  - `git` for downloading this repo
  - `cmake` and `ninja-build` to make and use the build system
  - `gcc`, `g++` for compiling with GCC _OR_
  - `clang`, `clang++` and `llvm` for compiling with Clang
  - \[optional\] `doxygen` to generate HTML documentation

Steps to build:
1. Download the repo using `git clone https://github.com/gerazo/pipert.git`
2. Run `./build.sh` to build the static and dynamic library in release mode
3. What you need for your project:
    - an `include` folder
    - the dynamic library, `build/libpipert.so` _OR_
    - the static library, `build/libpipert_static.a`
 
You can find the documentation under `build/docs/index.html`.

## Development Build for Contributors

Besides everything under _Normal Build_, you may also want the following packages:
  - `clang-format` to keep our code formatting conventions
  - `gcovr` to generate text and HTML coverage reports

Steps for development build:
1. Checkout the repo
2. Run `./devbuild.sh` or choose a generator for your IDE by using `./devbuild.sh -G "<yourIDE> - Ninja"` (see `cmake -h`)
3. Find the debug build under `build_debug` and the release build under `build_release` folders
4. In any of these, open your generated IDE project or use `ninja` and `ninja test` targets from the command line
5. Coverage reports will only be generated for the debug build into `build_debug/coverage/coverage.html` and can be regenerated by issuing `ninja coverage` at the command line
6. Documentation will only be generated for the release build by default into `build_release/docs/index.html`, but generation can be forced by issuing `ninja docs` at the command line

To only build in debug or release mode, you should specify both the build directory and the build mode by running `./devbuild.sh -dir <build_debug|build_release> -mode <Debug|Release>`.

## Usage examples

Soon, we will have some tests that also show simple examples of usage.

## Support for different platforms

The project is currently developed on *nix machines. Development on Windows or OS X systems is not supported as of yet, but execution on Windows machines is currently being looked into.

Have fun!