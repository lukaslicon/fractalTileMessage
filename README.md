# Fractal Tiled Renderer

This project demonstrates asynchronous fractal rendering using C++ with `std::async` and `ThreadSafeQueue`. The project starts with rendering fractal tiles asynchronously and is modified to implement a message queue system for distributing tasks among multiple threads.

## Table of Contents

- [Introduction](#introduction)
- [How It Works](#how-it-works)
- [Project Structure](#project-structure)
- [Setup and Compilation](#setup-and-compilation)
- [Usage](#usage)
- [Bug Fix and Modifications](#bug-fix-and-modifications)
- [License](#license)

## Introduction

The Fractal Tiled Renderer generates a fractal image by dividing the image into tiles. Each tile is rendered asynchronously in parallel. Initially, `std::async` was used to manage parallel rendering of fractal tiles. However, this version refactors the program to utilize a thread-safe message queue with a fixed number of worker threads (e.g., 8 threads) for rendering the tiles.

This project is intended to demonstrate concepts such as multithreading, thread-safe queues, and task distribution in C++.

## How It Works

### Initial Approach with `std::async`

- **FractalMessage Class**: Encapsulates all the state needed to render a "fractal tile."
- **Main Loop**: A nested for-loop in `Main.cpp` creates 16 instances of `FractalMessage`, one for each tile. These messages are passed into `std::async` to asynchronously compute and render each fractal tile.
  
### Modified Approach with `ThreadSafeQueue`

- **ThreadSafeQueue**: A queue to hold `FractalMessage` instances. Messages are added to this queue and worker threads retrieve messages from it to process.
- **Worker Threads**: A number of worker threads (e.g., 8 threads) are spawned, each removing messages from the queue and rendering the corresponding fractal tile.
  
This approach uses a producer-consumer pattern, where the main thread acts as a producer, pushing `FractalMessage` objects to the queue, and worker threads act as consumers, processing these messages.

## Project Structure

- **`FractalMessage.h`**: Defines the `FractalMessage` class, which encapsulates the state for rendering a fractal tile.
- **`ThreadSafeQueue.h`**: Implements a thread-safe queue for managing `FractalMessage` instances.
- **`Main.cpp`**: Contains the main logic of the program, including fractal tile rendering and threading setup.
- **`fractal()`**: The function responsible for generating the fractal image for a specific tile.

## Setup and Compilation

To build and run the project, follow these steps:

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your-username/fractal-tiled-renderer.git
   cd fractal-tiled-renderer
   ```

2. **Compile the Project**:
   Ensure that you have a C++ compiler that supports C++11 or later.
   ```bash
   g++ -std=c++11 -pthread Main.cpp -o fractal_tiled_renderer
   ```

3. **Run the Executable**:
   ```bash
   ./fractal_tiled_renderer
   ```

## Usage

Upon running the program, it will generate a fractal image divided into tiles. Each tile will be rendered by one of the worker threads asynchronously. The resulting fractal image is saved as a bitmap (`output.bmp`).

## Bug Fix and Modifications

### Bug Fix:
An "introduced bug" in the initial version caused some fractal tiles to not be rendered, resulting in an incomplete image. This bug has been fixed in this version, ensuring that all tiles are rendered properly, producing a complete image.

### Modifications:
1. **Replaced `std::async`**: The original implementation using `std::async` has been replaced by a custom thread pool pattern with `ThreadSafeQueue`.
2. **Fixed Bug**: All fractal tiles are now rendered, fixing the issue with missing tiles in the output image.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
