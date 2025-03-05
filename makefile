CXX = g++
CXXFLAGS = -std=c++17 -pthread -lsfml-graphics -lsfml-window -lsfml-system
SRC = main.cpp grid.cpp dice.cpp player.cpp utility.cpp messages.cpp WindowManager.cpp GameLogic.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = ludo_game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

