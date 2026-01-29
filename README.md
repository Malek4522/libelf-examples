# Libelf Examples

This project contains several examples demonstrating how to use the `libelf` library to interact with ELF (Executable and Linkable Format) files.

## Project Structure

- `prog1.c`: Prints the ELF type (AR archive, ELF object, or data) of a given file.
- `prog2.c`: Displays the ELF Executable Header (Ehdr) details, including machine type, entry point, and section offsets.
- `prog3.c`: Prints the ELF Program Header Table (Phdr), detailing segments and their attributes (load, dynamic, etc.).
- `prog4.c`: Lists ELF sections and dumps the content of the `.shstrtab` (section header string table) section.
- `prog5.c`: Demonstrates how to create a new ELF object from scratch with a data section and a string table.
- `prog6.c`: Iterates through an `ar(1)` archive and prints the names and sizes of its members.

## Prerequisites

To compile and run these examples, you need:
- A C compiler (like `gcc`)
- `libelf` development files
- `libbsd` development files (required by `prog2` and `prog4` for the `vis` function)

On Debian-based systems, you can install these with:
```bash
sudo apt-get install libelf-dev libbsd-dev
```

## Building

You can build all the examples using the provided `Makefile`:

```bash
make
```

To clean up the compiled binaries:

```bash
make clean
```

## Usage

Each program takes a filename as a command-line argument. For example:

```bash
./prog1 my_binary
./prog2 my_binary
./prog3 my_binary
./prog4 my_binary
./prog5 output_elf_file
./prog6 my_archive.a
```

## Educational Insights

- **ELF Library**: The examples use `libelf`, which provides a high-level API for reading and writing ELF files.
- **GElf Interface**: Programs like `prog2` and `prog3` use the `GElf` (Generic ELF) interface, which provides a class-independent way to handle both 32-bit and 64-bit ELF objects.
- **Archive Handling**: `prog6` shows how `libelf` can also handle `ar` archives, treating them as containers of ELF objects.
- **Visualizing Data**: `prog2` and `prog4` use the `vis` function from `libbsd` to safely print non-printable characters, which is useful for inspecting raw ELF data like string tables.