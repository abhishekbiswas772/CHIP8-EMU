# CHIP8 Emulator

A CHIP8 emulator written in C that implements the CHIP8 instruction set and provides a simple window-based display interface.

## Features

- Complete CHIP8 instruction set implementation
- Window-based display using Windows API
- Keyboard input support
- Memory management
- Stack operations
- Timer support (delay and sound)
- Sprite rendering

## Prerequisites

- MinGW-w64 (GCC compiler for Windows)
- Make (optional, for using Makefile)

## Building on Windows

1. Install MinGW-w64:
   - Download MinGW-w64 from [MinGW-w64 Downloads](https://github.com/niXman/mingw-builds-binaries/releases)
   - Extract to a directory (e.g., `C:\mingw64`)
   - Add the `bin` directory to your system's PATH

2. Clone the repository:
```bash
git clone https://github.com/yourusername/CHIP8-EMU.git
cd CHIP8-EMU
```

3. Compile the project:
```bash
mingw32-make
```

## Running the Emulator

After compilation, you can run the emulator with a CHIP8 ROM file:
```bash
./main.exe path/to/your/rom.ch8
```

## Project Structure

```
CHIP8-EMU/
├── include/           # Header files
│   ├── chip8.h       # Main CHIP8 structure and functions
│   ├── chip8keyboard.h
│   ├── chip8memory.h
│   ├── chip8register.h
│   ├── chip8screen.h
│   ├── chip8stack.h
│   └── config.h      # Configuration constants
├── src/              # Source files
│   ├── chip8.c       # Main CHIP8 implementation
│   ├── chip8keyboard.c
│   ├── chip8memory.c
│   ├── chip8screen.c
│   └── chip8stack.c
└── README.md
```

## License

This project is open source and available under the MIT License.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request. 
