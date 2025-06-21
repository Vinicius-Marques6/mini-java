#include "parser.h"

Parser::Parser(string input)
{
    currentST = globalST = new SymbolTable();
    initSymbolTable();

    scanner = new Scanner(input, globalST);
}

void
Parser::advance()
{
    lToken = scanner->nextToken();
}

std::string tokenToString(int t) {
    switch (t) {
        case END_OF_FILE: return "EOF";
        case LBRACE: return "{";
        case RBRACE: return "}";
        case IF: return "if";
        case ELSE: return "else";
        case WHILE: return "while";
        case SYSTEM_OUT_PRINTLN: return "System.out.println";
        case ID: return "id";
        case INTEGER: return "int";
        case TRUE: return "true";
        case FALSE: return "false";
        case THIS: return "this";
        case NEW: return "new";
        case NOT: return "!";
        case LPAREN: return "(";
        case RPAREN: return ")";
        case LBRACKET: return "[";
        case RBRACKET: return "]";
        case ASSIGN: return "=";
        case SEMICOLON: return ";";
        case DOT: return ".";
        case LENGTH: return "length";
        case PLUS: return "+";
        case MINUS: return "-";
        case MULTIPLY: return "*";
        case DIVIDE: return "/";
        case AND: return "&&";
        case LT: return "<";
        case GT: return ">";
        case EQUAL: return "==";
        case NOT_EQUAL: return "!=";
        case CLASS: return "class";
        case PUBLIC: return "public";
        case STATIC: return "static";
        case VOID: return "void";
        case MAIN: return "main";
        case STRING: return "String";
        case EXTENDS: return "extends";
        case INT: return "int";
        case BOOLEAN: return "boolean";
        case RETURN: return "return";
        case COMMA: return ",";
        default: return "undef";
    }
}

void
Parser::match(int t)
{
    if (lToken->name == t || lToken->attribute == t)
        advance();
    else
    {
        error("Esperava '" + tokenToString(t) + "', encontrado '" + lToken->lexeme + "'");
        throw 1;
    }
}

void
Parser::run()
{
    advance();	

    program();
    
    if (scanner->getHadError() || hadError)
    {
        cout << "Compilação encerrada com um ou mais erros.\n";
    } else {
        cout << "Compilação encerrada sem erros.\n";
    }
}

// Program -> MainClass (ClassDeclaration)* EOF
void
Parser::program()
{
    try
    {
        mainClass();
    
        while (lToken->name != END_OF_FILE)
        {
            classDeclaration();
        }
    
        match(END_OF_FILE);
    }
    catch (...)
    {
        set<int> synchProgram = {
            END_OF_FILE
        };

        synch(synchProgram);
    }
}

// MainClass -> class ID { public static void main(String[] ID) { Statement } }
void
Parser::mainClass()
{
    try
    {
        match(CLASS);
        Token* t = lToken;
        if (currentST->getIdentifier(t->lexeme))
        {
            warn("Classe '" + t->lexeme + "' já declarada.");
        }
        match(ID);
        currentST->add(new STEntry(t));
        currentST = new SymbolTable(currentST);
        match(LBRACE);
        match(PUBLIC);
        match(STATIC);
        match(VOID);
        match(MAIN);
        currentST = new SymbolTable(currentST);
        match(LPAREN);
        match(STRING);
        match(LBRACKET);
        match(RBRACKET);
        t = lToken;
        match(ID);
        if(currentST->getIdentifier(t->lexeme))
        {
            warn("Parâmetro '" + t->lexeme + "' já declarado na função main.");
        }
        currentST->add(new STEntry(t));
        match(RPAREN);
        match(LBRACE);
        statement();
        currentST = currentST->getParent();
        match(RBRACE);
        currentST = currentST->getParent();
        match(RBRACE);
    }
    catch (...)
    {
        set<int> synchMainClass = {
            END_OF_FILE
        };

        synch(synchMainClass);

        currentST = globalST;
    }
}

