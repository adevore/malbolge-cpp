CXXFLAGS=-std=c++11 -g
OBJS=main.o value.o
BIN=malbolge


build: $(OBJS)
	$(CXX) -o $(BIN) $(OBJS)

clean:
	$(RM) $(OBJS) $(BIN)
