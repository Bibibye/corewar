include ../../Makefile.common

LDFLAGS += -lfl
OUTFILE := $(BIN_DIR)compiler
SRC_FILES := lexer.c parser.c
OBJ_FILES := $(SRC_FILES:%.c=$(OBJ_DIR)%.o)

.PHONY: all clean mrproper
.PRECIOUS: %.c

all: $(OUTFILE)

$(OUTFILE): $(OBJ_FILES)
	$(CC) $^ -o $@ $(LDFLAGS)

clean:

mrproper: clean
	$(RM) parser.h $(SRC_FILES)
	find -name "*.output" -delete

include $(SRC_FILES:%.c=$(DEPENDS_DIR)%.d)
