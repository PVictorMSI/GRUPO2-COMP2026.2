# Gerado automaticamente pelo lua2py (GRUPO2-COMP2026.2) a partir de um programa Lua.

# ---- runtime: semântica do Lua que o Python não tem ----
import math as lua_pymath

def lua_number_str(n):
    """Formata um número como o Lua 5.4 ("%.14g"; float inteiro ganha ".0")."""
    if isinstance(n, int):
        return str(n)
    if n != n:
        return "-nan" if lua_pymath.copysign(1.0, n) < 0 else "nan"
    if n in (float("inf"), float("-inf")):
        return "inf" if n > 0 else "-inf"
    s = "%.14g" % n
    if all(c in "-0123456789" for c in s):
        s += ".0"
    return s

def lua_tostring(v):
    """tostring() do Lua."""
    if v is None:
        return "nil"
    if v is True:
        return "true"
    if v is False:
        return "false"
    if isinstance(v, (int, float)):
        return lua_number_str(v)
    if isinstance(v, str):
        return v
    if isinstance(v, (list, dict)):
        return "table: 0x%012x" % id(v)
    if callable(v):
        return "function: 0x%012x" % id(v)
    return str(v)

def lua_print(*args):
    """print() do Lua: valores separados por tabulação."""
    print("\t".join(lua_tostring(a) for a in args))


# ---- programa ----

i = 0
while True:
    x = i
    i = i + 1
    lua_print("iteracao", i)
    if x >= 2:
        break
