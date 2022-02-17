
TARGET = github_release_tracker_bot
SRC = src
INC = include
BIN = bin

SOURCE = $(wildcard $(SRC)/*.cpp)
OBJECT = $(patsubst %,$(BIN)/%, $(notdir $(SOURCE:.cpp=.o)))

CXX = g++
WARNINGS = -Wall -Wextra -Wunused -pedantic
CXXFLAGS = -O1 -fcompare-debug-second -std=c++14 -I$(INC) 
INCLUDE = -I/usr/local/include
LIBS =  -lcurl -lTgBot -lboost_system -lssl -lcrypto -lpthread

all : $(BIN)/$(TARGET)

RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
NC = \033[1;0m

$(BIN)/$(TARGET) : $(OBJECT)
	@echo "$(RED)Linking...$(NC)"
	$(CXX) -o $@ $(CXXFLAGS1) $(INCLUDE) $(CXXFLAGS)  $^  $(LIBS)
	@echo "$(BLUE)Finished!$(NC)"

$(BIN)/%.o : $(SRC)/%.cpp
	@echo "$(GREEN)Compiling...$(NC)"
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY : help run clean

run: $(BIN)/$(TARGET)
	@echo "$(YELLOW)Running...$(NC)"
	cd $(BIN) && ./$(TARGET)

clean:
	rm -f $(OBJECT) $(BIN)/$(TARGET)

help :
	@echo "src: $(SOURCE)"
	@echo "obj: $(OBJECT)"

