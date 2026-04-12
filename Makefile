CC := cc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic

TARGET := sorting-visualiser
SRC := main.c

UNAME_S := $(shell uname -s)
RAYLIB_CFLAGS := $(shell pkg-config --cflags raylib 2>/dev/null)
RAYLIB_LIBS := $(shell pkg-config --libs raylib 2>/dev/null)

ifeq ($(strip $(RAYLIB_LIBS)),)
ifeq ($(UNAME_S),Darwin)
RAYLIB_LIBS := -lraylib -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL
else
RAYLIB_LIBS := -lraylib -lm -lpthread -ldl -lrt -lX11
endif
endif

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(RAYLIB_CFLAGS) $(SRC) -o $@ $(RAYLIB_LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
