CC  = gcc
CXX = g++
RM  = rm

# Directories
OBJDIR = obj

CTAGS=ctags

TGT_GTK = gtk+-3.0

PACKAGE = `pkg-config --cflags $(TGT_GTK)`

# Libraries
LIBS = `pkg-config --libs $(TGT_GTK)` `pkg-config --libs libcurl`
LIBS += -lm -lsqlite3

C_FLAGS = -Wall -g -Wno-deprecated-declarations ${PACKAGE} -DIS_GTK_3=1

C_FILES=$(shell find . -name "*.c")

S_OBJ=$(patsubst %.c, %.o, $(C_FILES))

LDFLAGS := `pkg-config --libs ${TGT_GTK}`

SRCDIR = ./

SRC1 = main_test.c my_sql.c ../link_list.c ../my_time.c ../transactions.c

OBJ_DIR = ./obj

OBJ11_FILES = $(patsubst $(SRC1),$(OBJ_DIR)/%.o,$(SRC1))

OBJ1_FILES = obj/main_test.o obj/my_sql.o link_list.o my_time.o transactions.o

OBJ11_RULE = $(SRC11:.c)

OBJ1 =$(SRC1:.c=.o)

$(warning SRC1=${SRC1} )

$(warning OBJ1=${OBJ1} )

$(warning OBJ11_FILES=${OBJ11_FILES} )

all: buildrepo db_test

obj_dir:
	@mkdir -p $(OBJ_DIR)

%.o: %.c
	echo ".c to .o  GENERIC"
	$(CC) $(C_FLAGS) -c $^ -o $(OBJ_DIR)/$@

db_test: $(OBJ1)
	echo " LINK sekai "
	$(CC) -o $@ $(OBJ1_FILES) $(LDFLAGS) $(LIBS)


clean:
	rm -fr $(OBJ_DIR)/*.o db_test *.o

tags: FORCE
	${RM} -f tags
	${CTAGS} -R $(SRCDIR)

FORCE: ;

buildrepo:
	$(info -->buildrepo: objdir=$(OBJDIR) srcdir=$(SRCDIRS)  )
	@$(call make-repo)

# Create obj directory structure
define make-repo
	mkdir -p $(OBJDIR)
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir; \
	done
endef

.PHONY: all buildrepo  clean

