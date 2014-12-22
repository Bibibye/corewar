include ../../Makefile.common

OUTFILE := $(BIN_DIR)corewar
SRC_FILES := $(wildcard *.c)
OBJ_FILES := $(SRC_FILES:%.c=$(OBJ_DIR)%.o)

all: $(OUTFILE)

$(OUTFILE): $(OBJ_FILES)
	$(CC) $^ -o $@ $(LDFLAGS)

clean:

mrproper: clean

include $(SRC_FILES:%.c=$(DEPENDS_DIR)%.d)
