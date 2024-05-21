//
// Created by sdollar on 5/21/2024.
//

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#ifdef _WIN32
#include <windows.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fileapi.h>
#include <fileextd.h>
#define SECTOR_SIZE 512
#else
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#define SECTOR_SIZE 512 // or 4096 if your filesystem uses that
#endif // _WIN32


int create_file(const char* filename);
int open_file(const char *filename);
int read_file(int fd, void *buf, size_t count);
int write_file(int fd, const void *buf, size_t count);
int close_file(int fd);

#endif //FILE_MANAGER_H
