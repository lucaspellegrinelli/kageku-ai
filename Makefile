CC := g++
CFLAGS := -g -Wall -std=c++11 -O3

# Nome do executável
TARGET := main

# Caminho para os .cpp
SRCDIR := src

# Caminho para os .h
HDIR := headers

# Caminho onde serão criados os .o
OBJDIR := obj

SRCS := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

# Gera o executável a partir dos .o gerados
$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	@$(CC) $^ -o $@

# Cria os arquivos .o necessários para a compilação
$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $^ -o $@ -I $(HDIR)

# Definição do procedimento de exclusão dos arquivos/pastas geradas pelo makefile
.PHONY: clean
clean:
	@rm -rf $(TARGET) $(OBJDIR)/*
	@rm -r $(OBJDIR)/
