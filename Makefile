CC := cc
BUILD := build
CFLAGS := -Wall -Wextra -pedantic
DEBUG_FLAGS := $(CFLAGS) -ggdb -O0

build_folder:
	mkdir -p $(BUILD) 2> /dev/null

clean:
	rm -r $(BUILD)

table_o: build_folder
	$(CC) $(CFLAGS) -c table.c -o $(BUILD)/table.o

table_o_debug: build_folder
	$(CC) $(DEBUG_FLAGS) -c table.c -o $(BUILD)/table.o

build_test_table: table_o_debug
	$(CC) $(DEBUG_FLAGS) test_table.c $(BUILD)/table.o -o $(BUILD)/test_table

test_table: build_test_table
	$(BUILD)/test_table

