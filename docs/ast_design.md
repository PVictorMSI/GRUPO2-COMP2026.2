# Estrutura da AST — Compilador Lua / Python

**Issue relacionada:** #15 — Definir estrutura da AST (nós e tipos)
**Projeto:** GRUPO2-COMP2026.2 (Compiladores)

## 1. Visão Geral da Arquitetura

Este documento e o cabeçalho associado descrevem a estrutura da Árvore de Sintaxe Abstrata (AST) adotada para representar programas escritos em Lua antes da etapa de tradução/geração de código para Python. A AST é organizada em torno de duas grandes categorias de nós: `Stmt` (comandos/statements) e `Expr` (expressões), ambas derivadas de uma classe base comum chamada `ASTNode`. Essa divisão reflete diretamente a gramática do Lua, na qual programas consistem fundamentalmente em sequências de comandos compostos por expressões.

Principais escolhas de design da implementação:

* **Identificação de nós sem RTTI (`NodeKind`):** Cada nó possui um campo `kind` (do tipo `NodeKind`), permitindo que o gerador de código Python e a análise semântica façam *dispatch* (utilizando switch/case ou padrão Visitor) sem depender do custo e da prolixidade de `dynamic_cast` em C++.
* **Posse exclusiva e gerenciamento de memória (`std::unique_ptr`):** A hierarquia utiliza `std::unique_ptr` para representar as subárvores filhas. Isso garante a gestão de memória pelo padrão RAII, evitando vazamentos e garantindo a destruição automática de toda a árvore a partir do nó raiz.
* **Rastreabilidade e relatórios de erro (`line`):** Todos os nós registram a linha do código-fonte Lua original. Essa informação é vital para emissão de mensagens de erro precisas durante a compilação e mapeamento de código durante a depuração da tradução.

## 2. Categorias de Nós

### 2.1 Comandos (Stmt)

Um bloco (`Block`) é a unidade básica de execução contendo uma lista de comandos (`std::vector<StmtPtr>`). Os comandos modelados incluem:

* **`Assign`:** Atribuição múltipla (ex: `a, b = 1, 2`), em que os alvos são limitados a *lvalues* válidos em Lua (`Identifier` ou `Index`).
* **`LocalDecl`:** Declaração de variáveis locais (ex: `local a, b = 1, 2`).
* **`If` / `IfClause`:** Estrutura condicional modelada como uma lista de cláusulas condição/corpo, permitindo suportar `if`, múltiplos `elseif` e `else` (nó de condição nula).
* **Laços de Repetição:** `While`, `RepeatUntil` (cujo corpo é avaliado antes da condição, permitindo visibilidade de escopo local) e os laços `NumericFor` (com passo opcional) e `GenericFor` (com iteradores).
* **Declarações de Função:** `FunctionDecl` (global/método) e `LocalFunctionDecl` (local).
* **Outros Comandos:** `Return`, `Break`, bloco isolado `Do` e `CallStmt` (chamada de função cujo retorno é descartado).

### 2.2 Expressões (Expr)

As expressões suportadas cobrem todo o espectro necessário para a tradução das construções em Lua:

* **Literais:** `NilLiteral`, `BoolLiteral`, `NumberLiteral`, `StringLiteral` e o operador `Vararg` (`...`).
* **Identificadores e Indexação:** `Identifier` e `Index` (para acessos no formato `obj[chave]` ou `obj.campo`).
* **Operadores:** `BinaryOp` (com operadores aritméticos, lógicos e relacionais) e `UnaryOp` (negação, negação lógica e operador de tamanho `#`).
* **Construção de Tabelas (`TableConstructor`):** Modelagem flexível de tabelas Lua via `TableField`, suportando tanto elementos posicionais (arrays) quanto pares chave-valor.
* **Chamadas e Funções:** `FunctionCall` (com suporte nativo a açúcar sintático de métodos `obj:metodo()` via flag `isMethodCall`) e `FunctionExpr` (funções anônimas).

## 3. Tabela Resumo dos Tipos de Nós

