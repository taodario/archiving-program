/*
** This file is part of the CSC209 -- Fall 2023 Assignment 2
**
** All of the files in this directory and all subdirectories are:
** Copyright (c) 2023 Demetres Kostas
*/

#include "kar_tree.h"
#include <stdio.h>
#include <string.h>

void print_help() {
    printf("Usage: kar [COMMAND] [ARCHIVE]\n");
    printf("Creates or extracts files from the kar-formatted ARCHIVE.\n");
    printf("\n");
    printf("COMMAND may be one of the following:\n");
    printf("    create [ARCHIVE] [FILES/DIRS]\n");
    printf("    extract [ARCHIVE]\n");
    printf("    --help\n");
    printf("\n");
    printf("create:\n");
    printf("    Creates an archive with the name specified in [ARCHIVE] consisting of the listed [FILES] which can include directories. Paths to the files are not preserved, each listed file is part of the top-level of the archive. If [ARCHIVE] already exists, it is overwritten. Directories are added recursively, such that all files within the directory are added to the archive.\n");
    printf("\n");
    printf("extract:\n");
    printf("    Extracts the files from the [ARCHIVE] file into the current directory.\n");
    printf("\n");
    printf("--help:\n");
    printf("    Prints this message and exits.\n");
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "error: please provide command\n");
        print_help(); // display the help stuff
        return 1;
    }

    if (strcmp(argv[1], "create") == 0) { // compares first argument to create
        if (argc < 4) {
            fprintf(stderr, "error: please provide the arguments for create\n");
            print_help();
            return 1;
        }
        return create_archive(argv[2], argc - 3, &argv[3]);
    } else if (strcmp(argv[1], "extract") == 0) { // compares first argument to extract
        if (argc != 3) {
            fprintf(stderr, "error: not enough arguments for extract\n");
            print_help();
            return 1;
        }
        return extract_archive(argv[2]);
    } else if (strcmp(argv[1], "--help") == 0) { // compares first argument to help
        print_help();
    } else { // unrecognized command
        fprintf(stderr, "error: command isn't valid '%s'\n", argv[1]);
        print_help();
        return 1;
    }

    return 0;
}