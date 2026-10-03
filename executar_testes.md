# Como Rodar e Testar o Analisador Léxico

Este guia explica como executar o nosso compilador para testar a etapa de Análise Léxica, focando na **Issue #22 (Tratamento de erros léxicos)**.

## Onde rodar os comandos?
Você deve sempre rodar os comandos no **diretório raiz do projeto** (onde fica o arquivo executável `teste`).
No seu caso, garanta que o terminal esteja no seguinte caminho:
```bash
/home/'insira o seu usuário'/Documentos/comp/GRUPO2-COMP2026.2
```

## Como executar um teste
O compilador lê o código de entrada (arquivos `.lua`) através do redirecionamento de arquivo do Linux (`<`). 

Para testar, utilize a sintaxe:
```bash
./teste < "caminho/do/arquivo.lua"
```

### 1. Rodando os Testes de Erro (Foco da Issue #22)
Estes testes servem para garantir que o compilador não quebra, mas sim avisa graciosamente quando há caracteres inválidos ou strings que não fecharam.

Abra o terminal na pasta raiz do projeto e rode um por um para ver o resultado:

**Teste 1: String não terminada (falta fechar aspas)**
```bash
./teste < "Testes/Testes errados/Teste1(TokenInválido).lua"
```

**Teste 2: Nome de variável começando com número**
```bash
./teste < "Testes/Testes errados/Teste2(TokenInválido).lua"
```

**Teste 3: Número flutuante malformado**
```bash
./teste < "Testes/Testes errados/Teste3(TokenInvalido).lua"
```

**Teste 4: String longa de Lua sem fechamento**
```bash
./teste < "Testes/Testes errados/Teste4(TokenInvalido).lua"
```

**Teste 5: Múltiplos erros no mesmo arquivo**
```bash
./teste < "Testes/Testes errados/Teste5(TokenInvalido).lua"
```

> **Aviso sobre os Números na Tela:**
> Você notará que o terminal imprimirá vários números (como `57`, `31`, etc) antes de mostrar o erro. Estes números foram adicionados propositalmente pelo grupo como um rastreio (debug) para acompanhar qual Token o Lexer (Flex) está reconhecendo em tempo real. Isso será removido mais para o fim do projeto para deixar a saída limpa.

### 2. Rodando Testes Certos
Para ver o compilador funcionando com códigos Lua válidos, sem acusar erros, execute:

```bash
./teste < "Testes/Testes certos/Teste1(HelloWorld).lua"
./teste < "Testes/Testes certos/Teste2(Variáveis).lua"
./teste < "Testes/Testes certos/Teste3(Tokens válidos).lua"
```
