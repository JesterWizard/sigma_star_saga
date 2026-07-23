# GBA ROM header (from Sigma Star Saga USA/Europe; verify against your baserom)
TITLE      := SIGMA STAR
GAME_CODE  := BS3E
MAKER_CODE := 01
REVISION   := 0

FILE_NAME  := sigma_star_saga
BUILD_DIR  := build
OBJ_DIR    := $(BUILD_DIR)/sss

ROM  := $(FILE_NAME).gba
ELF  := $(FILE_NAME).elf
MAP  := $(FILE_NAME).map

# Toolchain (devkitARM or system arm-none-eabi-*)
TOOLCHAIN := $(DEVKITARM)
ifneq (,$(TOOLCHAIN))
  ifneq ($(wildcard $(TOOLCHAIN)/bin),)
    export PATH := $(TOOLCHAIN)/bin:$(PATH)
  endif
endif

PREFIX  := arm-none-eabi-
OBJCOPY := $(PREFIX)objcopy
OBJDUMP := $(PREFIX)objdump
AS      := $(PREFIX)as
LD      := $(PREFIX)ld
CPP     := $(CC) -E

EXE :=
ifeq ($(OS),Windows_NT)
  EXE := .exe
endif

MODERN  ?= 0
COMPARE ?= 0

ifeq (compare,$(MAKECMDGOALS))
  COMPARE := 1
endif
ifeq (modern,$(MAKECMDGOALS))
  MODERN := 1
endif

ASFLAGS   := -mcpu=arm7tdmi --defsym MODERN=$(MODERN)
INCLUDE_DIRS := include
CPPFLAGS  := $(INCLUDE_DIRS:%=-iquote %) -Wno-trigraphs -DMODERN=$(MODERN)

O_LEVEL ?= 2

# Libraries are only needed once C objects are linked.
LIB :=
ifeq ($(MODERN),0)
  CPPFLAGS += -I tools/agbcc/include -I tools/agbcc -nostdinc -undef -std=gnu89
  CC1 := tools/agbcc/bin/agbcc$(EXE)
  override CFLAGS += -mthumb-interwork -Wimplicit -Wparentheses -Werror -O$(O_LEVEL) -fhex-asm
  ifneq ($(strip $(C_SRCS)),)
    LIBPATH := -L ../../tools/agbcc/lib
    LIB := $(LIBPATH) -lgcc -lc
  endif
else
  MODERNCC := $(PREFIX)gcc
  PATH_MODERNCC := PATH="$(PATH)" $(MODERNCC)
  CC1 := $(shell $(PATH_MODERNCC) --print-prog-name=cc1) -quiet
  override CFLAGS += -mthumb -mthumb-interwork -O$(O_LEVEL) -mabi=apcs-gnu -mtune=arm7tdmi -march=armv4t -fno-toplevel-reorder
  ifneq ($(strip $(C_SRCS)),)
    LIBPATH := -L "$(dir $(shell $(PATH_MODERNCC) -mthumb -print-file-name=libgcc.a))" \
               -L "$(dir $(shell $(PATH_MODERNCC) -mthumb -print-file-name=libnosys.a))" \
               -L "$(dir $(shell $(PATH_MODERNCC) -mthumb -print-file-name=libc.a))"
    LIB := $(LIBPATH) -lc -lnosys -lgcc
  endif
endif

TOOLS_DIR := tools
SCANINC   := $(TOOLS_DIR)/scaninc/scaninc$(EXE)
PREPROC   := $(TOOLS_DIR)/preproc/preproc$(EXE)
FIX       := $(TOOLS_DIR)/gbafix/gbafix$(EXE)
RAMSCRGEN := $(TOOLS_DIR)/ramscrgen/ramscrgen$(EXE)

SHA1 := $(shell { command -v sha1sum || command -v shasum; } 2>/dev/null) -c

SHELL := bash -o pipefail

.SUFFIXES:
.SECONDARY:
.DELETE_ON_ERROR:

.PHONY: all rom modern compare clean tidy tools check-baserom

