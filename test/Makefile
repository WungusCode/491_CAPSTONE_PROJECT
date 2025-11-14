# This Makefile is only for testing
# In Tests folder, will not affect or intefere with main build
# Also, should support both .c and .cpp test files

CC      = gcc
PKG     = pkg-config
CFLAGS  = -Wall -g $(shell $(PKG) --cflags gtk+-3.0 glib-2.0)
LIBS    = $(shell $(PKG) --libs gtk+-3.0 glib-2.0)

# these must match the actual filenames in test/
TESTS = test_login_screen test_create_screen

.PHONY: all run clean

# build all test binaries
all: $(TESTS)

# generic rule: turn foo.c into foo
%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

# run them
run: $(TESTS)
	./test_login_screen
	./test_create_screen

clean:
	rm -f $(TESTS)
