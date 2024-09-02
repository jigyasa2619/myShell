CC = g++
CFLAGS = -std=c++17

DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O3

SOURCES = myShell.cpp
DEBUG_TARGETS = $(SOURCES:.cpp=_debug)
RELEASE_TARGETS = $(SOURCES:.cpp=_release)

.PHONY: all debug release clean

all: debug release

debug: $(DEBUG_TARGETS)

release: $(RELEASE_TARGETS)

%_debug: %.cpp
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $< -o $@

%_release: %.cpp
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $< -o $@

clean:
	rm -f $(DEBUG_TARGETS) $(RELEASE_TARGETS)