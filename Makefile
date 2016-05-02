# Makefile

CXXFLAGS += -Wall -Wextra -Werror -pedantic-errors -std=c++17
LDFLAGS += -static-libgcc -static-libstdc++ -mwindows
LIBS += -lole32 -ld2d1

# for debug
CXXFLAGS += -g -O0

CC = g++


SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst %.cpp,%.o,$(SRC))
DEP = $(patsubst %.cpp,%.d,$(SRC))
OUT = cpp_shooting.exe


.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OUT)
	rm -f $(OBJ)
	rm -f $(DEP)


$(OUT): $(OBJ)
	$(LINK.o) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(COMPILE.cc) -o $@ $<

%.d: %.cpp
	$(COMPILE.cc) -MM -MF $@ -MT '$@ $(patsubst %.cpp,%.o,$<)' $<

-include $(DEP)
