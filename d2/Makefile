.PHONY: clean FORCE

CXX:=clang++
CXXFLAGS:=-O3

main: main.cpp FORCE
	$(CXX) main.cpp $(CXXFLAGS) -o $@

test: main
	./main test.txt

input: main
	./main input.txt

clean:
	-rm main
	-rm *.o
