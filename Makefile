EXE = AsteroidsGame
SOURCES = main.c
OBJDIR = build
$(shell mkdir -p $(OBJDIR))
OBJS = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

CC = gcc
COMPILER_FLAGS = -w -std=c99 -I -Ibackends -g -Wall -Wformat `sdl2-config --cflags`
LINKER_FLAGS = -lSDL2 -lm

build/main.o: main.c
	$(CC) $(COMPILER_FLAGS) -c -o $@ $<

# make all
all: $(EXE)

$(EXE): $(OBJS)
	$(CC) -o $@ $^ $(COMPILER_FLAGS) $(LINKER_FLAGS)
	@echo Build complete

# make clean
clean:
	rm -f $(OBJS) $(EXE)