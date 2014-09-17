#include "csvhelper.h"

void print_record(Record* record){
    //Iterate all records and print the 10 bytes as characters.
    for(int i = 0; i < record->size()-1; i++){
        printf("%.10s,", record->at(i));
    }
    //Print the last variable with no trailing comma.
    printf("%.10s\n", record->at(record->size()-1));
}

void read_records(const char* file, std::vector<Record*>* records){
    FILE* csvfile = fopen(file, "r");
    
    while(!feof(csvfile)){
        //Initialize new record.
        Record* record = new Record;
        int i = 0;
        //Read all attributes into record.
        for(i = 0; i < num_attributes; i++){
            char* attribute = (char*)malloc(attribute_len);
            if(fread(attribute, attribute_len, 1, csvfile) == 0)
                break;
            
            //Skip comma and newline
            fgetc(csvfile);
            record->push_back(attribute);
        }
        
        //If we read the correct number of attributes, add record to records list.
        if(i == num_attributes){
            records->push_back(record);
        }
    }
}