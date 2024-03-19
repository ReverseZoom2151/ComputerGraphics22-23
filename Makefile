PROJECT_NAME := RedNoise

BUILD_DIR := build

# Define the names of key files
SOURCE_FILES := $(wildcard src/*.cpp) $(wildcard libs/sdw/*.cpp)
HEADER_FILES := $(wildcard src/*.h) $(wildcard libs/sdw/*.h)
# HEADER_FILES := $(wildcard src/*.h) $(wildcard src/Utils.h) $(wildcard libs/sdw/*.h)
OBJECT_FILES := $(patsubst src/%.cpp, $(BUILD_DIR)/%.o, $(SOURCE_FILES))
EXECUTABLE := $(BUILD_DIR)/$(PROJECT_NAME)

# Build settings
COMPILER := clang++
COMPILER_OPTIONS := -c -pipe -Wall -std=c++11 # If you have an older compiler, you might have to use -std=c++0x
# COMPILER_OPTIONS := -c -pipe -Wall -std=c++11 -Ilibs
DEBUG_OPTIONS := -ggdb -g3
LINKER_OPTIONS := -lSDL2

# Set up flags
SDL_COMPILER_FLAGS := $(shell sdl2-config --cflags)
GLM_INCLUDE_DIR := src

default: debug

# Rule to compile and link for use with a debugger (although works fine even if you aren't using a debugger!)
debug: $(OBJECT_FILES)
	$(COMPILER) $(LINKER_OPTIONS) $(DEBUG_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILES) $(SDL_COMPILER_FLAGS)
	./$(EXECUTABLE)

# Rule to help find runtime errors (when you get a segmentation fault)
diagnostic: $(OBJECT_FILES)
	$(COMPILER) $(LINKER_OPTIONS) $(FUSSY_OPTIONS) $(SANITIZER_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILES) $(SDL_COMPILER_FLAGS)
	./$(EXECUTABLE)

# Rule to build for high-performance executable (for manually testing interaction)
speedy: $(OBJECT_FILES)
	$(COMPILER) $(LINKER_OPTIONS) $(SPEEDY_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILES) $(SDL_COMPILER_FLAGS)
	./$(EXECUTABLE)

# Rule to compile and link for final production release
production: $(OBJECT_FILES)
	$(COMPILER) $(LINKER_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILES) $(SDL_COMPILER_FLAGS)
	./$(EXECUTABLE)

# Rule for building object files from source files
$(BUILD_DIR)/%.o: src/%.cpp $(HEADER_FILES)
	@mkdir -p $(BUILD_DIR)
	$(COMPILER) $(COMPILER_OPTIONS) -Ilibs/sdw -I$(GLM_INCLUDE_DIR) -o $@ $< $(SDL_COMPILER_FLAGS)

$(BUILD_DIR)/%.o: libs/sdw/%.cpp $(HEADER_FILES)
	@mkdir -p $(BUILD_DIR)
	$(COMPILER) $(COMPILER_OPTIONS) -Ilibs/sdw -I$(GLM_INCLUDE_DIR) -o $@ $< $(SDL_COMPILER_FLAGS)

# Rule to clean build artifacts
clean:
	rm -rf $(BUILD_DIR)