| Categoria     | Estrutura / Struct          | Descrição Semântica                                                        |
| :------------ | :-------------------------- | :------------------------------------------------------------------------- |
| **Comando**   | `Block`                     | Sequência de comandos executados sequencialmente.                          |
| **Comando**   | `Assign` / `LocalDecl`      | Atribuição múltipla global ou declaração local com valores opcionais.      |
| **Comando**   | `If`                        | Cadeia de `if` / `elseif` / `else` representada por lista de `IfClause`.   |
| **Comando**   | `NumericFor` / `GenericFor` | Laços de repetição por contagem numérica ou por iterador.                  |
| **Expressão** | `TableConstructor`          | Construtor de tabela `{ ... }` contendo campos posicionais ou chave-valor. |
| **Expressão** | `FunctionCall`              | Chamada de função ou método (`isMethodCall = true`).                       |

## 4. Considerações de Arquitetura e Extensibilidade

1. **Gerenciamento de Memória:** A posse exclusiva via `std::unique_ptr` atende à fase de construção da AST e geração de código, evitando vazamentos e reduzindo overhead. Caso fases futuras exijam navegação não-hierárquica, referências fracas ou ponteiros não-proprietários poderão ser adotados pontualmente.
2. **Organização do Código:** A consolidação em cabeçalho único (`ast.hpp`) simplifica a integração entre o analisador sintático e o gerador de código, mantendo a opção de modularização em múltiplos arquivos caso o número de tipos de nó se expanda.
3. **Escopo do Subconjunto:** Recursos como saltos incondicionais (`goto` e rótulos) e metatabelas avançadas permanecem escopados fora do subconjunto inicial suportado pelo compilador.

## 5. Exemplos de Utilização da AST

Os exemplos a seguir demonstram como construções da linguagem Lua são representadas pelos nós definidos na AST.

### 5.1 Atribuição

Código Lua:

```lua
a, b = 1, 2
```

Representação na AST:

```text
Assign
├── targets
│   ├── Identifier("a")
│   └── Identifier("b")
└── values
    ├── NumberLiteral(1)
    └── NumberLiteral(2)
```

O nó `Assign` armazena os alvos da atribuição em `targets` e os valores correspondentes em `values`.

### 5.2 Estrutura condicional

Código Lua:

```lua
if x > 10 then
    x = x + 1
else
    x = 0
end
```

Representação simplificada na AST:

```text
If
├── IfClause
│   ├── condition: BinaryOp(Gt)
│   │   ├── Identifier("x")
│   │   └── NumberLiteral(10)
│   └── body: Block
│       └── Assign
│           ├── Identifier("x")
│           └── BinaryOp(Add)
│               ├── Identifier("x")
│               └── NumberLiteral(1)
└── IfClause
    ├── condition: nullptr
    └── body: Block
        └── Assign
            ├── Identifier("x")
            └── NumberLiteral(0)
```

A última `IfClause` possui `condition == nullptr`, representando o bloco `else`.

### 5.3 Chamada de função

Código Lua:

```lua
print("Olá")
```

Representação na AST:

```text
CallStmt
└── FunctionCall
    ├── callee: Identifier("print")
    └── args
        └── StringLiteral("Olá")
```

O nó `CallStmt` representa a utilização de uma chamada de função como comando, enquanto `FunctionCall` representa a chamada propriamente dita.

### 5.4 Construção de tabela

Código Lua:

```lua
local pessoa = {
    nome = "Camile",
    idade = 22
}
```

Representação simplificada na AST:

```text
LocalDecl
├── names
│   └── "pessoa"
└── values
    └── TableConstructor
        ├── TableField
        │   ├── key: StringLiteral("nome")
        │   └── value: StringLiteral("Camile")
        └── TableField
            ├── key: StringLiteral("idade")
            └── value: NumberLiteral(22)
```

Nesse caso, cada entrada da tabela é representada por um `TableField`, contendo uma chave e seu respectivo valor.

## 6. Referências

* Ierusalimschy, R.; de Figueiredo, L. H.; Celes, W. *Lua 5.4 Reference Manual*. Lua.org, PUC-Rio, 2020–2024. Disponível em: https://www.lua.org/manual/5.4/manual.html
* Aho, A. V.; Lam, M. S.; Sethi, R.; Ullman, J. D. *Compilers: Principles, Techniques, and Tools*. 2ª ed. Boston: Pearson/Addison-Wesley, 2006.
