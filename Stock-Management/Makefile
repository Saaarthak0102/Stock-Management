CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lm

SRCDIR = src
OBJDIR = obj
BINDIR = .

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
TARGET = $(BINDIR)/stock_system.exe

.PHONY: all build clean run help

all: build

build: $(TARGET)
	@echo "[SUCCESS] Build completed: $(TARGET)"

$(TARGET): $(OBJECTS)
	@echo "[LINKING] Creating executable..."
	@$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@echo "[COMPILING] $<"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "[CLEANING] Removing build artifacts..."
	@if exist $(OBJDIR) rmdir /s /q $(OBJDIR)
	@if exist $(TARGET) del /q $(TARGET)
	@echo "[SUCCESS] Clean completed"

run: build
	@echo "[RUNNING] Starting Stock Management System..."
	@$(TARGET)

help:
	@echo "Stock Management System - Build Commands"
	@echo "Usage: make [target]"
	@echo "Targets: all | build | clean | run | help"
