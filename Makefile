# usage - all executables placed in ./bin, all .o in ./obj

# make tests		- make all unit tests
# make programs		- make any other programs made during process

# make library		- make lib

# make clean		- self explanatory
# make cleanlib
# make cleanobj
# make cleanbin

# search in these folders for any .c files
vpath %.c src/ src/smallengine src/smallengine/sys src/smallengine/maths src/smallengine/graphics src/programs test/smallengine test/infiltrator

# Compiler
CC = gcc
CCFLAGS = -g -Wall

# Folders
# library files
SOURCES = maths.c mem.c log.c arg.c tuple.c matrix.c canvas.c color.c sw_renderer.c timer.c console.c input.c texture.c palette.c

OBJECTS = $(patsubst %.c, obj/%.o, $(SOURCES))
SE_LIBRARY = lib/libsmallengine.a
INCLUDE = -I./inc/
MYLIBS = -lsmallengine
CLIBS = -lm -lSDL2 -lSDL2main

TEST_SOURCES = $(notdir $(wildcard test/*.c)) $(notdir $(wildcard test/smallengine/*.c))
TESTS := $(addprefix bin/, $(notdir $(TEST_SOURCES:.c=)))

EXERCISE_SOURCES = $(notdir $(wildcard src/exercises/*.c))
EXERCISES := $(addprefix bin/, $(notdir $(EXERCISE_SOURCES:.c=)))

PROG_SOURCES = $(notdir $(wildcard src/programs/*.c))
PROGRAMS := $(addprefix bin/, $(notdir $(PROG_SOURCES:.c=)))

# for user specified inputs
OUTPUT = $(addprefix bin/, $(notdir $(INPUT:.c=)))

# make bin, lib, obj and output folders if needed
$(shell [ -d bin ] || mkdir -p bin)
$(shell [ -d lib ] || mkdir -p lib)
$(shell [ -d obj ] || mkdir -p obj)
$(shell [ -d out ] || mkdir -p out)

	
obj/%.o: %.c
	@echo $<
	@$(CC) $(CCFLAGS) $(INCLUDE) -c $< -o $@ $(CLIBS)
	
library: $(OBJECTS)
	ar rcs $(SE_LIBRARY) $(OBJECTS)

tests: library $(TESTS)

exercises: library $(EXERCISES)

programs: library $(PROGRAMS)

bin/%: %.c
	$(CC) $(CCFLAGS) $(INCLUDE) -L./lib $< -o $@ $(MYLIBS) $(CLIBS)
	
input: $(INPUT) library
	$(CC) $(CCFLAGS) $(INCLUDE) -L./lib $< -o $(OUTPUT) $(MYLIBS) $(CLIBS)

# cleanup

.PHONY: clean cleanlib cleanall cleanbin

clean: cleanlib cleanobj
	
cleanobj:
	rm -rf obj/*
	
cleanlib:
	rm -rf lib/*
	
cleanbin:
	rm -rf bin/*
	
# valgrind cores
cleanvg:
	rm -rf vgcore*
	
cleanall: cleanbin cleanlib cleanobj cleanvg
	
testsuite:
	@echo Objects: $(OBJECTS)
	@echo Library: $(SE_LIBRARY)
	@echo Testsources: $(TEST_SOURCES)
	@echo Test progs: $(TESTS)
	@echo Ex sources: $(EXERCISE_SOURCES)
	@echo Ex progs: $(EXERCISES)
	@echo Program Sources: $(PROG_SOURCES)
	@echo Programs: $(PROGRAMS)
