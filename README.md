# archiving-program
 
KAR Archive Program

Introduction

This program implements a simple archive manager called kar (similar to tar) as part of the CSC209 course assignment. It allows users to create archives of files and directories, and extract files from existing archives.


Features

Create archives containing multiple files and directories

Extract files and directories from existing archives

Preserve directory structure within archives

Handle nested directories recursively


Files

kar.c: Main program file

kar_tree.c: Implementation of the archive tree structure

kar_tree.h: Header file for the archive tree structure

archive.c: Functions for creating archives

extract.c: Functions for extracting from archives

Makefile: Compilation instructions


Compilation

To compile the program, use the provided Makefile:
make


Usage

The program supports three main commands:

- Create an archive:

./kar create <archive_name> <file1> <file2> <directory1> ...

- Extract from an archive:

./kar extract <archive_name>

- Display help:

./kar --help


Archive Format

The KAR archive format uses a custom structure to store file and directory information:

Each file or directory is represented by a header (arch_tree_node struct)

Regular files have their contents stored immediately after their header

Directories are followed by the headers and contents of their child files/directories


Implementation Details

Uses a linked list structure to represent the archive contents

Implements recursive traversal for handling nested directories

Buffers file I/O operations for efficiency

Handles error cases such as file not found or permission issues


Error Handling

The program skips files that cannot be opened during archive creation

Exits with an error message if no files were successfully added to the archive

Assumes valid archive format during extraction


Memory Management

The program implements proper memory management, including:

Allocating memory for file/directory structures

Freeing all allocated memory before program termination


Limitations

Does not compress archived files

Cannot modify existing archives (only create new ones or extract)

Does not preserve file metadata such as permissions or timestamps
