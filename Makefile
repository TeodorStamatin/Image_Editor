# Copyright Stamatin Teodor 315CA 2022 - 2023

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS = image_editor

build: $(TARGETS)

image_editor: image_editor.c function.c function.h
	$(CC) $(CFLAGS) image_editor.c function.c function.h -o image_editor -lm

pack:
	zip -FSr 315CA_StamatinTeodor_Tema3.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
