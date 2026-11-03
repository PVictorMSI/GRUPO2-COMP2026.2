#!/usr/bin/env bash
# Verificações automáticas do compilador e da tabela de símbolos.
# Rode da RAIZ do projeto, preferencialmente via:  make check
#
# Cada linha imprime [OK] ou [FALHA]. O script termina com código 1 se algo falhar.

LUA2PY="${LUA2PY:-build/lua2py}"
SYM="${SYM:-build/test_symbols}"
DIR="Testes/unitarios"

pass=0
fail=0
ok()  { echo "  [OK]    $1"; pass=$((pass + 1)); }
nok() { echo "  [FALHA] $1"; fail=$((fail + 1)); }

# expect "descrição" <condição shell>
expect() {
    local desc="$1"; shift
    if "$@"; then ok "$desc"; else nok "$desc"; fi
}

for bin in "$LUA2PY" "$SYM"; do
    if [ ! -x "$bin" ]; then echo "ERRO: $bin não existe. Rode 'make check' (ele compila antes)."; exit 2; fi
done

# ---------------------------------------------------------------- compilador
echo "== Compilador (build/lua2py) =="

out=$("$LUA2PY" < "$DIR/valido_basico.lua" 2>&1); rc=$?
expect "programa válido via '<' : exit 0 e nenhuma mensagem" \
    test "$rc" -eq 0 -a -z "$out"

out=$("$LUA2PY" "$DIR/valido_basico.lua" 2>&1); rc=$?
expect "programa válido via argumento de arquivo: exit 0" test "$rc" -eq 0

out=$("$LUA2PY" < "$DIR/erro_sintatico.lua" 2>&1); rc=$?
expect "erro sintático: exit 1 (sem crash)" test "$rc" -eq 1
expect "erro sintático: mensagem traz 'Erro sintático na linha N'" \
    bash -c 'echo "$1" | grep -Eq "^Erro sintático na linha [0-9]+:"' _ "$out"

"$LUA2PY" /tmp/arquivo_que_nao_existe_$$.lua >/dev/null 2>&1; rc=$?
expect "arquivo inexistente: exit 1 (sem crash)" test "$rc" -eq 1

# ---------------------------------------------------------- tabela de símbolos
echo "== Tabela de símbolos (RF06) =="

all_python_names_unique() {   # nenhuma dupla de símbolos pode compartilhar o nome Python
    [ -z "$(echo "$1" | awk '/^[0-9]+ /{print $NF}' | sort | uniq -d)" ]
}
distinct_ids_for() {          # quantos símbolos DIFERENTES os usos do nome $2 referenciam
    echo "$1" | grep -E "^ +linha +[0-9]+ +$2 +->" | sed -E 's/.*-> #([0-9]+).*/\1/' | sort -u | wc -l
}

o=$("$SYM" "$DIR/teste_simbolos_1_criterio.lua" 2>&1)
expect "critério: existe um 'x' GLOBAL e um 'x' LOCAL (entidades distintas)" \
    bash -c 'echo "$1" | grep -Eq "^[0-9]+ +x +global " && echo "$1" | grep -Eq "^[0-9]+ +x +local "' _ "$o"
expect "critério: os usos de 'x' apontam para 2 símbolos diferentes" \
    test "$(distinct_ids_for "$o" x)" -eq 2
expect "critério: o local vira 'x_1' no Python e a global continua 'x'" \
    bash -c 'echo "$1" | grep -Eq "^[0-9]+ +x +local .* x_1$" && echo "$1" | grep -Eq "^[0-9]+ +x +global .* x$"' _ "$o"
expect "critério: nomes Python todos distintos" all_python_names_unique "$o"

o=$("$SYM" "$DIR/teste_simbolos_2_sombra.lua" 2>&1)
expect "sombreamento: três 'x' locais distintos" \
    test "$(echo "$o" | grep -Ec '^[0-9]+ +x +local ')" -eq 3
expect "sombreamento: os usos de 'x' apontam para 3 símbolos diferentes" \
    test "$(distinct_ids_for "$o" x)" -eq 3
expect "sombreamento: nomes Python todos distintos" all_python_names_unique "$o"

o=$("$SYM" "$DIR/teste_simbolos_3_lua.lua" 2>&1)
expect "recursão: 'fat' é local e capturado pelo próprio corpo" \
    bash -c 'echo "$1" | grep -Eq "^[0-9]+ +fat +local +0 +1 +sim "' _ "$o"
