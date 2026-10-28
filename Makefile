# ==============================================================================
# Makefile — Compilador Lua → Python (GRUPO2-COMP2026.2)
# Ferramentas: Flex + Bison + g++
# Uso:
#   make          → compila e gera o executável em build/lua2py
#   make test     → roda toda a suíte de testes
#   make clean    → apaga os arquivos gerados
# ==============================================================================

# --- Ferramentas ---
CC      = g++
FLEX    = flex
BISON   = bison

# --- Diretórios ---
BUILD   = build
LEXDIR  = lexer
PARDIR  = parser
SRCDIR  = src

# --- Arquivos fonte ---
LEXER_L  = $(LEXDIR)/lexer.l
PARSER_Y = $(PARDIR)/parser.y

# --- Arquivos gerados (ficam em build/) ---
LEX_C    = $(BUILD)/lex.yy.c
PARSE_C  = $(BUILD)/parser.tab.c
PARSE_H  = $(BUILD)/parser.tab.h
TARGET   = $(BUILD)/lua2py

# --- Flags de compilação ---
CFLAGS  = -Wall -Wno-unused-function -I$(BUILD) -I$(SRCDIR)

# ==============================================================================
# Regra padrão: compilar tudo
# ==============================================================================
all: $(TARGET)

$(TARGET): $(LEX_C) $(PARSE_C)
	$(CC) $(CFLAGS) $(PARSE_C) $(LEX_C) -o $(TARGET)
	@echo ""
	@echo "✅ Build concluído! Executável gerado em: $(TARGET)"
	@echo "   Para usar: ./$(TARGET) < arquivo.lua"
	@echo ""

# Gera o parser C a partir do arquivo .y do Bison
$(PARSE_C) $(PARSE_H): $(PARSER_Y) | $(BUILD)
	$(BISON) -d -o $(PARSE_C) $(PARSER_Y)

# Gera o lexer C a partir do arquivo .l do Flex
# Depende do header do Bison (parser.tab.h) para conhecer os tokens
$(LEX_C): $(LEXER_L) $(PARSE_H) | $(BUILD)
	$(FLEX) -o $(LEX_C) $(LEXER_L)

# Garante que a pasta build/ existe
$(BUILD):
	mkdir -p $(BUILD)

# ==============================================================================
# Suíte de testes: roda todos os arquivos de teste e filtra os números de debug
# ==============================================================================
FILTER = 2>&1 | grep -v '^[0-9]\+$$'

test: $(TARGET)
	@echo "=========================================="
	@echo "  SUÍTE DE TESTES — Lua → Python"
	@echo "=========================================="
	@echo ""
	@echo "--- TESTES CORRETOS (devem passar sem erro) ---"
	@for f in "Testes/Testes certos"/*.lua; do \
		echo ""; \
		echo "  ✔ $$(basename "$$f")"; \
		./$(TARGET) < "$$f" $(FILTER) || true; \
	done
	@echo ""
	@echo "--- TESTES DE ERRO (devem reportar erro léxico) ---"
	@for f in "Testes/Testes errados"/*.lua; do \
		echo ""; \
		echo "  ✘ $$(basename "$$f")"; \
		./$(TARGET) < "$$f" $(FILTER) || true; \
	done
	@echo ""
	@echo "=========================================="
	@echo "  Testes finalizados."
	@echo "=========================================="

# ==============================================================================
# Limpeza: remove apenas os arquivos GERADOS (nunca os fontes .l e .y)
# ==============================================================================
clean:
	rm -f $(LEX_C) $(PARSE_C) $(PARSE_H) $(TARGET)
	@echo "Arquivos gerados removidos."

.PHONY: all test clean
