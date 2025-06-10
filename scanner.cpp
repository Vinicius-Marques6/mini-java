#include "scanner.h"

//Construtor que recebe uma string com o nome do arquivo 
//de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string input, SymbolTable* table)
{
    fileName = input;
    /*this->input = input;
    cout << "Entrada: " << input << endl << "Tamanho: " 
         << input.length() << endl;*/
    pos = 0;
    line = 1;
    column = 0;

    st = table;

    ifstream inputFile(input, ios::in);

    if (inputFile.is_open())
    {
        string line;
        size_t currentOffset = 0;
        while (getline(inputFile,line) )
        {
            this->lineOffsets.push_back(currentOffset);
            this->input.append(line + '\n');
            currentOffset = this->input.length();
        }
        inputFile.close();
    }
    else 
        cout << "Unable to open file\n"; 

    //A próxima linha deve ser comentada posteriormente.
    //Ela é utilizada apenas para verificar se o 
    //preenchimento de input foi feito corretamente.
    //cout << this->input;

}

int
Scanner::getLine()
{
    return line;
}

int
Scanner::getColumn()
{
    return column;
}

string
Scanner::getFileName()
{
    return fileName;
}

string
Scanner::getLineInput(int lineNumber)
{
    if (lineNumber < 1 || lineNumber > lineOffsets.size()) {
        return "";
    }
    
    size_t start = lineOffsets[lineNumber - 1];
    size_t end = (lineNumber < lineOffsets.size()) ? lineOffsets[lineNumber] : input.length();
    
    return input.substr(start, end - start);
}

//Método que retorna o próximo token da entrada
Token* 
Scanner::nextToken()
{
    Token* tok;
    string lexeme;

    while(true)
    {
        while(isspace(input[pos]))
        {
            advance();
        }
    
        // Comentários
        if (input[pos] == '/' && input[pos + 1] == '/')
        {
            advance(2);
            while (input[pos] != '\n' && input[pos] != '\0')
            {
                advance();
            }
            continue;
        }
        else if (input[pos] == '/' && input[pos + 1] == '*')
        {
            advance(2);
            while (true)
            {
                if (input[pos] == '\0')
                {
                    lexicalError("Fim de arquivo inesperado");
                }
                if (input[pos] == '*' && input[pos + 1] == '/')
                {
                    advance(2);
                    break;
                }
                advance();
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
        advance();
        tok = new Token(PLUS, "+");
    }
    else if (input[pos] == '-')
    {
        advance();
        tok = new Token(MINUS, "-");
    }
    else if (input[pos] == '*')
    {
        advance();
        tok = new Token(MULTIPLY, "*");
    }
    else if (input[pos] == '/')
    {
        advance();
        tok = new Token(DIVIDE, "/");
    }
    // Operadores lógicos
    else if (input[pos] == '<')
    {
        advance();
        tok = new Token(LT, "<");
    }
    else if (input[pos] == '>')
    {
        advance();
        tok = new Token(GT, ">");
    }
    else if (input[pos] == '=')
    {
        advance();
        if (input[pos] == '=')
        {
            advance();
            tok = new Token(EQUAL, "==");
        }
        else
        {
            tok = new Token(ASSIGN, "=");
        }
    }
    else if (input[pos] == '!')
    {
        advance();
        if (input[pos] == '=')
        {
            advance();
            tok = new Token(NOT_EQUAL, "!=");
        }
        else
        {
            tok = new Token(NOT, "!");
        }
    }
    else if (input[pos] == '&')
    {
        advance();
        if (input[pos] == '&')
        {
            advance();
            tok = new Token(AND, "&&");
        }
        else
        {
            lexicalError("Esperado '&' após '&'");
        }
    }
    // Separadores
    else if (input[pos] == '(')
    {
        advance();
        tok = new Token(LPAREN, "(");
    }
    else if (input[pos] == ')')
    {
        advance();
        tok = new Token(RPAREN, ")");
    }
    else if (input[pos] == '{')
    {
        advance();
        tok = new Token(LBRACE, "{");
    }
    else if (input[pos] == '}')
    {
        advance();
        tok = new Token(RBRACE, "}");
    }
    else if (input[pos] == '[')
    {
        advance();
        tok = new Token(LBRACKET, "[");
    }
    else if (input[pos] == ']')
    {
        advance();
        tok = new Token(RBRACKET, "]");
    }
    else if (input[pos] == ';')
    {
        advance();
        tok = new Token(SEMICOLON, ";");
    }
    else if (input[pos] == '.')
    {
        advance();
        tok = new Token(DOT, ".");
    }
    else if (input[pos] == ',')
    {
        advance();
        tok = new Token(COMMA, ",");
    }

    // Identificadores e palavras reservadas
    else if (isalpha(input[pos]) || input[pos] == '_')
    {
        if (input.substr(pos, 18) == "System.out.println")
        {
            advance(18);
            tok = new Token(SYSTEM_OUT_PRINTLN, "System.out.println");
        }
        else
        {
            while (isalnum(input[pos]) || input[pos] == '_')
            {
                lexeme += input[pos];
                advance();
            }
            //Pesquisar na tabela de palavras reservadas
            //caso encontrem, retornem o token adequado,
            //caso contrário, é ID
            STEntry* obj = st->get(lexeme);
            if (!obj)
                tok = new Token(ID, lexeme);
            else 
                tok = new Token(obj->token->name, obj->token->lexeme);
        }
    }
    // Inteiros
    else if (isdigit(input[pos]))
    {
        while (isdigit(input[pos]))
        {
            lexeme += input[pos];
            advance();
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
Scanner::advance()
{
    if (input[pos] == '\n') {
        line++;
        column = 0;
    } else {
        column++;
    }
    pos++;
}

void
Scanner::advance(const int i)
{
    for (int j = 0; j < i; ++j) {
        advance();
    }
}

void 
Scanner::lexicalError(string msg)
{
    cout << fileName << ":" << line << ":" << column <<": " << msg << endl;

    cout << setw(5) << line << " | " << getLineInput(line) << endl;
    cout << setw(5) << line + 1 << " | " << string(column - 1, ' ') << "^" << endl;
    
    exit(EXIT_FAILURE);
}
