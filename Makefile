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

SRC_MAIN := src/usb-notify.o
SRC      :=

all: usb-notify

usb-notify: $(SRC_MAIN) $(SRC)
		 $(CC) $(CFLAGS) -o $(BIN)/$(PROJECT) $^ $(LIBS)

clean:
		 rm -f $(BIN)/*
		 rm -f src/*.o

install:
		 install -m 755 ./bin/usb-notify /usr/local/bin/usb-notify
