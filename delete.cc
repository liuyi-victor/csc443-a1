#include "library.h"
#include "csvhelper.h"

#include <stdio.h>

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("Usage: %s <heapfile> <record_id> <page_size>\n", argv[0]);
        return 1;
    }

    // open file from first argument
    Heapfile* heap = (Heapfile*)malloc(sizeof(Heapfile));
    FILE* heap_file = fopen(argv[1], "r+b");
    if (!heap_file) {
        printf("Failed to open heap file: %s\n", argv[1]);
        fclose(heap_file);
        free(heap);
        return 3;
    }

    // read in rest of the arguments
    int record_id = atoi(argv[2]);
    int page_size = atoi(argv[3]);

    // initialize our heapfile and the page we'll be reading and writing to
    heap->page_size = page_size;
    heap->file_ptr = heap_file;

    Page* page = (Page*)malloc(sizeof(Page));
    init_fixed_len_page(page, page_size, record_size);

    // extract info from record id about what page and slot we're operating on
    int records_per_page = fixed_len_page_capacity(page);
    int pid = record_id / records_per_page;
    int slot = record_id % records_per_page;

    if (try_read_page(heap, pid, page) == -1) {
        printf("Record id out of bounds: %d\n", pid);
        free(page);
        fclose(heap_file);
        free(heap);
    }

    //Get byte position of slot in the directory.
    unsigned char* directory_offset = ((unsigned char*)page->data) + fixed_len_page_directory_offset(page);
    directory_offset += slot/8;

    //Update directory, set as free.
    unsigned char directory = (unsigned char)*directory_offset;
    directory &= ~(1 << (slot%8));
    memcpy(directory_offset, &directory, 1);

    write_page(page, heap, pid);

    // and free all our stuff
    fclose(heap_file);
    free(page);
    free(heap);

    return 0;
}
