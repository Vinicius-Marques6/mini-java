#include "scanner.h"    

//Construtor que recebe uma string com o nome do arquivo 
//de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string input)
{
    /*this->input = input;
    cout << "Entrada: " << input << endl << "Tamanho: " 
         << input.length() << endl;*/
    pos = 0;
    line = 1;

    ifstream inputFile(input, ios::in);
    string line;

    if (inputFile.is_open())
    {
        while (getline(inputFile,line) )
        {
            this->input.append(line + '\n');
        }
        inputFile.close();
    }
    else 
        cout << "Unable to open file\n"; 

    //A próxima linha deve ser comentada posteriormente.
    //Ela é utilizada apenas para verificar se o 
    //preenchimento de input foi feito corretamente.
    cout << this->input;

}

int
Scanner::getLine()
{
    return line;
}

//Método que retorna o próximo token da entrada
Token* 
Scanner::nextToken()
{
    Token* tok;
    string lexeme;

    while(true)
    {
        if (input[pos] == '\n')
        {
            line++;
            pos++;
            continue;
        }
    
        while(isspace(input[pos]))
        {
            pos++;
        }
    
        // Comentários
        if (input[pos] == '/' && input[pos + 1] == '/')
        {
            while (input[pos] != '\n' && input[pos] != '\0')
            {
                pos++;
            }
            continue;
        }
        else if (input[pos] == '/' && input[pos + 1] == '*')
        {
            pos += 2;
            while (true)
            {
                if (input[pos] == '\0')
                {
                    lexicalError("Fim de arquivo inesperado");
                }
                if (input[pos] == '*' && input[pos + 1] == '/')
                {
                    pos += 2;
                    break;
                }
                if (input[pos] == '\n')
                    line++;
                pos++;
            }
            continue;
        }
        break;
    }

    //Fim de arquivo
    if (input[pos] == '\0')
    {
        tok = new Token(END_OF_FILE);
    }
    // Operadores aritméticos
    else if (input[pos] == '+')
    {
        pos++;
        tok = new Token(PLUS);
    }
    else if (input[pos] == '-')
    {
        pos++;
        tok = new Token(MINUS);
    }
    else if (input[pos] == '*')
    {
        pos++;
        tok = new Token(MULTIPLY);
    }
    else if (input[pos] == '/')
    {
        pos++;
        tok = new Token(DIVIDE);
    }
    // Operadores lógicos
    else if (input[pos] == '<')
    {
        pos++;
        tok = new Token(LT);
    }
    else if (input[pos] == '>')
    {
        pos++;
        tok = new Token(GT);
    }
    else if (input[pos] == '=')
    {
        pos++;
        if (input[pos] == '=')
        {
            pos++;
            tok = new Token(EQUAL);
        }
        else
        {
            tok = new Token(ASSIGN);
        }
    }
    else if (input[pos] == '!')
    {
        pos++;
        if (input[pos] == '=')
        {
            pos++;
            tok = new Token(NOT_EQUAL);
        }
        else
        {
            tok = new Token(NOT);
        }
    }
    else if (input[pos] == '&')
    {
        pos++;
        if (input[pos] == '&')
        {
            pos++;
            tok = new Token(AND);
        }
        else
        {
            lexicalError("Esperado '&' após '&'");
        }
    }
    // Separadores
    else if (input[pos] == '(')
    {
        pos++;
        tok = new Token(LPAREN);
    }
    else if (input[pos] == ')')
    {
        pos++;
        tok = new Token(RPAREN);
    }
    else if (input[pos] == '{')
    {
        pos++;
        tok = new Token(LBRACE);
    }
    else if (input[pos] == '}')
    {
        pos++;
        tok = new Token(RBRACE);
    }
    else if (input[pos] == '[')
    {
        pos++;
        tok = new Token(LBRACKET);
    }
    else if (input[pos] == ']')
    {
        pos++;
        tok = new Token(RBRACKET);
    }
    else if (input[pos] == ';')
    {
        pos++;
        tok = new Token(SEMICOLON);
    }
    else if (input[pos] == '.')
    {
        pos++;
        tok = new Token(DOT);
    }
    else if (input[pos] == ',')
    {
        pos++;
        tok = new Token(COMMA);
    }

    // Identificadores e palavras reservadas
    else if (isalpha(input[pos]) || input[pos] == '_')
    {
        if (input.substr(pos, 18) == "System.out.println")
        {
            pos += 18;
            tok = new Token(SYSTEM_OUT_PRINTLN);
        }
        else
        {
            while (isalnum(input[pos]) || input[pos] == '_')
            {
                lexeme += input[pos];
                pos++;
            }
            if (lexeme == "boolean")
                tok = new Token(BOOLEAN, lexeme);
            else if (lexeme == "class")
                tok = new Token(CLASS, lexeme);
            else if (lexeme == "else")
                tok = new Token(ELSE, lexeme);
            else if (lexeme == "extends")
                tok = new Token(EXTENDS, lexeme);
            else if (lexeme == "false")
                tok = new Token(FALSE, lexeme);
            else if (lexeme == "if")
                tok = new Token(IF, lexeme);
            else if (lexeme == "int")
                tok = new Token(INT, lexeme);
            else if (lexeme == "length")
                tok = new Token(LENGTH, lexeme);
            else if (lexeme == "main")
                tok = new Token(MAIN, lexeme);
            else if (lexeme == "new")
                tok = new Token(NEW, lexeme);
            else if (lexeme == "public")
                tok = new Token(PUBLIC, lexeme);
            else if (lexeme == "return")
                tok = new Token(RETURN, lexeme);
            else if (lexeme == "static")
                tok = new Token(STATIC, lexeme);
            else if (lexeme == "String")
                tok = new Token(STRING, lexeme);
            else if (lexeme == "this")
                tok = new Token(THIS, lexeme);
            else if (lexeme == "true")
                tok = new Token(TRUE, lexeme);
            else if (lexeme == "void")
                tok = new Token(VOID, lexeme);
            else if (lexeme == "while")
                tok = new Token(WHILE, lexeme);
            else
                tok = new Token(ID, lexeme);
        }
    }
    // Inteiros
    else if (isdigit(input[pos]))
    {
        while (isdigit(input[pos]))
        {
            lexeme += input[pos];
            pos++;
        }
        tok = new Token(INTEGER, lexeme);
    }
    else
    {
        lexicalError("Caractere inválido: " + string(1, input[pos]));
    }


    return tok;

}

void 
Scanner::lexicalError(string msg)
{
    cout << "Linha "<< line << ": " << msg << endl;
    
    exit(EXIT_FAILURE);
}
