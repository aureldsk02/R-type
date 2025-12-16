# 🚀 R-Type

<div align="center">

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)
![Raylib](https://img.shields.io/badge/Raylib-white.svg?style=for-the-badge&logo=raylib&logoColor=black)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)

**A high-performance, multiplayer R-Type clone powered by a custom Entity Component System (ECS).**

</div>

## 📖 About The Project

This project is a faithful recreation of the classic R-Type arcade game, re-engineered from the ground up to demonstrate advanced C++ concepts. It features a fully custom **Entity Component System (ECS)** for modular game logic and a robust **client-server architecture** using UDP for low-latency multiplayer action.

### ✨ Key Features

*   **Custom ECS Engine**: A highly efficient, data-oriented ECS built from scratch (located in `ecs-engine/`).
*   **Real-time Multiplayer**: Authoritative server Architecture using `asio` for UDP networking, handling multiple clients simultaneously.
*   **Cross-Platform**: Builds and runs on Linux, Windows, and macOS.
*   **Modern C++**: Written in C++17 utilizing modern best practices.
*   **Binary Protocol**: Custom-designed binary protocol for optimized network bandwidth.

## 🏗 Architecture

The project follows a clean mono-repo structure:

| Component | Path | Description |
|-----------|------|-------------|
| **ECS Engine** | `ecs-engine/` | Shared core library managing Systems (Render, Physics, Network) and Components. |
| **Server** | `r-type-server/` | Authoritative game state manager. Handles AI, physics, and broadcasts updates. |
| **Client** | `r-type-client/` | Visual frontend using **Raylib**. Interpolates state and captures input. |
| **Shared** | `shared/` | Common network protocols and utility headers. |

## 🛠 Built With

*   [Raylib](https://www.raylib.com/) - Simple and easy-to-use library to enjoy videogames programming.
*   [Asio](https://think-async.com/Asio/) - Cross-platform C++ library for network and low-level I/O programming.
*   [nlohmann/json](https://github.com/nlohmann/json) - JSON for Modern C++.
*   [CMake](https://cmake.org/) - Build system generator.

## 🚀 Getting Started

Follow these steps to get a local copy up and running.

### Prerequisites

*   C++17 capable compiler (GCC, Clang, MSVC)
*   CMake (3.16+)
*   vcpkg (Recommended for dependency management)

### Installation

1.  **Clone the repository**
    ```sh
    git clone https://github.com/your-username/r-type.git
    cd r-type
    ```

2.  **Configure and Build**
    ```sh
    # Unix / Linux / macOS
    mkdir build-new && cd build-new
    cmake ..
    make -j4
    ```

    > **Note**: If using `vcpkg`, ensure you define the toolchain file if it's not automatically detected:
    > `cmake .. -DCMAKE_TOOLCHAIN_FILE=[path/to/vcpkg]/scripts/buildsystems/vcpkg.cmake`

## 🎮 Usage

Once built, the executables will be in `build-new/bin/`.

### 1. Start the Server
The server must be running to accept connections.
```sh
./build-new/bin/r-type-server
```

### 2. Start the Client
Launch one or more clients.
```sh
./build-new/bin/r-type-client
```

### ⌨️ Controls

| Key | Action |
|-----|--------|
| `Arrow Keys` | Move Spaceship |
| `Space` | Shoot |
| `Enter` | Select / Start |

## 🤝 Protocol

Communication relies on a custom binary protocol to ensure speed:
*   **Magic Number**: `0x5678` (Validation)
*   **Packet Structure**: `[Header] + [Payload]`
*   **Types**: Connect, Disconnect, Input(Move/Shoot), StateUpdate.

---
<div align="center">
  <sub>Built with ❤️ by Aurel DSK</sub>
</div>
