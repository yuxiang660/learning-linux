CXX = g++
CFLAGS = -c -fPIC -Wall -std=c++17 -g

TARGET = main
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

all: clean $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^

%.o : %.cpp
	$(CXX) $(CFLAGS) $< -o $@

clean:
	@rm -rf *.o $(TARGET)

.PHONY: clean $(TARGET)

run:
	@./$(TARGET)
