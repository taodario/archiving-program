/*
** This file is part of the CSC209 -- Fall 2023 Assignment 2
**
** All of the files in this directory and all subdirectories are:
** Copyright (c) 2023 Demetres Kostas
*/

#include "kar_tree.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

arch_tree_node *read_node(FILE *archive) {
    // allocate memory to new node
    arch_tree_node *node = malloc(sizeof(arch_tree_node));

    if (!node) {
        perror("memory not allocated to node due to an error");
        return NULL;
    }

    // read node info from archive
    if (fread(node, sizeof(arch_tree_node), 1, archive) != 1) {
        free(node);
        return NULL;
    }

    return node;
}

void extract_directory(FILE *archive, arch_tree_node **current, const char *path_prefix) {

    while (1) {
        arch_tree_node *node = read_node(archive); // read node from archive

        // makes sure end of archive isn't reached
        if (!node)
            break;

        // Construct the full path for the node
        char full_path[1024]; // Assuming a maximum path length
        snprintf(full_path, sizeof(full_path), "%s%s", path_prefix, node->name);

        printf("full path: %s\n", full_path); // for debugging purposes

        // adds node to LL
        if (*current) {
            (*current)->next_file = node;
            current = &(*current)->next_file;
        } else {
            *current = node;
        }

        printf("node name: %s, is_directory: %d\n", node->name, node->is_directory); // for debugging purposes

        // makes sure node reps a directorys
        if (node->is_directory) {
            // Create directory if it doesn't exist
            if (mkdir(full_path, 0755) != -1) { // Using full_path to create the directory
                char new_path_prefix[1024];
                snprintf(new_path_prefix, sizeof(new_path_prefix), "%s/", full_path);
                extract_directory(archive, &(node->dir_contents), new_path_prefix);
            } else {
                perror("failed to create directory");
                free(node);
            }
        } else {
            // dynamically allocate to buffer
            char *buffer = malloc(node->size);
            if (!buffer) {
                perror("failed to allocate memory for buffer");
                continue;
            }

            if (fread(buffer, 1, node->size, archive) != node->size) {
                perror("failed to read from archive");
                free(buffer);
                continue;
            }

            FILE *file = fopen(full_path, "wb");

            if (!file) {
                perror("didn't create file");
                free(buffer);
                continue;
            }

            if (fwrite(buffer, 1, node->size, file) != node->size) {
                perror("file not wrote");
            }

            fclose(file);
            free(buffer);
        }
    }
}

int extract_archive(char *archive_name) {
    FILE *archive = fopen(archive_name, "rb");

    if (!archive) {
        perror("archive wasn't opened.\n");
        return 1;
    }

    kar_tree root;
    root.root = NULL;

    extract_directory(archive, &root.root, "");

    free_tree(root.root);
    fclose(archive);
    return 0;
}