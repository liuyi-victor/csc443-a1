#include "library.h"
#include "csvhelper.h"

int main(int argc, char** argv){
    
    //Make sure all args given.
    if(argc != 4){
        printf("Usage: write_fixed_len_pages <csv_file> <page_file> <page_size>\n");
        return 1;
    }
    
    //Open page file.
    FILE* page_file = fopen(argv[2], "w");
    
    int page_size = atoi(argv[3]);
    printf("Page size: %d\n", page_size);
    
    //Get records
    std::vector<Record*> records;
    read_records(argv[1], &records);
    
    //Create initial page
    Page* page = (Page*)malloc(sizeof(Page));;
    init_fixed_len_page(page, page_size, record_size);
    
    int page_counter = 1;
    
    //Add records to pages
    for(int i = 0; i < records.size(); i++){
        
        if(add_fixed_len_page(page, records.at(i)) == -1){
            //Write page to file.
            
            //Create new page.
            free(page);
            page = (Page*)malloc(sizeof(Page));;
            init_fixed_len_page(page, page_size, record_size);
            add_fixed_len_page(page, records.at(i));
            page_counter++;
        }
    }
    
    //Write final page to file.
    
    //Print stats
    printf("NUMBER OF RECORDS: %d\n", records.size());
    printf("NUMBER OF PAGES: %d\n", page_counter);
    printf("TIME: %d\n", 0);
}