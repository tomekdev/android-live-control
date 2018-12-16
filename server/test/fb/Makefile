OBJS = main.o

CROSS_COMPILER = /usr/bin/arm-linux-gnueabihf-
CXX = $(CROSS_COMPILER)g++
CXXFLAGS = -static -fPIE -g

all: fbtest

fbtest: $(OBJS)
	$(CXX) $(CXXFLAGS) -o fbtest $(OBJS)

clean:
	rm -rf $(OBJS) fbtest
