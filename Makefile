#
# Copyright (c) 2023 Jozef Nagy
# 
# Use of this source code is governed by an MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT.
#
#

CC := gcc
LD := $(CC)

INTERNAL_CFLAGS := -O2 -g3 -Wall -Wextra -Werror -pedantic -std=c99
INTERNAL_LDFLAGS :=

CFLAGS += $(INTERNAL_CFLAGS)
LDFLAGS += $(INTERNAL_LDFLAGS)

CFILES := $(shell find src -name "*.c")
OBJ := $(CFILES:.c=.o)

DEST := /usr/local/bin

PROGRAM := psh

.PHONY: all
all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	@printf " LD   $@\n"
	@$(LD) $(LDFLAGS) $(OBJ) -o $@

%.o: %.c
	@printf " CC   $^\n"
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: format
format:
	@clang-format -i $(CFILES)

.PHONY: docs
docs:
	@printf " DOXY docs/html\n"
	@mkdir -p docs
	@doxygen
	@printf " DOXY docs/latex\n"
	@make -C docs/latex 1>/dev/null
	@mv docs/latex/refman.pdf docs

.PHONY: install
install: $(PROGRAM)
	@printf " INSTALL\n"
	@sudo install -m 755 $(PROGRAM) $(DEST)

.PHONY: uninstall
uninstall:
	@printf " UNINSTALL\n"
	@sudo rm $(DEST)/$(PROGRAM)

.PHONY: clean
clean:
	@printf " CLEAN\n"
	@rm -rf $(OBJ) $(PROGRAM) docs/
