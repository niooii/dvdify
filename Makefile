CC := clang
CFLAGS := -std=c99 -Werror -Wall -Wno-unused-variable -Wno-deprecated-declarations -g -O0
TARGET := dvdify
SRCDIR := src
LIBDIR := libs
BUILDDIR := build
LINK_FLAGS := -luser32 -l$(LIBDIR)/window_watcher.lib
INCLUDES := -I$(SRCDIR) -Iincludes -I$(LIBDIR) -Iextern

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

$(BUILDDIR):
	if not exist $(BUILDDIR) mkdir $(BUILDDIR)

copy-dlls: $(DLLS) | $(BUILDDIR)
	@echo DLLs: $(DLLS)
	$(foreach dll,$(DLLS),copy "$(shell cd)\\$(subst /,\,$(dll))" "$(shell cd)\\$(BUILDDIR)\\")
	

# Rule to compile .c files into .o files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Rule to link object files into the final executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(LINK_FLAGS) $^ -o $(BUILDDIR)/$(TARGET).exe

clean:
	if exist $(BUILDDIR) rmdir /s /q $(BUILDDIR)
