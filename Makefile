PROGRAM=tower

# Add your objs to generate in OBJS var
OBJS=src/main.o src/map.o src/entities/entities.o src/entities/enemies/enemies.o src/entities/towers/towers.o src/game.o src/player.o src/scene/scene.o src/scoreboard.o src/soundAndMusic.o 

CXX?=g++
TARGET?=$(shell $(CXX) -dumpmachine)

OPTIM_FLAGS?=-O0
CXXFLAGS=$(OPTIM_FLAGS) -g -Wall -Wextra -Wno-unused-parameter
CPPFLAGS=-Iinclude -Ithird_party/include -MMD
LDFLAGS=-Lthird_party/libs-$(TARGET)
LDLIBS=-lraylib

ifeq ($(TARGET),x86_64-linux-gnu)
LDLIBS+=-ldl -lpthread -lm
else ifeq ($(TARGET),x86_64-pc-cygwin)
LDLIBS+=-lgdi32
else ifeq ($(TARGET),wasm32-unknown-emscripten)
OPTIM_FLAGS=-Os
LDFLAGS+=-s ASYNCIFY
LDFLAGS+=-s USE_GLFW=3
LDFLAGS+=--preload-file assets
EXT=.html
endif

DEPS=$(OBJS:.o=.d)

.PHONY: all clean

all: $(PROGRAM)$(EXT)

-include $(DEPS)

%.o: %.c
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

$(PROGRAM)$(EXT): $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

build.tar.gz: $(PROGRAM) $(wildcard assets/*)
	tar czf build.tar.gz $(PROGRAM) assets

host: $(PROGRAM)$(EXT)
	python3 -m http.server 8000

clean:
	rm -f $(OBJS) $(DEPS) $(PROGRAM) build.tar.gz
	rm -f $(PROGRAM).html $(PROGRAM).js $(PROGRAM).data $(PROGRAM).wasm
 