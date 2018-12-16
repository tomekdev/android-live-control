OBJS = main.o 

#CROSS_COMPILER = /usr/bin/arm-linux-gnueabihf-
CXX = $(CROSS_COMPILER)g++
CXXFLAGS = -static -fPIE -g

all: livectrl-client

livectrl-client: $(OBJS)
	$(CXX) $(CXXFLAGS) -o livectrl-client $(OBJS)

clean:
	rm -rf $(OBJS) livectrl-client
