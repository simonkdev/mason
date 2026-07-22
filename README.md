# Mason OS

A lightweight hobby operating system kernel written in C and x86 assembly, targeting 32-bit architectures. Mason OS implements core OS concepts including memory management, hardware abstraction, and a basic shell interface.

## 📌 About

### Description
Mason OS is a minimal, educational operating system kernel designed to explore low-level programming, hardware interaction, and OS development fundamentals. It boots via the Multiboot specification, runs in 32-bit protected mode, and provides a text-based interface through VGA output and PS/2 keyboard input.

### Status
**Active Development** – The kernel compiles, boots in QEMU, and provides basic I/O functionality. Core subsystems (memory, keyboard, display) are implemented and tested.

### Features
- **Multiboot-compliant bootloader** for standardized OS loading
- **32-bit protected mode** with Global Descriptor Table (GDT)
- **VGA text mode** display (80x25 text buffer)
- **PS/2 keyboard** input with configurable keymaps
- **Basic I/O drivers** for port-mapped and memory-mapped hardware
- **Built-in shell commands** for kernel interaction
- **ISO generation** via GRUB for bootable media

### Goals
- Serve as a learning platform for OS development
- Implement core kernel functionality incrementally
- Maintain clean, readable, and well-documented code
- Support QEMU emulation for easy testing and development
- Practice using **Nix environments** for real-world projects across multiple machines
- Adhere to a **proper, maintainable structure** for OS projects

## 🏗️ Architecture

### Boot Process
Mason OS follows the **Multiboot specification** for booting:
1. GRUB loads the kernel (`mason` binary) from the ISO
2. Multiboot header in `boot.s` ensures compatibility
3. Control transfers to `_start` in `boot.s`
4. Stack and segments are initialized
5. Kernel entry point `kernel_main()` in `kernel.c` takes over

### Memory Layout
The linker script (`linker.ld`) defines a flat memory model starting at **2MB** to avoid conflicts with BIOS and hardware:

| Section      | Purpose                     |
|--------------|-----------------------------|
| `.multiboot` | Multiboot header            |
| `.text`      | Executable code             |
| `.rodata`    | Read-only data              |
| `.data`      | Initialized data            |
| `.bss`       | Zero-initialized data       |

### Kernel Components
| Component          | Location               | Purpose                          |
|--------------------|------------------------|----------------------------------|
| Bootloader         | `src/boot/boot.s`      | Entry point, initial setup       |
| GDT Setup          | `src/kernel/gdt_load.s`| Segment descriptor tables        |
| Main Kernel        | `src/kernel/kernel.c`  | Core logic, initialization       |
| Helpers            | `src/kernel/helpers.*` | Utility functions                |
| Built-in Commands  | `src/kernel/builtins/` | Shell command implementations     |

### Hardware Support
| Hardware       | Driver Location               | Features                     |
|----------------|-------------------------------|------------------------------|
| VGA Text       | `src/kernel/drivers/io/vgatxt.*` | 80x25 text mode, color support |
| PS/2 Keyboard  | `src/kernel/drivers/io/keyboard.*` | Scancode decoding, keymaps |
| I/O Ports      | `src/kernel/drivers/io/io.*`     | Port-mapped I/O operations   |

## 📁 Project Structure

| Directory   | Purpose |
|-------------|---------|
| `src/` | Core source code |
| `src/boot/` | Bootloader assembly (Multiboot entry point) |
| `src/kernel/` | Main kernel implementation |
| `src/kernel/builtins/` | Built-in shell commands |
| `src/kernel/drivers/` | Hardware drivers |
| `src/kernel/drivers/io/` | I/O drivers (keyboard, VGA text, ports) |
| `isodir/` | ISO image directory structure |
| `build/` | Generated build artifacts |

## ⚙️ Prerequisites

### Toolchain
- **Cross-compiler**: `i686-elf-gcc`, `i686-elf-as`, `i686-elf-ld`
- **Binutils**: `i686-elf-objcopy`, `i686-elf-objdump`

### System Packages
- **GRUB**: `grub-mkrescue` (for ISO generation)
- **QEMU**: `qemu-system-i386` (for emulation)

### Development Environment (Recommended)
- **[Devenv](https://devenv.sh/getting-started/)** (for reproducible builds)

## 🛠️ Building

### Quick Build
```sh
make
```
Compiles the kernel, links all components, and generates a bootable ISO (`mason.iso`).

### Using Devenv
```sh
devenv tasks run mason:assemble_iso
```
Runs the full build (compile + ISO assembly) in the Devenv environment.

### Clean Build
```sh
make clean && make
```
Removes all generated files (`build/` and `isodir/`) and rebuilds from scratch.

### Output Files
- `mason.iso` – Bootable ISO image (root directory)
- `build/mason` – Linked kernel binary
- `build/*.o` – Object files

## ▶️ Running

### QEMU Emulation
```sh
make test
```
Builds the ISO and launches QEMU with:
```sh
qemu-system-i386 -cdrom mason.iso
```

### Quick Test (Pre-built)
```sh
make test-nobuild
```
Runs QEMU directly with the existing kernel binary (skips rebuild):
```sh
qemu-system-i386 -kernel build/mason
```

### Expected Output
- Kernel initializes VGA text mode
- Displays debug messages
- Keyboard input is responsive
- Shell prompt appears (if builtins are enabled)

## 💻 Development Environment

### Devenv Setup
1. Install [Devenv](https://devenv.sh/getting-started/)
2. Enter the development shell:
   ```sh
   devenv shell
   ```
   This provides all required tools (`i686-elf-gcc`, `grub-mkrescue`, `qemu-system-i386`).

### Direnv Integration
With [direnv](https://direnv.net/) installed, `cd` into the project directory to automatically load the environment via `.envrc`.

### Available Tasks
Run specific tasks directly:
```sh
devenv tasks run mason:build      # Compile only
devenv tasks run mason:clean      # Clean build artifacts
devenv tasks run mason:test       # Build + run in QEMU
```

## 📜 License

Mason OS is licensed under the **MIT License**. See [LICENSE](LICENSE) for the full text.
