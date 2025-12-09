O ?= build
CFLAGS ?= -O2 -Wall
LDFLAGS ?=

ifeq ($(origin TARGET_OS),undefined)
    GCC_TARGET := $(shell $(CC) -dumpmachine)

    ifneq (,$(findstring mingw,$(GCC_TARGET)))
        TARGET_OS := win
    else
        TARGET_OS := posix
    endif
endif

SRCS := espfixdep.c

ifeq ($(TARGET_OS), posix)
SRCS += posix.c
NAME := espfixdep
else ifeq ($(TARGET_OS), win)
SRCS += win.c
NAME := espfixdep.exe
else
$(error OS variable not specified or not valid[posix, win])
endif

OBJS := $(SRCS:%.c=$(O)/%.o)

all: $(O)/$(NAME)

$(O):
	mkdir -p $@

$(O)/%.o: %.c | $(O)
		$(CC) $(CFLAGS) -std=gnu99 -MD -MP -o $@ -c $<
		

$(O)/$(NAME): $(OBJS) | $(O)
		$(CC) $(LDFLAGS) -o $@ $^

.PHONY: clean $(O)

clean:
		rm -rf $(O)

-include $(O)/*.d
