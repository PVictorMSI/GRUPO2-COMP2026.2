# Makefile do transpilador Lua -> Python (RNF09: compilar em Linux via make)
#
# Uso:
#   make                  compila o compilador         -> build/lua2py
#   make check            compila e roda as verificações automáticas ([OK]/[FALHA])
#   make test-simbolos    compila e MOSTRA a tabela de símbolos dos 3 programas de teste
#   make TRACE=1          liga o rastreio de tokens do lexer (números no stderr)
#                         (para trocar de modo: make clean && make TRACE=1)
#   make clean            apaga tudo que foi gerado
#
# Requisitos: g++ (C++17), bison, flex.
#
# Tudo é compilado como C++ (-x c++), inclusive os .c gerados pelo Flex/Bison,
# porque o %union e o ast.hpp usam tipos C++ (std::string, std::vector...).

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g -O0 -Isrc -Ibuild -MMD -MP
ifdef TRACE
CXXFLAGS += -DTRACE_TOKENS
endif
BISON    := bison
FLEX     := flex

BIN      := build/lua2py
TEST_BIN := build/test_symbols

# main: aceita src/main.cpp ou src/main.c
MAIN_SRC := $(if $(wildcard src/main.cpp),src/main.cpp,src/main.c)

# Demais fontes escritos à mão em src/ (symbol_table, resolver, e o que vier depois)
LIB_SRC  := $(filter-out src/main.cpp src/main.c,$(wildcard src/*.cpp))
LIB_OBJ  := $(patsubst src/%.cpp,build/%.o,$(LIB_SRC))

# Código gerado pelo Bison e pelo Flex
GEN_OBJ  := build/parser.tab.o build/lex.yy.o

.PHONY: all test-simbolos check clean
all: $(BIN)

$(BIN): build/main.o $(LIB_OBJ) $(GEN_OBJ)
	$(CXX) $^ -o $@

# ---------- geração do parser e do lexer ----------
build:
	mkdir -p build

build/parser.tab.c: parser/parser.y | build
	$(BISON) -d $< -o $@

# o .h sai junto com o .c (-d); esta regra só avisa o make disso
build/parser.tab.h: build/parser.tab.c ;

build/lex.yy.c: lexer/lexer.l build/parser.tab.h | build
	$(FLEX) -o $@ $<

# ---------- compilação ----------
build/lex.yy.o: build/parser.tab.h

build/%.o: build/%.c
	$(CXX) $(CXXFLAGS) -x c++ -c $< -o $@

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/main.o: $(MAIN_SRC) build/parser.tab.h | build
	$(CXX) $(CXXFLAGS) -x c++ -c $< -o $@

# ---------- teste da tabela de símbolos ----------
$(TEST_BIN): build/test_symbols.o $(LIB_OBJ) $(GEN_OBJ)
	$(CXX) $^ -o $@

build/test_symbols.o: Testes/unitarios/test_symbols.cpp build/parser.tab.h | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

test-simbolos: $(TEST_BIN)
	@for f in Testes/unitarios/teste_simbolos_*.lua; do \
	  echo "=== $$f ==="; $(TEST_BIN) "$$f" || exit 1; echo; \
	done

check: all $(TEST_BIN)
	@bash Testes/unitarios/rodar_checks.sh

clean:
	rm -f build/*.o build/*.d build/parser.tab.c build/parser.tab.h build/lex.yy.c $(BIN) $(TEST_BIN)

-include $(wildcard build/*.d)
