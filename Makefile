# CLAUDE AI GENERATED DECIPHER THIS SHIT LATER

CC := clang
CFLAGS := -std=c99 -Werror -Wall -g -Ofast
TARGET := dvdify
LIBDIR := lib
SRCDIR := src
BUILDDIR := build
LINK_FLAGS := -luser32 -l$(LIBDIR)/raylib.lib
INCLUDES := -I$(SRCDIR) -I$(LIBDIR) -Iextern

# Find all .c files in SRCDIR
SOURCES := $(wildcard $(SRCDIR)/*.c)
DLLS := $(wildcard $(LIBDIR)/*.dll)
LIBS := $(wildcard $(LIBDIR)/*.lib)
# Generate .o file names in BUILDDIR
OBJ_FILES := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))

.PHONY: all clean run

all: $(TARGET)

run: all
	$(BUILDDIR)/$(TARGET).exe

# Rule to create build directory
$(BUILDDIR):
	mkdir $@

copy-dlls: $(DLLS) | $(BUILDDIR)
	@echo "DLLs: $(DLLS)"
	for dll in $(DLLS) ; do \
		@echo "dll name: $(dll)"; \
		cp $$dll $(BUILDDIR)/ ; \
	done

# Rule to compile .c files into .o files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Rule to link object files into the final executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(LINK_FLAGS) $^ -o $(BUILDDIR)/$(TARGET).exe

clean:
	del $(BUILDDIR)
