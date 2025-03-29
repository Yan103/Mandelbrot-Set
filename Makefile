CXX       = g++
SRC_DIR   = src
INC_DIR   = include
OBJ_DIR   = obj
BIN_DIR   = build
SOURCES   = $(wildcard $(SRC_DIR)/*.cpp)
INCLUDES  = $(wildcard $(INC_DIR)/*.h)
OBJECTS   = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
EXECUTABLE= $(BIN_DIR)/a
CXXFLAGS = -I$(INC_DIR) -O3
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	@$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

run: $(EXECUTABLE)
	@./$(EXECUTABLE)

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
