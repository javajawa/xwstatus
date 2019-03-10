# The name of the program to build
PROGRAM=a.out

# The list of object files that need to be built
# the values her will be prefixed by the $(BUILD) folder
OBJECTS := logging.o
# The files with global configuration or macros
# the values her will be prefixed by the $(INCLUDE) folder
GLOBALS := layout.h

# The project's folder structure
SRC     := src
INCLUDE := src
BUILD   := build

# Set up the initial program flags
CFLAGS := '-DPROGRAM_NAME="$(PROGRAM)"' -I$(INCLUDE)
# Configure the C standards in use
CFLAGS += -D_POSIX_C_SOURCE=200809L -D_ISOC11_SOURCE
# Configure the error reporting at compilation
CFLAGS += -std=c11 -Wall -Wextra -Werror -pedantic -D_FORTIFY_SOURCE=2

# Libraries required for this project
LDFLAGS :=

# Optional extra config for logging in a threaded program
# you will need to implement get_current_thread_name() for thread-logging
# CFLAGS  += -DTHREADING
# LDFLAGS += -lpthreads

.DEFUALT_TARGET=debug
.PHONY=clean install

# A full build should be fully optimized and have debugging symbol stripped
build: CFLAGS  += -O3 -s
build: LDFLAGS += -O3 -s
build: $(PROGRAM)

# Debug builds have no optimisations, debugging symbols, and #define DEBUG
debug: CFLAGS  += -O0 -g -DDEBUG
debug: LDFLAGS += -O0 -g
debug: $(PROGRAM)

# Convert earlier variables into paths.
OBJECT_O := $(addprefix $(BUILD)/,$(OBJECTS))
GLOBAL_H := $(addprefix $(INCLUDE)/,$(GLOBALS))

$(PROGRAM): $(OBJECT_O)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD)/%.o : $(SRC)/%.c
	@mkdir -vp $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

install:
	cp $(PROGRAM) /usr/local/bin

clean:
	-rm -Rf $(PROGRAM) $(BUILD)