expect "closure: 'c' é marcado como capturado" \
    bash -c 'echo "$1" | grep -Eq "^[0-9]+ +c +local .* sim "' _ "$o"
expect "palavras reservadas do Python: 'class' -> class_ e 'def' -> def_" \
    bash -c 'echo "$1" | grep -Eq " class_$" && echo "$1" | grep -Eq " def_$"' _ "$o"
expect "function a:m() cria o parâmetro 'self'; function a.m() usa 'self' global" \
    bash -c 'echo "$1" | grep -Eq "^[0-9]+ +self +param " && echo "$1" | grep -Eq "^[0-9]+ +self +global "' _ "$o"
expect "nomes Python todos distintos" all_python_names_unique "$o"

# ------------------------------------------------- testes do próprio grupo
# Rodam em "Testes/Testes certos" e "Testes/Testes errados" (não fazem parte do pacote).
# As expectativas vêm dos comentários que o grupo escreveu nos próprios arquivos.
echo "== Testes do grupo (Testes/Testes certos e Testes/Testes errados) =="
CERTOS="Testes/Testes certos"
ERRADOS="Testes/Testes errados"

find_test() {   # find_test <pasta> <prefixo>  -> imprime o caminho do arquivo, ou nada
    local f
    for f in "$1"/"$2"*.lua; do [ -f "$f" ] && { printf '%s' "$f"; return; }; done
}

# certo <prefixo>: deve compilar sem nenhuma mensagem
certo() {
    local f; f=$(find_test "$CERTOS" "$1")
    [ -z "$f" ] && { echo "  (pulado: $CERTOS/$1* não encontrado)"; return; }
    local o rc; o=$("$LUA2PY" < "$f" 2>&1); rc=$?
    expect "certo   $1: exit 0 e sem mensagens" test "$rc" -eq 0 -a -z "$o"
}
# errado <prefixo> <padrão extra de mensagem> ...: deve falhar (exit 1) e conter cada padrão
errado() {
    local pre="$1"; shift
    local f; f=$(find_test "$ERRADOS" "$pre")
    [ -z "$f" ] && { echo "  (pulado: $ERRADOS/$pre* não encontrado)"; return; }
    local o rc; o=$("$LUA2PY" < "$f" 2>&1); rc=$?
    expect "errado  $pre: não compila (exit 1)" test "$rc" -eq 1
    local pat
    for pat in "$@"; do
        expect "errado  $pre: reporta '$pat'" \
            bash -c 'echo "$1" | grep -Fq -- "$2"' _ "$o" "$pat"
    done
}

certo Teste1
certo Teste2
certo Teste3
errado Teste1 "Erro léxico na linha 15:"                       # arquivo: "deve retornar algo na linha 15"
errado Teste2 "Erro léxico na linha 5:"                        # arquivo: "retorna algo na linha 5"
errado Teste3 "Erro léxico na linha 1:"                        # arquivo: "retorna erro na linha 1"
errado Teste4 "Erro léxico na linha 9:"                        # arquivo: "linha 3 ou 9"
errado Teste5 "Erro léxico na linha 1:" "Erro léxico na linha 2:"   # arquivo: "linha 1, depois linha 2, e não deve compilar"
errado Teste6 "Erro sintático na linha 3:"
errado Teste7 "Erro sintático na linha 5:"
errado Teste8 "Erro sintático na linha 5:"
errado Teste9 "Erro sintático na linha 7:"

# ------------------------------------------------------------------- memória
echo "== Memória (Valgrind) =="
if command -v valgrind >/dev/null 2>&1; then
    mem_files=("$DIR/valido_basico.lua" "$DIR/erro_sintatico.lua" "$DIR/teste_simbolos_3_lua.lua")
    for f in "$CERTOS"/*.lua "$ERRADOS"/*.lua; do [ -f "$f" ] && mem_files+=("$f"); done
    for f in "${mem_files[@]}"; do
        v=$(valgrind --leak-check=full "$LUA2PY" < "$f" 2>&1 >/dev/null)
        expect "$(basename "$f"): sem vazamento e sem acesso inválido" \
            bash -c 'echo "$1" | grep -q "definitely lost: 0 bytes" && echo "$1" | grep -q "indirectly lost: 0 bytes" && echo "$1" | grep -q "ERROR SUMMARY: 0 errors"' _ "$v"
    done
else
    echo "  (valgrind não instalado — pulando; instale com: sudo apt install valgrind)"
fi

echo
echo "Resultado: $pass passaram, $fail falharam."
[ "$fail" -eq 0 ]
