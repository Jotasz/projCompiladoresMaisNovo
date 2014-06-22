#include <fstream>
#include <vector>

using namespace std;

class lexToken {

private:
	unsigned int linha;
	string nome;
	string classificacao;

public:
	lexToken(unsigned int line, string name, string classif){
		linha = line;
		nome = name;
		classificacao = classif;
	}
	unsigned int getLine(){
		return linha;
	}
	string getName(){
		return nome;
	}
	string getClassif(){
		return classificacao;
	}
};


class LexicalAnalyzer{
public:
	LexicalAnalyzer();
	~LexicalAnalyzer();
	void analyze(ifstream *code);
	void writeOutput(ofstream *output);
	bool checkRestriction(string word);
	bool isLetter(char symbol);
	bool isNumber(char symbol);
private:
	/* Tabela de Léxicos */
	vector<lexToken> *token;
	/* Lista de Palavras Reservadas */
	vector<string> *restricted_word;
};