//
// Created by sdollar on 5/22/2024.
//

#include "page-manager.h"
#include "file_manager.h"
#include "util.h"

// TODO - error messages
DataPage* read_data_page(uint64_t offset) {
    const char* filepath = "./data/databases/db1/sc1/users/users.dat"; // TODO lookup ids as stored metadata or pass as args
    const int fd = open_file(filepath);
    if (fd == -1) {
        return NULL;
    }

    void* buffer = allocate_aligned_buffer(SECTOR_SIZE, DATA_PAGE_SIZE);
    if (buffer == NULL) {
        close_file(fd);
        return NULL;
    }

    if (lseek(fd, offset, SEEK_SET) == -1) {
        free(buffer);
        close_file(fd);
        return NULL;
    }

    DataPage* page = malloc(sizeof(DataPage));
    if (read_file(fd, buffer, DATA_PAGE_SIZE) == -1) {
        free(page);
        free(buffer);
        close_file(fd);
        return NULL;
    }

    memcpy(page, buffer, DATA_PAGE_SIZE);

    free(buffer);
    close_file(fd);

    return page;
}

void write_data_page(DataPage* page) {
    // TODO
}

IndexPage* read_index_page(uint64_t key) {

    return NULL;
}

void write_index_page(IndexPage* page) {
    // TODO
}