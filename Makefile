SRCS := $(wildcard src/*.cpp)
BINS := $(SRCS:src/%.cpp=%)

all: clean ${BINS}

%: src/%.cpp
	mkdir -p build
	g++ -fopenmp $< -o build/$@

clean:
	rm -rf build
