include Makefile.common

.PHONY: all compiler corewar clean mrproper

all: compiler corewar

compiler:
	$(MAKE) -C $(COMPILER_DIR)

corewar:
	$(MAKE) -C $(COREWAR_DIR)

etags:
	etags `find -name *.c && find -name *.h`

ctags:
	ctags `find -name *.c && find -name *.h`

clean:
	$(MAKE) -C $(COMPILER_DIR) $@
	$(MAKE) -C $(COREWAR_DIR) $@
	find -name "*.o" -delete
	find -name "*~" -delete
	find -name "\#*" -delete
	find -name core -delete

mrproper: clean
	$(MAKE) -C $(COMPILER_DIR) $@
	$(MAKE) -C $(COREWAR_DIR) $@
	$(RM) $(BIN_DIR)*
	find -name "*.d" -delete
	$(RM) TAGS
