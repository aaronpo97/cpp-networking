# Variables
CXX = g++
CXXFLAGS = -Wall -std=c++11
LDFLAGS = 

SERVER_TARGET = $(BUILD_DIR)/server
CLIENT_TARGET = $(BUILD_DIR)/client

SRC_DIR = src
OBJ_DIR = obj
BUILD_DIR = build

# Source files
SERVER_SRC = $(SRC_DIR)/server.cpp
CLIENT_SRC = $(SRC_DIR)/client.cpp

# Object files
SERVER_OBJ = $(OBJ_DIR)/server.o
CLIENT_OBJ = $(OBJ_DIR)/client.o

# Rules
all: $(BUILD_DIR) $(OBJ_DIR) $(SERVER_TARGET) $(CLIENT_TARGET)

# Rule to create build directories if not exist
$(BUILD_DIR) $(OBJ_DIR):
	@mkdir -p $@

# Rule to create the server executable
$(SERVER_TARGET): $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to create the client executable
$(CLIENT_TARGET): $(CLIENT_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile .cpp files to .o object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean up build artifacts
clean:
	rm -f $(OBJ_DIR)/*.o $(BUILD_DIR)/*

# Run the server
run_server: $(SERVER_TARGET)
	./$(SERVER_TARGET)

# Run the client
run_client: $(CLIENT_TARGET)
	./$(CLIENT_TARGET)

# Phony targets
.PHONY: all clean run_server run_client

# Dependencies
$(SERVER_OBJ): $(SERVER_SRC)
$(CLIENT_OBJ): $(CLIENT_SRC)
