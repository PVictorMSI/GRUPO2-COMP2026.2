# Gera um programa Lua aleatório com centenas de expressões (aritméticas, lógicas,
# comparações, concatenação) e if/while com condições não booleanas.
# Usado por fuzz_operadores.sh para comparar o Python gerado com o Lua 5.4 real (#36).
# Uso: python3 fuzz_operadores.py <semente> > programa.lua
import random, sys
random.seed(int(sys.argv[1]))
NUMS = ["0", "1", "2", "3", "7", "10", "0.5", "2.5", "-3", "1e2", "a", "b", "f()"]
def num(d):
    if d <= 0 or random.random() < 0.3:
        return random.choice(NUMS)
    op = random.choice(["+", "-", "*", "/", "//", "%", "^", "-u"])
    if op == "-u": return "-(" + num(d-1) + ")"
    if op in ("//", "%"):  # evita divisão inteira por zero
        return "(" + num(d-1) + " " + op + " " + random.choice(["2", "3", "7", "2.5", "a", "b"]) + ")"
    if op == "^": return "(" + num(d-1) + " ^ " + random.choice(["2", "3", "0.5", "-1"]) + ")"
    if op == "/": return "(" + num(d-1) + " / " + random.choice(["2", "4", "0.5", "a", "b"]) + ")"
    return "(" + num(d-1) + " " + op + " " + num(d-1) + ")"
ANY = ["nil", "true", "false", "0", "1", "''", "'x'", "a", "n", "t1", "t2", "s"]
def anyv(d):
    r = random.random()
    if d <= 0 or r < 0.25: return random.choice(ANY)
    if r < 0.45: return "(" + anyv(d-1) + " " + random.choice(["and", "or"]) + " " + anyv(d-1) + ")"
    if r < 0.55: return "(not " + anyv(d-1) + ")"
    if r < 0.7: return "(" + anyv(d-1) + " " + random.choice(["==", "~="]) + " " + anyv(d-1) + ")"
    if r < 0.8: return "(" + num(d-1) + " " + random.choice(["<", "<=", ">", ">="]) + " " + num(d-1) + ")"
    if r < 0.9: return "(" + random.choice(["s", "'ab'", num(1)]) + " .. " + random.choice(["s", "'c'", num(1)]) + ")"
    return num(d)
print("local a, b, n, s = 4, 1.5, nil, 'str'")
print("local t1, t2 = {}, {}")
print("local function f() return 5 end")
for i in range(200):
    e = num(3) if i % 2 else anyv(3)
    print("print(%d, %s)" % (i, e))
    if i % 7 == 0:
        print("if %s then print('if', %d) else print('else', %d) end" % (anyv(2), i, i))
        print("local w%d = 0 while w%d < 2 and %s do w%d = w%d + 1 end print('w', w%d)" % (i, i, anyv(1), i, i, i))
