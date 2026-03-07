CC = wcl
CFLAGS = -bt=dos -d2 -ml

SRC = src/*.c
TARGET = build/enforcer.exe

.PHONY: all clean

all: $(TARGET) build/intro.dat build/dosbox.conf

$(TARGET):
	mkdir -p build
	$(CC) $(CFLAGS) -fe=$(TARGET) $(SRC)

build/intro.dat: assets/intro.dat
	mkdir -p build
	cp assets/intro.dat build/

build/dosbox.conf: dosbox.conf
	mkdir -p build
	cp dosbox.conf build/

clean:
	rm -f build/*
	rm -f *.o
	rm -f src/*.o
