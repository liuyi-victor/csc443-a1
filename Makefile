# Makefile
CC = g++
RM = rm

all: write_fixed_len_pages read_fixed_len_page csv2heapfile scan select insert update delete csv2colstore select2 select3

library.o: library.cc library.h
	$(CC) -o $@ -c $<

library: library.o

csvhelper.o: csvhelper.cc csvhelper.h
	$(CC) -o $@ -c $<

csvhelper: csvhelper.o

selecthelper.o: selecthelper.cc selecthelper.h library.h
	$(CC) -o $@ -c $<

selecthelper: selecthelper.o

write_fixed_len_pages: write_fixed_len_pages.cc library.o csvhelper.o
	$(CC) -o $@ $< library.o csvhelper.o
	
read_fixed_len_page: read_fixed_len_page.cc library.o csvhelper.o
	$(CC) -o $@ $< library.o csvhelper.o

csv2heapfile: csv2heapfile.cc csvhelper.o library.o
	$(CC) -o $@ $< library.o csvhelper.o

scan: scan.cc library.o csvhelper.o
	$(CC) -o $@ $< library.o csvhelper.o

insert: insert.cc library.o csvhelper.o
	$(CC) -o $@ $< library.o csvhelper.o

update: update.cc library.o csvhelper.o
	$(CC) -o $@ $< library.o csvhelper.o

delete: delete.cc library.o csvhelper.o
	$(CC) -o $@ $< library.o csvhelper.o

select: select.cc library.o csvhelper.o selecthelper.o
	$(CC) -o $@ $< library.o csvhelper.o selecthelper.o

csv2colstore: csv2colstore.cc library.o csvhelper.o
	$(CC) -o $@ $< library.o csvhelper.o

select2: select2.cc library.o selecthelper.o
	$(CC) -o $@ $< library.o selecthelper.o

select3: select3.cc library.o selecthelper.o
	$(CC) -o $@ $< library.o selecthelper.o

clean:
	$(RM) -vf *.o
	$(RM) write_fixed_len_pages read_fixed_len_page csv2heapfile scan insert update delete select csv2colstore select2 select3
