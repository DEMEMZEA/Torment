# Compiler and flags
CXX = g++-14
CXXFLAGS = -std=c++26 -Wall -Wextra -Ihpp -O3
LDFLAGS = -ldpp

# Directories
SRC_DIR = cpp
HDR_DIR = hpp
OBJ_DIR = obj

# Target executable
TARGET = bot

# Sources:
SRC_ROOT = bot.cpp
SRC_SUB  = $(wildcard $(SRC_DIR)/*.cpp)
SRC = $(SRC_ROOT) $(SRC_SUB)

# Object files:
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)

# Default rule
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile root-level bot.cpp → obj/bot.o
$(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(OBJ_DIR)/$(SRC_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile cpp/*.cpp → obj/cpp/*.o
$(OBJ_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)/$(SRC_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/$(SRC_DIR)/*.o $(TARGET)

.PHONY: all clean