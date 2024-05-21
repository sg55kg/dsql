//
// Created by sdollar on 5/21/2024.
//
#include "file_manager.h"

#ifdef _WIN32

// TODO - page manager will handle seeking to locations in a file

int create_file(const char* filename) {
   const HANDLE hFile = CreateFileA(
        filename,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_NEW,
        FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH,
        NULL
        );

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Failed to open file with direct I/O. Error code: %lu\n", GetLastError());
        return -1;
    }

   const int fd = _open_osfhandle((intptr_t)hFile, _O_RDWR);
    if (fd == -1) {
        printf("Failed to convert HANDLE to file descriptor.\n");
        CloseHandle(hFile);
    }

    return fd;
}

int open_file(const char* filename) {
    const HANDLE hFile = CreateFileA(
        filename,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH,
        NULL
        );

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Failed to open file with direct I/O. Error code: %lu\n", GetLastError());
        return -1;
    }

    const int fd = _open_osfhandle((intptr_t)hFile, _O_RDWR);
    if (fd == -1) {
        printf("Failed to convert HANDLE to file descriptor.\n");
        CloseHandle(hFile);
    }

    return fd;
}

int read_file(const int fd, void* buf, const size_t count) {
    if ((size_t)buf % SECTOR_SIZE != 0 || count % SECTOR_SIZE != 0) {
        fprintf(
            stderr,
            "Buffer size and count must be multiples of system sector size (%d bytes).\n",
            SECTOR_SIZE);
        return -1;
    }

    if (read(fd, buf, count) == -1) {
        perror("Failed to read file.\n");
        close(fd);
        return -1;
    }

    return 0;
}

int write_file(const int fd, const void* buf, const size_t count) {
    if ((size_t)buf % SECTOR_SIZE != 0 || count % SECTOR_SIZE != 0) {
        fprintf(
            stderr,
            "Buffer size and count must be multiples of system sector size (%d bytes).\n",
            SECTOR_SIZE);
        return -1;
    }

    if (write(fd, buf, count) == -1) {
        perror("Failed to write to file.\n");
        close(fd);
        return -1;
    }

    return 0;
}

int close_file(const int fd) {
    if (close(fd) == -1) {
        printf("Failed to close file.\n");
        return -1;
    }

    return 0;
}

#else

int create_file(const char* filename) {
    int fd = open(filename, O_RDWR | O_CREAT | O_DIRECT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Failed to create file with direct I/O");
        return -1;
    }
    return fd;
}

int open_file(const char* filename) {
    int fd = open(filename, O_RDWR | O_DIRECT);
    if (fd == -1) {
        perror("Failed to open file with direct I/O");
        return -1;
    }
    return fd;
}

int read_file(int fd, void* buf, size_t count) {
    if ((size_t)buf % SECTOR_SIZE != 0 || count % SECTOR_SIZE != 0) {
        fprintf(stderr, "Buffer and count must be aligned to sector size (%d bytes).\n", SECTOR_SIZE);
        return -1;
    }

    ssize_t bytesRead = read(fd, buf, count);
    if (bytesRead == -1) {
        perror("Failed to read file");
        return -1;
    }

    return 0;
}

int write_file(int fd, const void* buf, size_t count) {
    if ((size_t)buf % SECTOR_SIZE != 0 || count % SECTOR_SIZE != 0) {
        fprintf(stderr, "Buffer and count must be aligned to sector size (%d bytes).\n", SECTOR_SIZE);
        return -1;
    }

    ssize_t bytesWritten = write(fd, buf, count);
    if (bytesWritten == -1) {
        perror("Failed to write to file");
        return -1;
    }

    return 0;
}

int close_file(int fd) {
    if (close(fd) == -1) {
        perror("Failed to close file");
        return -1;
    }

    return 0;
}

#endif