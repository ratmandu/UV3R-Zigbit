MAINDIR := ..

MCU = atmega1281
CPU = 8000000

TARGET = main

DEPENDS= ./main.o ./rda1846.o ./afsk.o

main:
	@echo $(DEPENDS)
	@$(MAKE) TARGET=$(TARGET) DEPENDS="$(DEPENDS)" all

include $(MAINDIR)/makefile.common
