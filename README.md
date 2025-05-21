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

