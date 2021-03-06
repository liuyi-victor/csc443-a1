#include "library.h"
#include "csvhelper.h"

/**
 * Insert records from a csv into an existing heap file.
 */
int main(int argc, char** argv){
    if(argc != 4){
        fprintf(stderr, "Usage: %s <heapfile> <csv_file> <page_size>\n", argv[0]);
        return 1;
    }

    //Load records from csv.
    std::vector<Record*> records;
    int error = read_records(argv[2], &records);
    if (error) {
        fprintf(stderr, "Could not read records from file: %s\n", argv[2]);
        return 2;
    }
    if(records.size() == 0){
        fprintf(stderr, "No records in file: %s\n", argv[2]);
        return 3;
    }

    Heapfile* heap = (Heapfile*)malloc(sizeof(Heapfile));
    if (open_heapfile(heap, argv[1], atoi(argv[3]), record_size) != 0) {
        return 4;
    }

    Page* page = (Page*)malloc(sizeof(Page*));
    Page* directory_page = (Page*)malloc(sizeof(Page*));

    PageID current_id = 0; // increments at the end of the loop and from seek
    int records_exhausted = 0;
    while (records_exhausted < records.size()) {
        current_id = seek_page(page, directory_page, current_id, heap, false);

        // if no free page exists, we need to create a new one to insert into.
        if (current_id == -1) {
            current_id = alloc_page(heap);
            read_page(heap, current_id, page);
        }

        // insert a record into each free slot, short-circuit if we run out
        std::vector<int> freeslots = fixed_len_page_freeslot_indices(page);
        for (int i = 0; i < freeslots.size(); i++) {
            write_fixed_len_page(page, freeslots[i], records[records_exhausted]);
            records_exhausted++;

            if (records_exhausted >= records.size())
                break;
        }

        // only write the page if we would have emptied some records into it,
        // which occurs when there are some free record slots.
        if (freeslots.size() > 0) {
            write_page(page, heap, current_id);
        }

        current_id++;
    }

    free(page);
    free(directory_page);

    fclose(heap->file_ptr);
    free(heap);

    return 0;
};
