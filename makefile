
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = retro_game.exe

SOURCES = main.c snake_engine.c tictactoe_engine.c ui_renderer.c analytics.c file_io.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	del /Q $(TARGET) 2>NUL