// ClassDeclaration -> class ID (extends ID)? { (VarDeclaration)* (MethodDeclaration)* }
void
Parser::classDeclaration()
{
    try {
        Token* t;
        match(CLASS);
        t = lToken;
        if (currentST->getIdentifier(t->lexeme))
        {
            warn("Classe '" + t->lexeme + "' já declarada.");
        }
        match(ID);
        currentST->add(new STEntry(t));
        if (lToken->name == EXTENDS)
        {
            advance();
            t = lToken;
            // TODO: Essa bomba vai dar aviso de classe não declarada mesmo se for outro tipo de token
            if (!currentST->getIdentifier(t->lexeme))
            {
                warn("Classe '" + t->lexeme + "' não declarada.");
            }
            match(ID);
        }
        currentST = new SymbolTable(globalST);
        match(LBRACE);
        while (lToken->name == INT || lToken->name == BOOLEAN || lToken->name == ID)
        {
            varDeclaration();
        }
        while (lToken->name == PUBLIC)
        {
            methodDeclaration();
        }
        currentST = currentST->getParent();
        match(RBRACE);
    }
    catch (...)
    {

        set<int> synchClassDec = {
            END_OF_FILE
        };

        synch(synchClassDec);

        currentST = globalST;
    }
}

// VarDeclaration -> type ID ;
void
Parser::varDeclaration()
{
    try {
        type();
        Token* t = lToken;
        if (currentST->getIdentifier(t->lexeme))
        {
            warn("Variável '" + t->lexeme + "' já declarada.");
        }
        match(ID);
        currentST->add(new STEntry(t));
        match(SEMICOLON);
    }
    catch (...)
    {
        set<int> synchVarDec = {
            PUBLIC,
            RBRACE,
            LBRACE,
            IF,
            WHILE,
            SYSTEM_OUT_PRINTLN,
            RETURN
        };

        synch(synchVarDec);
    }
}

// MethodDeclaration -> public Type ID ( (Params)? ) { (VarDeclaration)* (Statement)* return Expr ; }
void
Parser::methodDeclaration()
{
    try {
        match(PUBLIC);
        type();
        Token* t = lToken;
        if (currentST->getIdentifier(t->lexeme))
        {
            warn("Método '" + t->lexeme + "' já declarado.");
        }
        match(ID);
        currentST->add(new STEntry(t));
        currentST = new SymbolTable(currentST);
        match(LPAREN);
        try {
            if (lToken->name == INT || lToken->name == BOOLEAN || lToken->name == ID)
            {
                type();
                t = lToken;
                if (currentST->getIdentifier(t->lexeme))
                {
                    warn("Parâmetro '" + t->lexeme + "' já declarado no método.");
                }
                match(ID);
                currentST->add(new STEntry(t));
                while (lToken->name == COMMA)
                {
                    advance();
                    type();
                    t = lToken;
                    if (currentST->getIdentifier(t->lexeme))
                    {
                        warn("Parâmetro '" + t->lexeme + "' já declarado no método.");
                    }
                    match(ID);
                    currentST->add(new STEntry(t));
                }
            }
        }
        catch (...)
        {
            set<int> synchParams = {
                RPAREN
            };

            synch(synchParams);
        }
        match(RPAREN);
        match(LBRACE);
        while (lToken->name == INT || lToken->name == BOOLEAN || lToken->name == ID)
        {
            Token* peekedToken = scanner->peekToken();
            // Pode ser que este ID seja das derivações do Statement (Statement -> ID ( [ Expr ] )? = Expr ;)
            if(lToken->name == ID && (peekedToken->name == LBRACKET || peekedToken->name == ASSIGN))
            {
                delete peekedToken; // Libera o token que foi pego pelo peek
                break;
            }
            delete peekedToken;
            varDeclaration();
        }
        while (lToken->name != RETURN)
        {
            statement();
        }
        match(RETURN);
        expr();
        match(SEMICOLON);
        currentST = currentST->getParent();
        match(RBRACE);
    }
    catch (...)
    {
        currentST = currentST->getParent();

        set<int> synchMethDec = {
            RBRACE,
        };

        synch(synchMethDec);
    }
}

