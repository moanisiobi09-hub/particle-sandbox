# Particle Sandbox

Particle Sandbox is a simple physics simulator written in C++ using SFML. This project is a way to hone development skills while exploring 2D particle collisions, gravity, and multi-directional forces.

![Particle Sandbox screenshot](https://github.com/user-attachments/assets/66adfeec-a54c-4b9c-a934-a269c1161a20)

## Features

- **Particles**: Left-click to add a particle.
- **Fans**: Right-click twice to place a fan object that pushes particles between two points.
- **Gravity and collisions**: Particles respond to gravity and collide realistically.

![Add a fan object](https://github.com/user-attachments/assets/83d0537b-712f-4c85-9df6-370ad470f219)

## Pending Features

- **Shapes**: Add more shapes and custom figures.
- **Emitters**: Place particle emitters on-screen.

## Build Instructions

### Requirements

- Windows
- MinGW/GCC toolchain installed
- `mingw32-make` available
- SFML is included in `external/SFML-3.0.2`

### Build

From the repository root:

```powershell
mingw32-make
```

The executable is created at:

```text
build\bin\sandbox.exe
```

### Run

From the repository root:

```powershell
mingw32-make run
```

This launches:

```text
./build/bin/sandbox.exe
```

### Notes

- Runtime paths are resolved from the working directory at launch time.
- The program expects the font at:

```text
external\liberation-sans\LiberationSans-Regular.ttf
```

### Clean

To remove build outputs:

```powershell
mingw32-make clean
```