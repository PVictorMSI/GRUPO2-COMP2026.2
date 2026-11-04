#!/usr/bin/env bash
# Teste diferencial dos operadores (#36): para cada semente, gera um programa Lua
# aleatório, traduz com o lua2py e compara a saída do Python com a do Lua 5.4 real.
# Precisa de lua5.4 instalado (sudo apt install lua5.4). Rode da raiz do projeto:
#   bash Testes/geracao/fuzz_operadores.sh [quantidade de sementes, padrão 20]
#
# Diferenças conhecidas e aceitas (ver docs/geracao_codigo.md):
#   - endereços de "table: 0x..." (mudam a cada execução; são filtrados aqui);
#   - sinal do zero em "x % 3" com x float negativo (Lua -0.0, Python 0.0).
LUA="${LUA:-lua5.4}"
command -v "$LUA" >/dev/null || { echo "$LUA não instalado"; exit 2; }
n="${1:-20}"; tmp=$(mktemp -d); falhas=0
for seed in $(seq 1 "$n"); do
    python3 Testes/geracao/fuzz_operadores.py "$seed" > "$tmp/p.lua"
    norm() { grep -v "table: 0x" | sed 's/-0\.0\b/0.0/g'; }   # diferenças aceitas
    "$LUA" "$tmp/p.lua" 2>&1 | norm > "$tmp/lua.txt"
    build/lua2py "$tmp/p.lua" -o "$tmp/p.py" || { echo "semente $seed: o compilador falhou"; falhas=$((falhas+1)); continue; }
    python3 "$tmp/p.py" 2>&1 | norm > "$tmp/py.txt"
    if ! diff -q "$tmp/lua.txt" "$tmp/py.txt" >/dev/null; then
        echo "semente $seed: DIFERENTE"; diff "$tmp/lua.txt" "$tmp/py.txt" | head -6
        falhas=$((falhas+1))
    fi
done
rm -rf "$tmp"
echo "$((n - falhas)) de $n sementes iguais ao Lua."