// Type -> int ([])? | boolean | ID
void
Parser::type()
{
    try
    {
        if (lToken->name == INT)
        {
            advance();
            if (lToken->name == LBRACKET)
            {
                advance();
                match(RBRACKET);
            }
        }
        else if (lToken->name == BOOLEAN)
        {
            advance();
        }
        else if (lToken->name == ID)
        {
            Token* t = lToken;
            if (!currentST->getIdentifier(t->lexeme))
            {
                warn("Tipo '" + t->lexeme + "' não declarado.");
            }
            advance();
        }
        else {
            error("Esperava 'int', 'boolean' ou 'ID', encontrado '" + lToken->lexeme + "'");
            throw 1;
        }
    }
    catch (...)
    {
        set<int> synchType = {
            END_OF_FILE
        };

        synch(synchType);
    }
}

// Statement -> { (Statement)* }
//            | if ( Expr ) Statement else Statement
//            | while ( Expr ) Statement
//            | System.out.println( Expr ) ;
//            | ID ( [ Expr ] )? = Expr ;
void
Parser::statement()
{
    try
    {
        if (lToken->name == LBRACE)
        {
            currentST = new SymbolTable(currentST);
            advance();
            while (lToken->name != RBRACE)
            {
                statement();
            }
            currentST = currentST->getParent();
            match(RBRACE);
        }
        else if (lToken->name == IF)
        {
            advance();
            match(LPAREN);
            expr();
            match(RPAREN);
            statement();
            match(ELSE);
            statement();
        }
        else if (lToken->name == WHILE)
        {
            advance();
            match(LPAREN);
            expr();
            match(RPAREN);
            statement();
        }
        else if (lToken->name == SYSTEM_OUT_PRINTLN)
        {
            advance();
            match(LPAREN);
            expr();
            match(RPAREN);
            match(SEMICOLON);
        }
        else if (lToken->name == ID)
        {
            Token* t = lToken;
            if (!currentST->getIdentifier(t->lexeme))
            {
                warn("Variável '" + t->lexeme + "' não declarada.");
            }
            advance();
            if (lToken->name == LBRACKET)
            {
                advance();
                expr();
                match(RBRACKET);
            }
            match(ASSIGN);
            expr();
            match(SEMICOLON);
        }
        else
        {
            error("Esperava uma instrução, encontrado '" + lToken->lexeme + "'");
            throw 1;
        }
    }
    catch (...)
    {
        // Aqui é um apelo para os escopos não se perderem.
        if (lToken->name != RBRACE)
        {
            currentST = currentST->getParent();
        }

        set<int> synchStmt = {
            RBRACE,
            ELSE,
            RETURN
        };

        synch(synchStmt);
    }
}

