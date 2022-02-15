
TARGET = github_release_tracker_bot
SRC = src
INC = include
BIN = bin

SOURCE = $(wildcard $(SRC)/*.cpp)
OBJECT = $(patsubst %,$(BIN)/%, $(notdir $(SOURCE:.cpp=.o)))

CXX = g++
WARNINGS = -Wall -Wextra -Wunused -pedantic
CXXFLAGS = -O1 --std=c++14 -I$(INC)
INCLUDE = -I/usr/local/include
LIBS = -lcurl -lTgBot -lboost_system -lssl -lcrypto -lpthread

all : $(BIN)/$(TARGET)


$(BIN)/$(TARGET) : $(OBJECT)
	$(CXX) -o $@ $(CXXFLAGS1) $(INCLUDE) $(CXXFLAGS)  $^  $(LIBS)

$(BIN)/%.o : $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY : help run clean

run: $(BIN)/$(TARGET)
	cd $(BIN) && ./$(TARGET)

clean:
	rm -f $(OBJECT) $(BIN)/$(TARGET)

help :
	@echo "src: $(SOURCE)"
	@echo "obj: $(OBJECT)"

