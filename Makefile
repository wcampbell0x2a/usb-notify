PROJECT=usb-notify
CC=gcc
BIN=bin

CFLAGS = -ansi \
		 -pedantic \
		 -g \
		 -std=c99 \
		 -W \
		 -Wextra \
		 -Wall \
		 `pkg-config --cflags --libs libnotify`
LIBS     =  -ludev

DEPENDS  := include/usb-notify.h
SRC_MAIN := src/usb-notify.o

all: usb-notify

$(SRC_MAIN): $(DEPENDS)

usb-notify: $(SRC_MAIN)
		 $(CC) $(CFLAGS) -o $(BIN)/$(PROJECT) $^ $(LIBS)

clean:
		 rm -f $(BIN)/*
		 rm -f src/*.o

install:
		 install -m 755 ./bin/usb-notify /usr/local/bin/usb-notify