// Expr -> INTEGER ExprLinha
//       | true ExprLinha
//       | false ExprLinha
//       | ID ExprLinha
//       | this ExprLinha
//       | new NewSuffix ExprLinha
//       | ! Expr ExprLinha
//       | ( Expr ) ExprLinha
//
// NewSuffix -> int [ Expr ]
//            | ID ()
void
Parser::expr()
{
    try
    {
        if (lToken->name == INTEGER)
        {
            advance();
            exprLinha();
        }
        else if (lToken->name == TRUE)
        {
            advance();
            exprLinha();
        }
        else if (lToken->name == FALSE)
        {
            advance();
            exprLinha();
        }
        else if (lToken->name == ID)
        {
            Token* t = lToken;
            if (!currentST->getIdentifier(t->lexeme))
            {
                warn("Variável '" + t->lexeme + "' não declarada.");
            }
            advance();
            exprLinha();
        }
        else if (lToken->name == THIS)
        {
            advance();
            exprLinha();
        }
        else if (lToken->name == NEW)
        {
            advance();
            if (lToken->name == INT)
            {
                advance();
                match(LBRACKET);
                expr();
                match(RBRACKET);
            }
            else if (lToken->name == ID)
            {
                Token* t = lToken;
                if (!currentST->getIdentifier(t->lexeme))
                {
                    warn("Classe '" + t->lexeme + "' não declarada.");
                }
                advance();
                match(LPAREN);
                match(RPAREN);
            }
            else
            {
                error("Esperava 'int' ou 'ID' após 'new', encontrado '" + lToken->lexeme + "'");
                throw 1;
            }
            exprLinha();
        }
        else if (lToken->name == NOT)
        {
            advance();
            expr();
            exprLinha();
        }
        else if (lToken->name == LPAREN)
        {
            advance();
            expr();
            match(RPAREN);
            exprLinha();
        }
        else
        {
            error("Esperava uma expressão, encontrado '" + lToken->lexeme + "'");
            throw 1;
        }
    }
    catch (...)
    {
        set<int> synchExpr = {
            AND,
            LT,
            GT,
            PLUS,
            MINUS,
            MULTIPLY,
            DIVIDE,
            EQUAL,
            NOT_EQUAL,
            RPAREN,
            LBRACKET,
            RBRACKET,
            SEMICOLON,
            DOT,
        };

        synch(synchExpr);

        exprLinha();
    }
}

// ExprLinha -> Op Expr ExprLinha
//            | [ Expr ] ExprLinha
//            | . DotSuffix ExprLinha
//            | e
//
// DotSuffix -> length
//            | ID ( ( ExprList )? )
void
Parser::exprLinha()
{
    try
    {
        if (lToken->name == DOT)
        {
            try
            {
                advance();
                if (lToken->name == LENGTH)
                {
                    advance();
                }
                else if (lToken->name == ID)
                {
                    Token* t = lToken;
                    if (!currentST->getIdentifier(t->lexeme))
                    {
                        warn("Método '" + t->lexeme + "' não declarado.");
                    }
                    advance();
                    match(LPAREN);
                    if (lToken->name == INTEGER || lToken->name == TRUE || lToken->name == FALSE || lToken->name == ID || lToken->name == THIS || lToken->name == NEW || lToken->name == NOT || lToken->name == LPAREN)
                    {
                        exprList();
                    }
                    match(RPAREN);
                }
                else
                {
                    error("Esperava 'length' ou 'ID' após '.', encontrado '" + lToken->lexeme + "'");
                    throw 1;
                }
                exprLinha();
            }
            catch (...)
            {
                set<int> synchDotSuffix = {
                    AND,
                    LT,
                    GT,
                    PLUS,
                    MINUS,
                    MULTIPLY,
                    DIVIDE,
                    EQUAL,
                    NOT_EQUAL,
                    RPAREN,
                    LBRACKET,
                    RBRACKET,
                    SEMICOLON,
                    DOT
                };

                synch(synchDotSuffix);
            }
        }
        else if (lToken->name == PLUS || lToken->name == MINUS || lToken->name == MULTIPLY || lToken->name == DIVIDE || lToken->name == AND || lToken->name == LT || lToken->name == GT || lToken->name == EQUAL || lToken->name == NOT_EQUAL)
        {
            advance();
            expr();
            exprLinha();
        }
        else if (lToken->name == LBRACKET)
        {
            advance();
            expr();
            match(RBRACKET);
            exprLinha();
        }
    }
    catch (...)
    {
        set<int> synchExpr = {
            RPAREN,
            RBRACKET,
            SEMICOLON,
        };

        synch(synchExpr);
    }
}

// ExprList -> Expr ( , Expr )*
void
Parser::exprList()
{
    try
    {
        if (lToken->name == INTEGER || lToken->name == TRUE || lToken->name == FALSE || lToken->name == ID || lToken->name == THIS || lToken->name == NEW || lToken->name == NOT || lToken->name == LPAREN)
        {
            expr();
            while (lToken->name == COMMA)
            {
                advance();
                expr();
            }
        }
        else if (lToken->name != RPAREN)
        {
            error("Esperava uma expressão ou ')', encontrado '" + lToken->lexeme + "'");
            throw 1;
        }
    }
    catch (...)
    {
        set<int> synchExprList = {
            RPAREN
        };

        synch(synchExprList);
    }
}

