CXX = g++
CFLAGS = -c -fPIC -Wall -g

TARGET = main
SRCS = main.cpp foo.cpp bar.cpp
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
