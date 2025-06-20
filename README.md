# MiniJava

MiniJava é um compilador contruído como trabalho prático para a disciplina de Compiladores I do curso de Ciência da Computação UFMS.

## Estrutura do Projeto

- `scanner.h/cpp`: Implementa o analisador léxico (Scanner) que identifica tokens no código fonte
- `token.h`: Define os tipos de tokens suportados pela linguagem
- `parser.h/cpp`: Implementa o analisador sintático (Parser) que valida a estrutura do programa
- `principal.cpp`: Contém o ponto de entrada do programa

## Como Usar

1. Compile o projeto:
```
g++ -o compiler principal.cpp parser.cpp scanner.cpp
```

2. Execute o compilador passando um arquivo fonte `.mj` como argumento:
```
./compiler arquivo.mj
```

## Formato dos Arquivos Fonte

Os arquivos fonte devem ter a extensão `.mj` e seguir a sintaxe do Mini-Java. Por exemplo:

```java
int a = 2;
int b = 4;
int c = a + b;

if (c > 0) {
    System.out.println(c);
}
```

FIRST(Program) = FIRST(MainClass) = {class} \
FIRST(MainClass) = {class} \
FIRST(ClassDec.) = {class} \
FIRST(VarDec.) = FIRST(Type) = {int, boolean, ID} \
FIRST(MethodDec.) = {public} \
FIRST(Params) = FIRST(Type) = {int, boolean, ID} \
FIRST(Type) = {int, boolean, ID} \
FIRST(Stmt) = { {, if, while, ID, System.out.println } \
FIRST(Expr) = { INTEGER, true, false, ID, this, new, !, ( } \
FIRST(ExprLinha) = FIRST(Op) U { [, ., e } = { &&, <, >, ==, !=, +, -, *, /, [, ., e} \
FIRST(NewSuffix) = {int, ID} \
FIRST(DotSuffix) = {length, ID} \
FIRST(Op) = { &&, <, >, ==, !=, +, -, *, /} \
FIRST(ExprList) = FIRST(Expr) = { INTEGER, true, false, ID, this, new, !, ( }

FOLLOW(Program) = {EOF} \
FOLLOW(MainClass) = FIRST(ClassDec.) U {EOF} = {class, EOF} \
FOLLOW(ClassDec.) = FIRST(ClassDec.) U {EOF} = {class, EOF} \
FOLLOW(VarDec.) = FIRST(VarDec.) U FIRST(MethodDec.) U {}} U FIRST(Stmt) U {return} = {int, boolean, ID, public, }, {, if, while, ID, System.out.println, return} \
FOLLOW(MethodDec.) = FIRST(MethodDec.) U {}} = {public, }} \
FOLLOW(Params) = {)} \
FOLLOW(Type) = {ID} \
FOLLOW(Stmt) = FIRST(Stmt) U {}, else, return} = {{, if, while, ID, Sytem.out.println, }, else, return} \
FOLLOW(Expr) = {;, ), ]} U FIRST(ExprLinha) = {;, ), ], &&, <, >, ==. !=, +, -, *, /, [, ., } \  
FOLLOW(NewSuffix) = FOLLOW(Expr) = {;, ), ], &&, <, >, ==, !=, +, -, *, /, [, ., } \  
FOLLOW(DotSuffix) = FOLLOW(ExprLinha) = {;, ), ], &&, <, >, ==. !=, +, -, *, /, [, ., } \
FOLLOW(Op) = FIRST(E) = {INTEGER, true, false, ID, this, new, !, ( } \
FOLLOW(ExprLinha) = FOLLOW(Expr) U FOLLOW(NewSuffix) U FOLLOW(DotSuffix) = {;, ), ], &&, <, >, ==. !=, +, -, *, /, [, ., } \
FOLLOW(ExprList) = { ) }