all: rom

C_SUBDIR = src
ASM_SUBDIR = asm
DATA_ASM_SUBDIR = data

C_BUILDDIR = $(OBJ_DIR)/$(C_SUBDIR)
ASM_BUILDDIR = $(OBJ_DIR)/$(ASM_SUBDIR)
DATA_ASM_BUILDDIR = $(OBJ_DIR)/$(DATA_ASM_SUBDIR)

# Scaffold stage: only the baserom blob is linked.
# Uncomment C_SRCS / DATA_ASM_SRCS as you start matching code.
C_SRCS :=
# C_SRCS := $(wildcard $(C_SUBDIR)/*.c)
ASM_SRCS := $(ASM_SUBDIR)/rom.s
DATA_ASM_SRCS :=

C_OBJS := $(patsubst $(C_SUBDIR)/%.c,$(C_BUILDDIR)/%.o,$(C_SRCS))
ASM_OBJS := $(patsubst $(ASM_SUBDIR)/%.s,$(ASM_BUILDDIR)/%.o,$(ASM_SRCS))
DATA_ASM_OBJS := $(patsubst $(DATA_ASM_SUBDIR)/%.s,$(DATA_ASM_BUILDDIR)/%.o,$(DATA_ASM_SRCS))

OBJS := $(C_OBJS) $(ASM_OBJS) $(DATA_ASM_OBJS)
OBJS_REL := $(patsubst $(OBJ_DIR)/%,%,$(OBJS))

SUBDIRS := $(sort $(dir $(OBJS)))
$(shell mkdir -p $(SUBDIRS))

modern: all
compare: all

rom: check-baserom $(ROM)
ifeq ($(COMPARE),1)
	@$(SHA1) rom.sha1
endif

check-baserom:
	@test -f baserom.gba || { \
	  echo "error: baserom.gba not found."; \
	  echo "Place a clean Sigma Star Saga (USA/Europe) ROM named baserom.gba in the repo root."; \
	  exit 1; \
	}

tools:
	@$(MAKE) -C tools

clean: tidy
	@$(MAKE) clean -C tools

tidy:
	rm -f $(ROM) $(ELF) $(MAP)
	rm -rf $(BUILD_DIR)

$(ASM_BUILDDIR)/%.o: $(ASM_SUBDIR)/%.s
	$(AS) $(ASFLAGS) -I . -o $@ $<

$(DATA_ASM_BUILDDIR)/%.o: $(DATA_ASM_SUBDIR)/%.s
	$(AS) $(ASFLAGS) -I . -o $@ $<

$(C_BUILDDIR)/%.o: $(C_SUBDIR)/%.c
ifeq ($(MODERN),0)
	@test -x $(CC1) || { echo "error: agbcc missing. See INSTALL.md"; exit 1; }
endif
	@$(CPP) $(CPPFLAGS) $< | $(CC1) $(CFLAGS) -o $(C_BUILDDIR)/$*.s
	@echo -e ".text\n\t.align\t2, 0\n" >> $(C_BUILDDIR)/$*.s
	$(AS) $(ASFLAGS) -o $@ $(C_BUILDDIR)/$*.s

LD_SCRIPT := ld_script.ld
LDFLAGS = -Map ../../$(MAP)

$(ELF): $(LD_SCRIPT) $(OBJS)
	cd $(OBJ_DIR) && $(LD) $(LDFLAGS) -T ../../$(LD_SCRIPT) -o ../../$@ $(OBJS_REL) $(LIB)
ifneq ($(wildcard $(FIX)),)
	$(FIX) $@ -t"$(TITLE)" -c$(GAME_CODE) -m$(MAKER_CODE) -r$(REVISION) --silent
endif

$(ROM): $(ELF)
	$(OBJCOPY) -O binary --gap-fill 0xFF $< $@
ifneq ($(wildcard $(FIX)),)
	$(FIX) $@ -p --silent
endif
	@# Scaffold: ROM is a raw baserom image; trim ELF metadata padding if any
	@truncate -s 8388608 $@
