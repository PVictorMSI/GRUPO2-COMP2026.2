#!/usr/bin/env bash
# Regrava os arquivos .esperado (saída do Lua 5.4 real) de cada programa desta pasta.
# Use depois de criar ou alterar um teste. Precisa de lua5.4 (sudo apt install lua5.4).
# Rode da raiz do projeto:  bash Testes/geracao/gerar_esperados.sh
LUA="${LUA:-lua5.4}"
command -v "$LUA" >/dev/null || { echo "$LUA não instalado"; exit 2; }
for f in Testes/geracao/*.lua; do
    "$LUA" "$f" > "${f%.lua}.esperado" 2>&1 && echo "ok   $f" || echo "ERRO $f (veja ${f%.lua}.esperado)"
done
