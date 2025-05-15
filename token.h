#include <iostream>
#include <ctype.h>//Funções de caracteres
#include <string>

using namespace std;

enum Names 
{
    UNDEF,
    ID,
    INTEGER,            // [0-9]+

    // Operadores
    AND,                // &&
    LT,                 // <
    GT,                 // >
    PLUS,               // +
    MINUS,              // -
    MULTIPLY,           // *
    DIVIDE,             // /
    ASSIGN,             // =
    EQUAL,              // ==
    NOT_EQUAL,          // !=
    NOT,                // !

    // Separadores
    LPAREN,             // (    
    RPAREN,             // )
    LBRACE,             // {
    RBRACE,             // }
    LBRACKET,           // [
    RBRACKET,           // ]
    SEMICOLON,          // ;
    DOT,                // .
    COMMA,              // ,

    // Palavras reservadas
    BOOLEAN,
    CLASS,
    ELSE,
    EXTENDS,
    FALSE,
    IF,
    INT,
    LENGTH,
    MAIN,
    NEW,
    PUBLIC,
    RETURN,
    STATIC,
    STRING,
    SYSTE_OUT_PRINTLN,
    THIS,
    TRUE,
    VOID,
    WHILE,

    END_OF_FILE
};

class Token 
{
    public: 
        int name;
        int attribute;
        string lexeme;
    
        Token(int name)
        {
            this->name = name;
            attribute = UNDEF;
        }

        Token(int name, string l)
        {
            this->name = name;
            attribute = UNDEF;
            lexeme = l;
        }
        
        Token(int name, int attr)
        {
            this->name = name;
            attribute = attr;
        }
};
