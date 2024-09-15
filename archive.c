/*
** This file is part of the CSC209 -- Fall 2023 Assignment 2
**
** All of the files in this directory and all subdirectories are:
** Copyright (c) 2023 Demetres Kostas
*/

#include "kar_tree.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_FILENAME_LENGTH 255
#define BUFFER_SIZE 1024
#define FILEPATH_SIZE 512

arch_tree_node *create_tree_node(char *filepath) {

    // get file metadata
    struct stat st;
    if (stat(filepath, &st) == -1) {
        perror("failed to get metadata");
        return NULL;
    }

    // allocates memory to node
    arch_tree_node *new_node = malloc(sizeof(arch_tree_node));
    if (!new_node) {
        perror("memory wasn't allocated to tree node");
        return NULL;
    }

    // copies file name, however, this also assumes that the name of the file is at the end of the entire filepath
    char *filename = strrchr(filepath, '/');
    if (!filename) {
        filename = filepath; // if there isn't a slash '/' then its just the filename
    } else {
        filename++; // to skip the /
    }
    strncpy(new_node->name, filename, FILENAME_MAX - 1);
    new_node->name[FILENAME_MAX - 1] = '\0'; // for null termination

    // setting the other node attributes
    new_node->size = st.st_size;
    new_node->is_directory = S_ISDIR(st.st_mode);
    new_node->next_file = NULL;
    new_node->dir_contents = NULL;

    return new_node;
}

// Forward declaration of the recursive function
void archive_directory(FILE *archive, char *dirpath);

int create_archive(char *archive_name, int num_files, char *files_to_add[num_files]) {

    // creates the archive file
    FILE *archive = fopen(archive_name, "wb");

    // checks if file creation was successful
    if (!archive) {
        perror("error: archive file not opened");
        return 1;
    }

    // to iterate through list of files and directories which will be added to the archive
    for (int i = 0; i < num_files; i++) {
        archive_directory(archive, files_to_add[i]); // utilize archive_directory helper function
    }

    fclose(archive); // closes the archive file (if required)

    return 0;
}

void archive_directory(FILE *archive, char *dirpath) {

    // obtains directory stream
    DIR *dir = opendir(dirpath);

    // ensure opening directory works
    if (!dir) {
        perror("error: directory not opened");
        return;
    }

    // pointer to the direcotory entry
    struct dirent *entry;

    // loops through ecah directory in the current opened directory
    while ((entry = readdir(dir)) != NULL) {

        // skips current and parent dirs
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; // Skip "." and ".."
        }

        // constructs dir path
        char filepath[FILEPATH_SIZE];
        snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, entry->d_name);

        // just in case, preserving original file path
        char original_filepath[FILEPATH_SIZE];
        strncpy(original_filepath, filepath, FILEPATH_SIZE);

        // creating a new tree node.
        arch_tree_node *node = create_tree_node(original_filepath);

        // ensure node creation worked
        if (!node) {
            fprintf(stderr, "error: node not created for %s\n", original_filepath);
            continue; // skips
        }

        // writes metadata header
        if (fwrite(node, sizeof(arch_tree_node), 1, archive) != 1) {
            fprintf(stderr, "error: header not written for %s\n", original_filepath);
            free(node);
            continue;
        }

        // writes the content to archive if it is a file
        if (!node->is_directory) {
            FILE *file = fopen(original_filepath, "rb");

            if (!file) {
                perror("error: failed to open file for reading");
                free(node);
                continue;
            }

            char buffer[BUFFER_SIZE];
            size_t bytes_read;

            // buffered read and write
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
                if (fwrite(buffer, 1, bytes_read, archive) != bytes_read) {
                    fprintf(stderr, "error: content not written for %s\n", original_filepath);
                    fclose(file);
                    free(node);
                    break;
                }
            }

            fclose(file);

        } else {

            // if it's a directory, recurse immediately
            archive_directory(archive, original_filepath);
        }

        free(node);
    }

    closedir(dir);
}
