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

C_FLAGS = -Wall -g -Wno-deprecated-declarations ${PACKAGE} -DIS_GTK_3=1

CPP_FILES=$(shell find . -name "*.c")

S_OBJ=$(patsubst %.c, %.o, $(CPP_FILES))

LDFLAGS := `pkg-config --libs ${TGT_GTK}` -lm

#CPPFLAGS := ...
#CXXFLAGS := ...

CPPFLAGS := -g `pkg-config --cflags opencv4`
#CXXFLAGS =

SRCDIR = ./

SRC1 = main_skeleton.c home_page.c params.c my_time.c link_list.c custom_pie_widget.c

OBJ_DIR = ./obj

OBJ11_FILES = $(patsubst $(SRC1),$(OBJ_DIR)/%.o,$(SRC1))

OBJ1_FILES = obj/main_skeleton.o obj/home_page.o obj/params.o obj/my_time.o obj/link_list.o obj/custom_pie_widget.o

OBJ11_RULE = $(SRC11:.c)

OBJ1 =$(SRC1:.c=.o)

$(warning SRC1=${SRC1} )

$(warning OBJ1=${OBJ1} )

$(warning OBJ11_FILES=${OBJ11_FILES} )

all: buildrepo sekai

obj_dir:
	@mkdir -p $(OBJ_DIR)

# %.o: %.c
# 	echo ".c to .o  GENERIC"
# 	$(CC) $(C_FLAGS) -c $^ -o $(OBJ_DIR)/$@

# Had to update this code block to ensure the target matched the output
$(OBJ_DIR)/%.o: %.c | obj_dir		# update: added "| obj_dir"
	echo ".c to .o GENERIC"
	$(CC) $(C_FLAGS) -c $< -o $@


sekai: $(OBJ1_FILES)		# update: Added "_FILES"
	echo " LINK sekai "
	$(CC) -o $@ $(OBJ1_FILES) $(LDFLAGS)


clean:
	rm -fr $(OBJ_DIR)/*.o sekai

tags: FORCE
	${RM} -f tags
	${CTAGS} -R $(SRCDIR)

FORCE: ;

.PHONY: all sekai

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
