/*
** This file is part of the CSC209 -- Fall 2023 Assignment 2
**
** All of the files in this directory and all subdirectories are:
** Copyright (c) 2023 Demetres Kostas
*/

#include "kar_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_tree(arch_tree_node *root) {
    if (!root)
        return; // base case

    free_tree(root->next_file);    // recursively frees next file nodes
    free_tree(root->dir_contents); // recursively frees child nodes
    free(root);                    // frees memory for the current node
}

// helper function to only extract the filename from the filepath
void only_filename(char *filepath) {
    int total_chars = strlen(filepath);

    if (filepath[total_chars - 1] == '/') {
        filepath[total_chars - 1] = '\0'; // removes the trailing slashes if it exists
    }

    char *filename = strrchr(filepath, '/');

    for (size_t i = 0; filename && i <= strlen(filename); i++) {
        filepath[i] = filename[i + 1]; // for copying filename to the buffer
    }
}

// WARNING: This function assumes that path_buffer is large enough
// helper function to construct valid paths by using the directory and filename
void valid_path(char path_buffer[], char *directory, char *filename) {
    strcpy(path_buffer, directory);

    // adds a slash if the directory does not end with a slash
    if (strlen(path_buffer) > 0) {
        if (path_buffer[strlen(path_buffer) - 1] != '/') {
            strcat(path_buffer, "/");
        }
    }
    strcat(path_buffer, filename);
}

// this helper function helps with buffered read/write
int buffered_read_write(FILE *input, FILE *output, size_t read_remaining, size_t buffer_size) {
    char *buffer = malloc(buffer_size);
    if (!buffer) {
        perror("failed to allocate memory for buffer");
        return -1;
    }

    size_t bytes_read;
    size_t next_read_size = buffer_size < read_remaining ? buffer_size : read_remaining;

    // reads from input stream and write to output stream until theres not more data
    while (read_remaining > 0 && (bytes_read = fread(buffer, 1, next_read_size, input)) > 0) {
        int to_write = bytes_read;

        char *buf_pt = buffer;

        // writes data from buffer to output stream - precaution
        while (to_write > 0) {
            size_t num_written = fwrite(buf_pt, 1, to_write, output);
            to_write -= num_written;
            buf_pt += num_written;
        }

        read_remaining -= bytes_read;
        next_read_size = buffer_size < read_remaining ? buffer_size : read_remaining;
    }

    free(buffer); // free memory which was for buffer
    return 0;
}
