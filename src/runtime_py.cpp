#include "runtime_py.hpp"

#include <set>
#include <sstream>

namespace lua2py {

// Convenções do runtime:
//  - todo nome definido aqui começa com "lua_" (ou é LuaError); a tabela de
//    símbolos renomeia variáveis do usuário que colidiriam (symbol_table.cpp);
//  - nil -> None, true/false -> True/False, número -> int/float, string -> str;
//  - tabela-lista (índices 1..n) -> list do Python (índices 0..n-1, issue #41);
//    tabela com chaves -> dict do Python;
//  - vários valores de retorno -> tuple do Python (issue #39). Uma tupla nunca é
//    um valor Lua comum, então ela sempre significa "lista de valores".

const std::vector<RuntimePart>& runtimeParts() {
    static const std::vector<RuntimePart> parts = {
// ------------------------------------------------------------------ imports
{"lua_pymath", "", R"PY(import math as lua_pymath
)PY"},
{"lua_pyrandom", "", R"PY(import random as lua_pyrandom
)PY"},
{"lua_pyfunctools", "", R"PY(import functools as lua_pyfunctools
)PY"},
{"lua_pysys", "", R"PY(import sys as lua_pysys
)PY"},

// ------------------------------------------------------------------ base
{"LuaError", "", R"PY(
class LuaError(Exception):
    """Erro do programa Lua (error(), índice em nil, etc.)."""

    def __init__(self, value):
        super().__init__(value)
        self.value = value
)PY"},

{"lua_truthy", "", R"PY(
def lua_truthy(v):
    """No Lua só nil e false são falsos: 0 e "" são verdadeiros."""
    return v is not None and v is not False
)PY"},

// ------------------------------------------------- retorno múltiplo (#39)
{"lua_first", "", R"PY(
def lua_first(v):
    """Usa só o primeiro de vários valores (ou nil se não houver nenhum)."""
    if isinstance(v, tuple):
        return v[0] if v else None
    return v
)PY"},

{"lua_multi", "", R"PY(
def lua_multi(v):
    """Expande uma chamada que pode devolver vários valores (último argumento/valor)."""
    return v if isinstance(v, tuple) else (v,)
)PY"},

{"lua_adjust", "", R"PY(
def lua_adjust(n, *vals):
    """Ajusta os valores a n variáveis: completa com nil ou descarta os que sobram."""
    if len(vals) >= n:
        return vals[:n]
    return vals + (None,) * (n - len(vals))
)PY"},

// ------------------------------------------------------------ valores
{"lua_type", "", R"PY(
def lua_type(v):
    """type() do Lua."""
    if v is None:
        return "nil"
    if isinstance(v, bool):
        return "boolean"
    if isinstance(v, (int, float)):
        return "number"
    if isinstance(v, str):
        return "string"
    if isinstance(v, (list, dict)):
        return "table"
    if callable(v):
        return "function"
    return "userdata"
)PY"},

{"lua_number_str", "lua_pymath", R"PY(
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
)PY"},

{"lua_tostring", "lua_number_str", R"PY(
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
)PY"},

{"lua_print", "lua_tostring", R"PY(
def lua_print(*args):
    """print() do Lua: valores separados por tabulação."""
    print("\t".join(lua_tostring(a) for a in args))
)PY"},

{"lua_tonumber", "", R"PY(
def lua_tonumber(v, base=None):
    """tonumber() do Lua: devolve nil quando a conversão falha."""
    if base is not None:
        try:
            return int(str(v).strip().lower(), base)
        except ValueError:
            return None
    if isinstance(v, (int, float)) and not isinstance(v, bool):
        return v
    if not isinstance(v, str):
        return None
    s = v.strip().lower()
    try:
        if s.startswith(("0x", "-0x")):
            return int(s, 16)
        return int(s)
    except ValueError:
        pass
    if "n" in s or "i" in s:  # o Lua não aceita "inf"/"nan"
        return None
    try:
        return float(s)
    except ValueError:
        return None
)PY"},

// ----------------------------------------------------- operadores (#36)
{"lua_eq", "", R"PY(
def lua_eq(a, b):
    """== do Lua: tabelas comparam identidade e true nunca é igual a 1."""
    if a is b:
        return True
    if isinstance(a, bool) or isinstance(b, bool):
        return False
    if isinstance(a, (list, dict)) or isinstance(b, (list, dict)):
        return False
    return a == b
)PY"},

{"lua_div", "lua_pymath", R"PY(
def lua_div(a, b):
    """/ do Lua: sempre float, e divisão por zero dá inf/-inf/nan (não é erro)."""
    a, b = float(a), float(b)
    if b == 0.0:
        if a == 0.0 or a != a:
            return lua_pymath.copysign(float("nan"), -1.0)
        return lua_pymath.copysign(float("inf"), a) * lua_pymath.copysign(1.0, b)
    return a / b
)PY"},

{"lua_idiv", "LuaError lua_div lua_pymath", R"PY(
def lua_idiv(a, b):
    """// do Lua: piso da divisão (inteiros: erro se b == 0; floats: inf/nan)."""
    if isinstance(a, int) and isinstance(b, int):
        if b == 0:
            raise LuaError("attempt to perform 'n//0'")
        return a // b
    q = lua_div(a, b)
    return float(lua_pymath.floor(q)) if lua_pymath.isfinite(q) else q
)PY"},

{"lua_mod", "LuaError lua_pymath", R"PY(
def lua_mod(a, b):
    """% do Lua: resto com o sinal do divisor (inteiros: erro se b == 0)."""
    if isinstance(a, int) and isinstance(b, int):
        if b == 0:
            raise LuaError("attempt to perform 'n%%0'")
        return a % b
    a, b = float(a), float(b)
    if b == 0.0 or not lua_pymath.isfinite(a):
        return lua_pymath.copysign(float("nan"), -1.0)
    m = lua_pymath.fmod(a, b)
    if (b < 0) if m > 0 else (m < 0 and b != m):
        m += b
    return m
)PY"},

{"lua_pow", "lua_pymath", R"PY(
def lua_pow(a, b):
    """^ do Lua: sempre float; casos sem resultado real dão nan/inf (não é erro)."""
    a, b = float(a), float(b)
    try:
        return lua_pymath.pow(a, b)
    except OverflowError:
        odd = b.is_integer() and int(b) % 2 == 1
        return float("-inf") if a < 0 and odd else float("inf")
    except ValueError:
        if a == 0.0:  # 0 ^ negativo
            odd = b.is_integer() and int(b) % 2 == 1
            return lua_pymath.copysign(float("inf"), a) if odd else float("inf")
        return lua_pymath.copysign(float("nan"), -1.0)
)PY"},

{"lua_concat", "LuaError lua_type lua_number_str", R"PY(
def lua_concat(*parts):
    """Operador .. do Lua: só aceita strings e números."""
    out = []
    for p in parts:
        if isinstance(p, str):
            out.append(p)
        elif isinstance(p, (int, float)) and not isinstance(p, bool):
            out.append(lua_number_str(p))
        else:
            raise LuaError("attempt to concatenate a %s value" % lua_type(p))
    return "".join(out)
)PY"},

// ------------------------------------------------- tabelas e base 1 (#41)
{"lua_list_index", "", R"PY(
# Tabelas (#41): o índice Lua k (base 1) de uma lista é a posição k - 1 (base 0)
# do Python. A conversão de base fica concentrada aqui.
def lua_list_index(t, k):
    """Posição Python (base 0) do índice Lua k, ou None se k não está na lista."""
    if isinstance(k, bool):
        return None
    if isinstance(k, float):
        if not k.is_integer():
            return None
        k = int(k)
    if isinstance(k, int) and 1 <= k <= len(t):
        return k - 1
    return None
)PY"},

{"lua_get", "LuaError lua_type lua_list_index", R"PY(
def lua_get(t, k):
    """t[k] do Lua (nil se a chave não existe)."""
    if isinstance(t, list):
        i = lua_list_index(t, k)
        return t[i] if i is not None else None
    if isinstance(t, dict):
        return t.get(k)
    raise LuaError("attempt to index a %s value" % lua_type(t))
)PY"},

{"lua_set", "LuaError lua_type lua_tostring lua_list_index", R"PY(
def lua_set(t, k, v):
    """t[k] = v do Lua. Em lista, t[#t + 1] = v acrescenta e t[#t] = nil remove."""
    if isinstance(t, list):
        i = lua_list_index(t, k)
        if i is not None:
            t[i] = v
            while t and t[-1] is None:
                t.pop()
            return
        kk = int(k) if isinstance(k, float) and k.is_integer() else k
        if isinstance(kk, int) and not isinstance(kk, bool) and kk == len(t) + 1:
            if v is not None:  # t[#t + 1] = v
                t.append(v)
            return
        raise LuaError("tabela usada como lista (índices 1..n) não aceita a chave %s"
                       % lua_tostring(k))
    if isinstance(t, dict):
        if k is None:
            raise LuaError("index is nil")
        if v is None:
            t.pop(k, None)
        else:
            t[k] = v
        return
    raise LuaError("attempt to index a %s value" % lua_type(t))
)PY"},

{"lua_len", "LuaError lua_type", R"PY(
def lua_len(v):
    """Operador # do Lua (string: número de bytes; tabela: último índice 1..n)."""
    if isinstance(v, str):
        return len(v.encode("utf-8"))
    if isinstance(v, list):
        return len(v)
    if isinstance(v, dict):
        n = 0
        while v.get(n + 1) is not None:
            n += 1
        return n
    raise LuaError("attempt to get length of a %s value" % lua_type(v))
)PY"},

// ----------------------------------------------------- laços (#30)
{"lua_range", "LuaError lua_pymath lua_pysys", R"PY(
def lua_range(start, stop, step=1):
    """Valores de "for i = start, stop, step" (Lua 5.4; limites avaliados uma vez)."""
    for v, what in ((start, "initial"), (stop, "limit"), (step, "step")):
        if not isinstance(v, (int, float)) or isinstance(v, bool):
            raise LuaError("'for' %s value must be a number" % what)
    if step == 0:
        raise LuaError("'for' step is zero")
    if isinstance(start, int) and isinstance(step, int):
        if isinstance(stop, float):  # limite float com laço inteiro: arredonda
            if stop != stop:
                return range(0)
            if stop in (float("inf"), float("-inf")):
                stop = lua_pysys.maxsize if stop > 0 else -lua_pysys.maxsize
            else:
                stop = lua_pymath.floor(stop) if step > 0 else lua_pymath.ceil(stop)
        return range(start, stop + 1, step) if step > 0 else range(start, stop - 1, step)
    return lua_float_range(float(start), float(stop), float(step))


def lua_float_range(i, stop, step):
    while (i <= stop) if step > 0 else (i >= stop):
        yield i
        i += step
)PY"},

{"lua_pairs_iter", "LuaError lua_type", R"PY(
def lua_pairs_iter(t):
    """Pares (chave, valor) de "for k, v in pairs(t)" (chaves de lista em base 1)."""
    if isinstance(t, list):
        for i, v in enumerate(list(t)):
            if v is not None:
                yield i + 1, v
    elif isinstance(t, dict):
        for k, v in list(t.items()):
            if v is not None:
                yield k, v
    else:
        raise LuaError("bad argument #1 to 'pairs' (table expected, got %s)" % lua_type(t))
)PY"},

{"lua_ipairs_iter", "lua_get", R"PY(
def lua_ipairs_iter(t):
    """Pares (i, t[i]) de "for i, v in ipairs(t)", parando no primeiro nil."""
    i = 1
    while True:
        v = lua_get(t, i)
        if v is None:
            return
        yield i, v
        i += 1
)PY"},

{"lua_next", "lua_pairs_iter lua_eq", R"PY(
def lua_next(t, k=None):
    """next(t, k) do Lua: o par seguinte a k (ou nil no fim)."""
    items = list(lua_pairs_iter(t))
    if k is None:
        pos = 0
    else:
        pos = next((j + 1 for j, (kk, _) in enumerate(items) if lua_eq(kk, k)), len(items))
    return items[pos] if pos < len(items) else None
)PY"},

{"lua_pairs", "lua_next", R"PY(
def lua_pairs(t):
    """pairs(t) usado como valor: devolve (next, t, nil), como no Lua."""
    return (lua_next, t, None)
)PY"},

{"lua_ipairs", "lua_get", R"PY(
def lua_ipairs_step(t, i):
    i += 1
    v = lua_get(t, i)
    return None if v is None else (i, v)


def lua_ipairs(t):
    """ipairs(t) usado como valor: devolve (iterador, t, 0), como no Lua."""
    return (lua_ipairs_step, t, 0)
)PY"},

{"lua_generic_for", "lua_adjust lua_multi", R"PY(
def lua_generic_for(n, f, s=None, ctrl=None):
    """Protocolo geral de "for v1, ..., vn in f, s, ctrl do": chama f(s, ctrl) até nil."""
    while True:
        vals = lua_adjust(n, *lua_multi(f(s, ctrl)))
        if vals[0] is None:
            return
        ctrl = vals[0]
        yield vals[0] if n == 1 else vals
)PY"},

// ------------------------------------------------ funções básicas do Lua
{"lua_select", "LuaError", R"PY(
def lua_select(n, *args):
    """select(n, ...) / select("#", ...) do Lua."""
    if n == "#":
        return len(args)
    if n < 0:
        if -n > len(args):
            raise LuaError("bad argument #1 to 'select' (index out of range)")
        return args[n:]
    if n == 0:
        raise LuaError("bad argument #1 to 'select' (index out of range)")
    return args[n - 1:]
)PY"},

{"lua_error", "LuaError", R"PY(
def lua_error(msg=None, level=1):
    """error() do Lua."""
    raise LuaError(msg)
)PY"},

{"lua_assert", "LuaError lua_truthy", R"PY(
def lua_assert(v=None, *rest):
    """assert() do Lua: devolve todos os argumentos se v é verdadeiro."""
    if not lua_truthy(v):
        raise LuaError(rest[0] if rest else "assertion failed!")
    return (v,) + rest
)PY"},

{"lua_pcall", "LuaError lua_multi", R"PY(
def lua_pcall(f, *args):
    """pcall() do Lua: (true, resultados...) ou (false, mensagem de erro)."""
    try:
        return (True,) + lua_multi(f(*args))
    except LuaError as e:
        return (False, e.value)
    except RecursionError:
        return (False, "stack overflow")
    except Exception as e:  # erro do próprio Python (ex.: chamar nil)
        return (False, str(e))
)PY"},

// -------------------------------- subconjunto da biblioteca padrão
{"lua_math", "lua_pymath lua_pyrandom", R"PY(
def lua_math_floor(x):
    return lua_pymath.floor(x) if isinstance(x, float) and lua_pymath.isfinite(x) else x


def lua_math_ceil(x):
    return lua_pymath.ceil(x) if isinstance(x, float) and lua_pymath.isfinite(x) else x


def lua_math_tointeger(x):
    if isinstance(x, int) and not isinstance(x, bool):
        return x
    if isinstance(x, float) and x.is_integer():
        return int(x)
    return None


def lua_math_type(x):
    if isinstance(x, bool) or not isinstance(x, (int, float)):
        return None
    return "integer" if isinstance(x, int) else "float"


def lua_math_random(m=None, n=None):
    if m is None:
        return lua_pyrandom.random()
    if n is None:
        return lua_pyrandom.randint(1, m)
    return lua_pyrandom.randint(m, n)


lua_math = {
    "abs": abs, "ceil": lua_math_ceil, "floor": lua_math_floor, "max": max, "min": min,
    "sqrt": lua_pymath.sqrt, "exp": lua_pymath.exp, "log": lua_pymath.log,
    "sin": lua_pymath.sin, "cos": lua_pymath.cos, "tan": lua_pymath.tan,
    "fmod": lua_pymath.fmod, "tointeger": lua_math_tointeger, "type": lua_math_type,
    "random": lua_math_random, "randomseed": lua_pyrandom.seed,
    "huge": float("inf"), "pi": lua_pymath.pi,
    "maxinteger": 2 ** 63 - 1, "mininteger": -2 ** 63,
}
)PY"},

{"lua_string", "LuaError lua_tostring lua_tonumber", R"PY(
def lua_str_sub(s, i=1, j=-1):
    n = len(s)
    if i < 0:
        i = max(n + i + 1, 1)
    elif i == 0:
        i = 1
    if j < 0:
        j = n + j + 1
    elif j > n:
        j = n
    return s[i - 1:j] if i <= j else ""


def lua_str_rep(s, n, sep=""):
    return sep.join([s] * n) if n > 0 else ""


def lua_str_byte(s, i=1, j=None):
    j = i if j is None else j
    return tuple(ord(c) for c in lua_str_sub(s, i, j))


def lua_str_find(s, pattern, init=1, plain=False):
    if not plain and any(c in pattern for c in "^$*+?.([%-"):
        raise LuaError("string.find: padrões do Lua não são suportados (use plain = true)")
    start = (init - 1) if init > 0 else max(len(s) + init, 0)
    pos = s.find(pattern, start)
    return None if pos < 0 else (pos + 1, pos + len(pattern))


def lua_str_format(fmt, *args):
    out, i, argi, n = [], 0, 0, len(fmt)
    while i < n:
        c = fmt[i]
        i += 1
        if c != "%":
            out.append(c)
            continue
        if i < n and fmt[i] == "%":
            out.append("%")
            i += 1
            continue
        j = i
        while j < n and fmt[j] in "-+ #0123456789.":
            j += 1
        if j >= n:
            raise LuaError("invalid conversion '%" + fmt[i:] + "' to 'format'")
        spec, conv = fmt[i:j], fmt[j]
        i = j + 1
        if argi >= len(args):
            raise LuaError("bad argument #%d to 'format' (no value)" % (argi + 2))
        a = args[argi]
        argi += 1
        if conv in "dixXoc":
            num = lua_tonumber(a)
            if num is None or (isinstance(num, float) and not num.is_integer()):
                raise LuaError("bad argument #%d to 'format' (number has no integer representation)"
                               % (argi + 1))
            num = int(num)
            if conv == "c":
                out.append(chr(num))
            else:
                out.append(("%" + spec + ("d" if conv == "i" else conv)) % num)
        elif conv in "eEfFgG":
            num = lua_tonumber(a)
            if num is None:
                raise LuaError("bad argument #%d to 'format' (number expected)" % (argi + 1))
            out.append(("%" + spec + conv) % float(num))
        elif conv == "s":
            out.append(("%" + spec + "s") % lua_tostring(a))
        elif conv == "q":
            q = lua_tostring(a).replace("\\", "\\\\").replace('"', '\\"').replace("\n", "\\n")
            out.append('"' + q + '"')
        else:
            raise LuaError("invalid conversion '%%%s' to 'format'" % conv)
    return "".join(out)


lua_string = {
    "len": lambda s: len(s.encode("utf-8")), "sub": lua_str_sub,
    "upper": lambda s: s.upper(), "lower": lambda s: s.lower(),
    "rep": lua_str_rep, "reverse": lambda s: s[::-1],
    "byte": lua_str_byte, "char": lambda *cs: "".join(chr(c) for c in cs),
    "find": lua_str_find, "format": lua_str_format,
}
)PY"},

{"lua_table", "LuaError lua_get lua_set lua_len lua_concat lua_truthy lua_pyfunctools", R"PY(
def lua_tbl_insert(t, *args):
    n = lua_len(t)
    if len(args) == 1:
        lua_set(t, n + 1, args[0])
        return
    pos, v = args
    if not 1 <= pos <= n + 1:
        raise LuaError("bad argument #2 to 'insert' (position out of bounds)")
    if isinstance(t, list):
        t.insert(pos - 1, v)
        return
    for k in range(n, pos - 1, -1):
        lua_set(t, k + 1, lua_get(t, k))
    lua_set(t, pos, v)


def lua_tbl_remove(t, pos=None):
    n = lua_len(t)
    if pos is None:
        pos = n
    if n == 0 and pos in (0, n):
        return None
    if isinstance(t, list):
        return t.pop(pos - 1)
    v = lua_get(t, pos)
    for k in range(pos, n):
        lua_set(t, k, lua_get(t, k + 1))
    lua_set(t, n, None)
    return v


def lua_tbl_concat(t, sep="", i=1, j=None):
    j = lua_len(t) if j is None else j
    return sep.join(lua_concat(lua_get(t, k)) for k in range(i, j + 1))


def lua_tbl_unpack(t, i=1, j=None):
    j = lua_len(t) if j is None else j
    return tuple(lua_get(t, k) for k in range(i, j + 1))


def lua_tbl_sort(t, comp=None):
    n = lua_len(t)
    items = [lua_get(t, k) for k in range(1, n + 1)]
    if comp is None:
        items.sort()
    else:
        def cmp(a, b):
            if lua_truthy(comp(a, b)):
                return -1
            return 1 if lua_truthy(comp(b, a)) else 0
        items.sort(key=lua_pyfunctools.cmp_to_key(cmp))
    for k, v in enumerate(items):
        lua_set(t, k + 1, v)


lua_table = {
    "insert": lua_tbl_insert, "remove": lua_tbl_remove, "concat": lua_tbl_concat,
    "unpack": lua_tbl_unpack, "sort": lua_tbl_sort,
}
lua_unpack = lua_tbl_unpack
)PY"},

{"lua_io", "lua_tostring lua_pysys", R"PY(
def lua_io_write(*args):
    """io.write do Lua (diferente do print, float inteiro sai sem ".0")."""
    for a in args:
        if isinstance(a, float) and not isinstance(a, bool):
            lua_pysys.stdout.write("%.14g" % a)
        else:
            lua_pysys.stdout.write(lua_tostring(a))


lua_io = {"write": lua_io_write}
)PY"},

{"lua_void", "", R"PY(
def lua_void(_):
    """Chamada de função que não devolve nada, expandida: zero valores."""
    return ()
)PY"},

// -------------------------------------------------------- métodos
{"lua_invoke", "LuaError lua_get lua_string", R"PY(
def lua_invoke(obj, name, *args):
    """obj:name(...) do Lua: chama obj.name passando obj como primeiro argumento."""
    f = lua_string.get(name) if isinstance(obj, str) else lua_get(obj, name)
    if f is None:
        raise LuaError("attempt to call a nil value (method '%s')" % name)
    return f(obj, *args)
)PY"},
    };
    return parts;
}

std::string runtimeCode(const std::vector<std::string>& wanted) {
    const auto& parts = runtimeParts();
    std::set<std::string> need;
    std::vector<std::string> stack(wanted.begin(), wanted.end());
    while (!stack.empty()) {
        std::string n = stack.back();
        stack.pop_back();
        if (!need.insert(n).second) continue;
        for (const auto& p : parts) {
            if (n != p.name) continue;
            std::istringstream deps(p.deps);
            std::string d;
            while (deps >> d) stack.push_back(d);
        }
    }
    std::string imports, code;
    for (const auto& p : parts) {
        if (!need.count(p.name)) continue;
        std::string c = p.code;
        if (c.rfind("import ", 0) == 0) imports += c;
        else code += c;
    }
    return imports + code;
}

}  // namespace lua2py
