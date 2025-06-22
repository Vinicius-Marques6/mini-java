# MiniJava

MiniJava é um compilador contruído como trabalho prático para a disciplina de Compiladores I do curso de Ciência da Computação UFMS.
#### Participantes:
- Vinícius Marques Diniz
- Mateus de Oliveira Sposo
- Luis Alberto Kenji Imazaki

## Como Usar

1. Compile o projeto:
```
g++ -o mj_compiler *.cpp
```

2. Execute o compilador passando um arquivo fonte `.mj` como argumento:
```
./mj_compiler arquivo.mj
```

# Anexo

## Conjunto FIRST e FOLLOW

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

