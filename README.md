# **R-Type**

![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)  
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)
![vcpkg](https://img.shields.io/badge/vcpkg-%23007ACC.svg?style=for-the-badge&logo=visualstudio&logoColor=white)
![GitHub Actions](https://img.shields.io/badge/github%20actions-%232671E5.svg?style=for-the-badge&logo=githubactions&logoColor=white)

---

## **Table of Contents**

- [**R-Type**](#r-type)
  - [**Table of Contents**](#table-of-contents)
  - [**Introduction**](#introduction)
    - [**Main Libraries**](#main-libraries)
    - [**Supported Platforms**](#supported-platforms)
  - [**The Game**](#the-game)
    - [**Story**](#story)
  - [**How to Play ?**](#how-to-play-)
    - [**Prerequisites**](#prerequisites)
    - [**How to install and use VCPKG**](#how-to-install-and-use-vcpkg)
    - [**Launch the Game**](#launch-the-game)
  - [**For developers**](#for-developers)
    - [**Build the program**](#build-the-program)
    - [**Testing program**](#testing-program)
    - [**Documentation**](#documentation)
    - [**Technologies choices**](#technologies-choices)
  - [**For game designers**](#for-game-designers)
    - [**Add content**](#add-content)
  - [**Thank for reading**](#thank-for-reading)
  - [**Authors**](#authors)

---

## **Introduction**

The **Epitech R-Type Project** is a reimagining of the classic shoot’em up **R-Type**, built from scratch using a **custom ECS (Entity Component System)** engine.  
It includes a **multithreaded C++ server** and a **graphical client**, with networking, physics, UI, and persistence handled through modern open-source libraries.

### **Main Libraries**
- **SFML** → 2D graphics rendering, sound, and event management  
- **Asio** → Networking and multithreading  
- **SQLite3** → Lightweight, embedded database for user and score management  
- **Libsodium** → Modern cryptographic library for secure password hashing and token management  
- **vcpkg** → Dependency manager for cross-platform builds  

---

### **Supported Platforms**

✅ **Windows**  
✅ **Linux**

---

## **The Game**

### **Story**

This story begins in 2442, after humanity barely survives the Fourth World War. The Earth, now fully radioactive, forces humankind to look to the stars.  
Five geniuses, known as *"Cold_as_Palmer"*, built ships capable of traveling to **Zelta**, a new habitable planet.

But space is not empty.  
Alien species, energy storms, and cosmic predators now stand between humanity and survival.  
Only you, as the last pilot, can help them defeat the alien forces and reach Zelta safely.

---

## **How to Play ?**

### **Prerequisites**

You need:
- **CMake ≥ 3.17**
- **C++20 compatible compiler**
- **vcpkg** (for dependency management)
- On Linux, make sure you have Libx11 and his relatives installed. It's a library on Linux that provides a client-side interface to the X Window System, also known as the Xlib. On ubuntu, you can install it (apt-get install libx11-dev libxrandr-dev libxcursor-dev libxi-dev libudev-dev libgl1-mesa-dev)

### **How to install and use VCPKG**

```bash
chomd +x installvcpkg.sh
./installvcpkg.sh
```

### **Launch the Game**

After building (see below), simply run:
```bash
./r-type_server [portTCP:int] [portUDP:int] &
./r-type_client [portTCP:int] [portUDP:int] [adress:string]
```

## **For developers**

### **Build the program** 

On Linux : - You may execute the following commands :
You have to install (libx11-dev libxrandr-dev libxcursor-dev libxi-dev libudev-dev libgl1-mesa-dev).

```bash
    chomd +x installvcpkg.sh
    ./installvcpkg.sh
    cd road/to/rtype/repo
    mkdir build
    cd build
    cmake ..
    make
    cd ..
    "./r-type_server"
    "./r-type_client"
    "./r-type_room"
    "./r-type_admin_panel"
```

Before launching the server, you have to define your server secret key for generating robust tokens.
Follow the instructions below : 

```bash
  cd network/RtypeServer/Scripts
  source set_env.sh
```

On Windows : - You may execute the following commands :
```bash
    git clone https://github.com/microsoft/vcpkg.git
    C:\vcpkg
    bootstrap-vcpkg.bat
    setx PATH "$env:PATH;C:\vcpkg"
    .\vcpkg integrate install
    set VCPKG_ROOT=C:\vcpkg
    cd to-rtype-repo
    mkdir build
    cd build
    cmake .. -G "Visual Studio 17 2022" -A x64
    cmake --build . --coonfig Release
    copy ".\Release\r-type_server.exe" .
    copy ".\Release\r-type_client.exe" .
    copy ".\Release\r-type_room.exe" .
    copy ".\Release\r-type_admin_panel.exe" .
```

### **Testing program** 

The project uses Google Tests for testing. They first need to be compiled to be executed.
You can launch the tests by launching the compiled test binaries.

Lauch the regular compilation with CMake
``` bash
rm -rf build && mkdir build && cd build && cmake .. & make
```
The test binaries will be in the build directory. Just lanch them. For example:
``` bash
./registry_tests
```

## **For developers**

### **Add content**

You can customize your experience with R-TYPE by creating your own
custom levels.
To do so, use the level_creator script to generate a new custom
level configuration file. Then, run the game and select, before
launching the room, the custom mode.
Enter the path to your configuration file and there you go, you can play
your custom level until every enemy get destroyed.
See more infos in the documentation, page "Adding Content".

### **Documentation**

🧠 **Code Reference:** Generated with [Doxygen](https://www.doxygen.nl/)  
📘 **Technical Docs:** [Notion Documentation](https://amazing-partridge-567)  
📜 **Communication Protocol:** [docs/protocol.md](./docs/protocol.md)  
💾 **Database Schema:** [docs/database.md](./docs/database.md)

### **Technologies choices**

Please consult the comparative study document [docs/comparatives_case_study.md](./docs/comparatives_case_study.md)

## **Author**

**Aurel DOSSOU KOHI**  
📧 [aurel.dossou-kohi@epitech.eu](mailto:aurel.dossou-kohi@epitech.eu)  
🔗 [GitHub: @aureldsk02](https://github.com/aureldsk02)

---

*This project is a custom implementation of the classic R-Type game, built with a modern ECS architecture.*
