OBJS = main.o \
       socket.o \
       uinput.o

#CROSS_COMPILER = /usr/bin/arm-linux-gnueabihf-
CXX = $(CROSS_COMPILER)g++
CXXFLAGS = -static -fPIE -g

all: livectrl-server

livectrl-server: $(OBJS)
	$(CXX) $(CXXFLAGS) -o livectrl-server $(OBJS)

clean:
	rm -rf $(OBJS) livectrl-server
