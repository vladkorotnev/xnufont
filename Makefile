PROG_NAME="xnufontmaker"
FONT_NAME="Lat15XNU8x16.psf"

default: font

builder:
	cp consts.h consts_actual.h
	echo "#define FILENAME \""$(FONT_NAME)"\"" >> consts_actual.h
	gcc main.c -o $(PROG_NAME)
	rm consts_actual.h

font: builder
	./$(PROG_NAME)

clean: 
	rm ./$(PROG_NAME)
	rm ./$(FONT_NAME)

install: font
	cp ./$(FONT_NAME) /usr/share/consolefonts/$(FONT_NAME)
