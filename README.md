# Klaymore Engine

A custom 2D tile engine made to mimic old platformer games.

## Dependencies

### Runtime packages

- SFML
- PhysFS
- JsonCpp
- Game Music Emu

### Required to build from source

- CMake
- pkg-config

## Building from source

### Installing dependencies

#### Debian/Ubuntu

```sh
sudo apt install build-essential cmake pkg-config libsfml-dev libphysfs-dev libjsoncpp-dev libgme-dev
```

### Compiling

```sh
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4
```
