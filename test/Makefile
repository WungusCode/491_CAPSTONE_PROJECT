# This Makefile is only for testing
# In Tests folder, will not affect or intefere with main build

CC      = gcc
PKG     = pkg-config
CFLAGS  = -Wall -g $(shell $(PKG) --cflags gtk+-3.0 glib-2.0)
LIBS    = $(shell $(PKG) --libs gtk+-3.0 glib-2.0 libcurl sqlite3) -lm

TESTS = test_login_screen test_create_screen test_budget_setter

# Include needed  the project .c files
SRC_COMMON = \
    ../login_screen.c \
    ../create_screen.c \
    ../start_screen.c \
    ../indent_print.c \
    ../params.c

SRC_HOME_DEPS = \
    ../home_page.c \
    ../budget.c \
    ../pie_page.c \
    ../transact_page.c \
    ../transaction_list_page.c \
    ../setting_page.c \
    ../transaction_list_view.c \
    ../custom_pie_widget.c \
    ../report_problem_page.c \
    ../transactions.c \
    ../link_list.c \
    ../database/my_sql.c \
    ../my_time.c

# Build all tests
all: $(TESTS)

test_login_screen: test_login_screen.c $(SRC_COMMON)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

test_create_screen: test_create_screen.c $(SRC_COMMON)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

test_budget_setter: test_budget_setter.c $(SRC_COMMON) $(SRC_HOME_DEPS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# run them
run: $(TESTS)
	./test_login_screen
	./test_create_screen
	./test_budget_setter

clean:
	rm -f $(TESTS)
