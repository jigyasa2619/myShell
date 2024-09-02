# myShell

myShell is a simple command-line shell program implemented in C++. It provides basic functionality for navigating the file system, listing directory contents, moving, copying, and removing files and directories. The shell also supports some additional options for these commands.

## Table of Contents

- [Features](#features)
- [Usage](#usage)
- [Commands](#commands)
  - [cd](#cd)
  - [ls](#ls)
  - [mv](#mv)
  - [rm](#rm)
  - [cp](#cp)
- [Building and Running](#building-and-running)


## Features

- Change directory (`cd`) with various options.
- List directory contents (`ls`) with options for long format, reverse order, and recursive display.
- Move files (`mv`) with options for interactive mode, wildcard support, backup before overwriting, and only move if the file doesn't exist.
- Remove files and directories (`rm`) with options for interactive mode, forceful removal, and recursive removal.
- Copy files and directories (`cp`) with options for copying special file contents, dereferencing symbolic links, creating hard links, and recursive copy.

## Usage

Simply run the executable, and you'll enter the MyShell command-line environment. You can start entering commands, and MyShell will execute them.

```bash
./myshell
```

To exit MyShell, use the `exit` command.

## Commands

### `cd`

Change directory.

```bash
cd [options] <directory>
```

#### Options:

- `~` or `~username`: Go to home directory or specified user's home directory.
- `.`: Stay in the current directory.
- `dir`: Go to a subdirectory.
- `--help`: Display help message.

### `ls`

List directory contents.

```bash
ls [options] <directory>
```

#### Options:

- `-l`: Show list in long format.
- `-r`: Print list in reverse order.
- `-R`: Display content of sub-directories also.
- `~`: Give the contents of the home directory.
- `../`: Give the contents of the parent directory.
- `--help`: Display help message.

### `mv`

Move files.

```bash
mv [options] <source> <destination>
```

#### Options:

- `-i`: Ask for permission to overwrite.
- `*`: Move multiple files to a specific directory.
- `--suffix`: Take backup before overwriting.
- `-u`: Only move those files that don't exist.
- `--help`: Display help message.

### `rm`

Remove files and directories.

```bash
rm [options] <file/directory>
```

#### Options:

- `-r, -R`: Remove directory recursively.
- `-i`: Remove file interactively.
- `-rf`: Remove directory forcefully.
- `-f`: Force removal, ignores non-existent files and overrides prompts.
- `--help`: Display help message.

### `cp`

Copy files and directories.

```bash
cp [options] <source> <destination>
```

#### Options:

- `--copy-contents`: Copy special file contents when recursive.
- `-d`: Equivalent to --no-dereference --preserve=links.
- `--link, -l`: Specify hard link files rather than copying.
- `--recursive, -r, -R`: Recursively copy directories.
- `--help`: Display help message.

## Building and Running

1. Clone the repository:

```bash
git clone https://github.com/your-username/myshell.git
cd myshell
```

2. Compile the code:

```bash
g++ myshell.cpp -o myshell -std=c++17 -lstdc++fs
```

3. Run MyShell:

```bash
./myshell
```

## Contributions

Jigyasa Saini
B.E. Information Science
NIE Mysore



