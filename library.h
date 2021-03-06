#ifndef _DBLIBRARY_
#define _DBLIBRARY_

#include <vector>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef const char* V;

/**
 * A basic record.
 * For the purposes of this assignment it has a fixed size of num_attributes
 * attributes each attribute_len number of bytes.
 */
typedef std::vector<V> Record;

/**
 * Data structure for a Page of records.
 * At the end of the data buffer for a page is the record directory.
 * The record directory is a bitmap that starts at directory_offset.
 * A value of 0 at bit n means the n'th slot is empty, it has no record.
 * A value of 1 at bit n means the n'th slot has a record in it.
 *
 * The directory_offset is calculated when init_fixed_len_page is called.
 * It is completely dependent on page_size and slot_size.
 */
typedef struct {
    /** Data buffer of records and the record directory. */
    void *data;
    /** Number of bytes in data buffer. */
    int page_size;
    /** Fixed size of 1 record slot. */
    int slot_size;
} Page;

/** The fixed number of attributes in a record. */
#define num_attributes 100
/** The fixed size of an attribute in bytes. */
#define attribute_len 10
/** The fixed size of a record. */
#define record_size num_attributes*attribute_len

typedef int PageID;

typedef struct {
    PageID page_id;
    int slot;
} RecordID;

/**
 * A heapfile representation. Grants direct access to heapfile on disk.
 */
typedef struct {
    FILE *file_ptr;
    int page_size;
    int slot_size;
} Heapfile;

/**
 * Compute the number of bytes required to serialize record
 */
int fixed_len_sizeof(Record *record);

/**
 * Serialize the record to a byte array to be stored in buf.
 */
void fixed_len_write(Record *record, void *buf);

/**
 * Deserializes `size` bytes from the buffer, `buf`, and
 * stores the record in `record`.
 */
void fixed_len_read(void *buf, int size, Record *record);

/**
 * Initializes a page using the given slot size
 */
void init_fixed_len_page(Page *page, int page_size, int slot_size);

/**
 * Get the start of the record directory of the given page.
 */
unsigned char* get_directory(Page* page);

/**
 * Release an allocated page from memory.
 */
void free_fixed_len_page(Page* page);

/**
 * Calculates the maximal number of records that fit in a page
 */
int fixed_len_page_capacity(Page *page);

/**
 * Calculates the offset of the directory on a page
 */
int fixed_len_page_directory_offset(Page *page);

/**
 * Return whether or not the given slot is free.
 */
bool is_freeslot(Page *page, int slot);

/**
 * Get the indices of every free slot in the given page.
 */
std::vector<int> fixed_len_page_freeslot_indices(Page* page);

/*
 * Calculate the free space (number of free slots) in the page
 */
int fixed_len_page_freeslots(Page *page);

/**
 * Add a record to the page
 * Returns:
 * record slot offset if successful,
 * -1 if unsuccessful (page full)
 */
int add_fixed_len_page(Page *page, Record *r);

/**
 * Write a record into a given slot.
 */
void write_fixed_len_page(Page *page, int slot, Record *r);

/**
 * Read a record from the page from a given slot.
 */
void read_fixed_len_page(Page *page, int slot, Record *r);

/**
 * Return number of slots in a heap directory given the size of a page.
 */
int number_of_slots_in_heap_directory(int page_size);

/**
 * Initalize a heapfile to use the file and page size given.
 */
void init_heapfile(Heapfile *heapfile, int page_size, FILE *file);

/**
 * Opens a heapfile with a given page size and slot size at path.
 */
int open_heapfile(Heapfile *heap, char *path, int page_size, int slot_size);

/**
 * Allocate another page in the heapfile.  This grows the file by a page.
 */
PageID alloc_page(Heapfile *heapfile, Page* dir_page, int current_page_id);

/**
 * Allocate another page in the heapfile, scanning from the beginning of the
 * file.  This grows the file by a page.
 */
PageID alloc_page(Heapfile *heapfile);

/**
 * Return whether or not the provided pid is out of bounds.
 */
bool out_of_bounds(PageID pid, Heapfile* heap);

/**
 * Return the offset to the begining of a heap directory given it's id and a page size.
 */
int offset_to_directory(int directory_id, int page_size);

/**
 * Return the id of the directory page for the given page id.
 */
int heap_id_of_page(PageID pid, int page_size);

/**
 * Return the offset to a page given it's id and a page size.
 */
int offset_of_pid(PageID pid, int page_size);

/**
 * Read a page into memory
 */
void read_page(Heapfile *heapfile, PageID pid, Page *page);

/**
 * Read a page into memory. Return -1 if the page does not exist.
 */
int try_read_page(Heapfile *heapfile, PageID pid, Page *page);

/**
 * Read a directory page into memory
 */
void read_directory_page(Heapfile *heapfile, PageID directory_id, Page *page);

/**
 * Write a directory page to disk
 */
void write_directory_page(Heapfile *heapfile, PageID directory_id, Page *dir_page);

/**
 * Write a page from memory to disk
 */
void write_page(Page *page, Heapfile *heapfile, PageID pid);

/**
 * Seek to a page on the disk from the given starting page id.
 * If should_be_occupied is true, seeks to the first page with some
 * records stored in it, otherwise seeks to the first page with
 * no records stored in it.
 */
PageID seek_page(Page* page, Page* dir_page, int start_pid, Heapfile* heap, bool should_be_occupied);

/**
 * Iterator to iterate over all records in a heap.
 */
class RecordIterator {
private:
    /** The heap being accessed by this iterator. */
    Heapfile* heap;

    /** The page where the next record is stored. */
    Page* current_page;

    /** Id of current page. */
    PageID current_page_id;

    /** The current record directory slot being checked */
    int current_slot;

    Page* current_directory_page;

    PageID current_directory_page_id;

public:

    RecordIterator(Heapfile *heapfile);
    Record next();
    bool hasNext();
    void printRecords(Record *record);

    ~RecordIterator();
};

#endif
