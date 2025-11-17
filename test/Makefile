# This Makefile is only for testing
# In Tests folder, will not affect or intefere with main build

CC      = gcc
PKG     = pkg-config
CFLAGS  = -Wall -g $(shell $(PKG) --cflags gtk+-3.0 glib-2.0)
LIBS    = $(shell $(PKG) --libs gtk+-3.0 glib-2.0)

TESTS = test_login_screen test_create_screen

# Include needed  the project .c files
SRC_COMMON = \
    ../login_screen.c \
    ../create_screen.c \
    ../start_screen.c \
    ../indent_print.c \
    ../params.c

# Build all tests
all: $(TESTS)

test_login_screen: test_login_screen.c $(SRC_COMMON)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

test_create_screen: test_create_screen.c $(SRC_COMMON)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# run them
run: $(TESTS)
	./test_login_screen
	./test_create_screen

clean:
	rm -f $(TESTS)
