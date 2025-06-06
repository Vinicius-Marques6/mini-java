#include <fstream>
#include "token.h"

class Scanner 
{
    private: 
        string input;//Armazena o texto de entrada
        int pos;//Posição atual
        int line;
        int column;
    
    public:
    //Construtor
        Scanner(string);

        int getLine();
        int getColumn();
    
        //Método que retorna o próximo token da entrada
        Token* nextToken();
        //Método que avança para o próximo caractere
        void advance();     
    
        //Método para manipular erros
        void lexicalError(string);
};
