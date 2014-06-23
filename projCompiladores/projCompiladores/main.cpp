#include <fstream>
#include <iostream>
#include <list>
#include <vector>
#include "LexicalAnalyzer.h"
#include "SyntacticAnalyzer.h"
#include "SemanticAnalyzer.h"

using namespace std;

int main(int n_arg, char** args){
	//Variáveis e Ponteiros
	ifstream *code = NULL;
	ofstream *lexc = NULL;
	char input[256] = "programa.txt", output[256] = "programa.lex";

	code = new ifstream(input);
	
	if (code == NULL){
		cout << "Arquivo invalido" << endl;
	}

	lexc = new ofstream(output);
	
	//Análise Léxica
	LexicalAnalyzer *lex = new LexicalAnalyzer();
	lex->analyze(code);
	lex->writeOutput(lexc);

	//Fechamento de Arquivos
	code->close();
	lexc->close();

	//Remoções
	delete lex;
	delete lexc;
	delete code;

	//Análise Semântica
	SemanticAnalyzer *sa = new SemanticAnalyzer("programa.lex");
	sa->analyze();
	delete sa;
	//Retorno
	return 0;
}