void
Parser::initSymbolTable()
{
    Token* t;

    t = new Token(BOOLEAN, "boolean");
    globalST->add(new STEntry(t, true));
    t = new Token(CLASS, "class");
    globalST->add(new STEntry(t, true));
    t = new Token(ELSE, "else");
    globalST->add(new STEntry(t, true));
    t = new Token(EXTENDS, "extends");
    globalST->add(new STEntry(t, true));
    t = new Token(FALSE, "false");
    globalST->add(new STEntry(t, true));
    t = new Token(IF, "if");
    globalST->add(new STEntry(t, true));
    t = new Token(INT, "int");
    globalST->add(new STEntry(t, true));
    t = new Token(LENGTH, "length");
    globalST->add(new STEntry(t, true));
    t = new Token(MAIN, "main");
    globalST->add(new STEntry(t, true));
    t = new Token(NEW, "new");
    globalST->add(new STEntry(t, true));
    t = new Token(PUBLIC, "public");
    globalST->add(new STEntry(t, true));
    t = new Token(RETURN, "return");
    globalST->add(new STEntry(t, true));
    t = new Token(STATIC, "static");
    globalST->add(new STEntry(t, true));
    t = new Token(STRING, "String");
    globalST->add(new STEntry(t, true));
    t = new Token(THIS, "this");
    globalST->add(new STEntry(t, true));
    t = new Token(TRUE, "true");
    globalST->add(new STEntry(t, true));
    t = new Token(VOID, "void");
    globalST->add(new STEntry(t, true));
    t = new Token(WHILE, "while");
    globalST->add(new STEntry(t, true));
}

void
Parser::synch(set<int> synchSet)
{
    /*
     * É tentando implementar o synch que me despeço da minha sanidade.
    */
    if (!hadError)
        hadError = true;

    while (synchSet.find(lToken->name) == synchSet.end() && lToken->name != END_OF_FILE)
    {
        advance();
    }
}

void
Parser::error(string str)
{
    const int line = scanner->getLine();
    const int column = scanner->getColumn();
    const int lexemeLen = lToken ? (int)lToken->lexeme.length() : 1;
    string lineInput = scanner->getLineInput(line);

    cout << FILE_NAME << scanner->getFileName() << ":" << line << ":" << column << ": " << ERROR << "erro" << ": " << RESET << str << endl;

    if (lineInput.empty())
        return;

    cout << LINE << setw(5) << line << " | " << RESET << lineInput;
    if (lineInput.back() != '\n')
        cout << endl;
    cout << LINE << setw(5) << line + 1 << " | " << ERROR << string(column - lexemeLen, ' ');
    if (lexemeLen > 1)
        cout << string(lexemeLen - 1, '~');
    cout << "^" << RESET << endl;

    //exit(EXIT_FAILURE);
}


void
Parser::warn(string str)
{
    const int line = scanner->getLine();
    const int column = scanner->getColumn();
    const int lexemeLen = lToken ? (int)lToken->lexeme.length() : 1;
    string lineInput = scanner->getLineInput(line);

    cout << FILE_NAME << scanner->getFileName() << ":" << line << ":" << column << ": " << WARNING << "aviso" << ": " << RESET << str << endl;

    if (lineInput.empty())
        return;

    cout << LINE << setw(5) << line << " | " << RESET << lineInput;
    if (lineInput.back() != '\n')
        cout << endl;
    cout << LINE << setw(5) << line + 1 << " | " << WARNING << string(column - lexemeLen, ' ');
    if (lexemeLen > 1)
        cout << string(lexemeLen - 1, '~');
    cout << "^" << RESET << endl;

}