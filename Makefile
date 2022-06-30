SRC_DIR := src
BUILD_DIR := build
INCLUDE_DIR := include
CC := g++
CFLAGS := -lglfw -lGL

EXECUTABLE := main

CPP_SOURCES :=  $(wildcard $(SRC_DIR)/*.cpp) 
C_SOURCES := $(wildcard $(SRC_DIR)/*.c) 

CPP_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SOURCES))
C_OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))

OBJ_FILES := $(C_OBJECTS) $(CPP_OBJECTS)

build:$(EXECUTABLE)

run: $(EXECUTABLE)
	$(BUILD_DIR)/$(EXECUTABLE) 

$(EXECUTABLE):$(OBJ_FILES)
	$(CC) $(OBJ_FILES)  -I$(INCLUDE_DIR) -o $(BUILD_DIR)/$(EXECUTABLE)  $(CFLAGS)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC)  -I$(INCLUDE_DIR) -c $< -o $@  $(CFLAGS)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC)  -I$(INCLUDE_DIR) -c $< -o $@  $(CFLAGS)

clean:
	rm -r $(BUILD_DIR)/*



