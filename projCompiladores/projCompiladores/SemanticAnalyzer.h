#include <vector>

using namespace std;

class SemanticAnalyzer {
public:
	/* Funcoes de Classe */
	SemanticAnalyzer(string filename);
	~SemanticAnalyzer();
	/* Funcoes de Vetores */
	string getClass(int index);
	string getToken(int index);
	string getLinha(int index);
	string getTipo(string token);
	bool isThere(string token);
	bool isSomewhere(string token);
	void removeMark();
	void removeTipo();
	/* Funções de Semântica */
	string checkTable(string op, string val);
	string checkTable(string op, string valLeft, string valRight);
	/* Funcoes de Sintaxe */
	void analyze();
	int programa(int index);
	int declaracoes_variaveis(int index);
	int lista_declaracoes_variaveis(int index);
	int lista_declaracoes_variaveis_auxiliar(int index);
	int lista_de_identificadores(int index);
	int lista_de_identificadores_auxiliar(int index);
	int tipo(int index);
	int declaracoes_de_subprogramas(int index);
	int declaracoes_de_subprogramas_auxiliar(int index);
	int declaracao_de_subprograma(int index);
	int argumentos(int index);
	int lista_de_parametros(int index);
	int lista_de_parametros_auxiliar(int index);
	int comando_composto(int index);
	int comandos_opcionais(int index);
	int lista_de_comandos(int index);
	int lista_de_comandos_auxiliar(int index);
	int comando(int index, vector<string> pct);
	int parte_else(int index);
	int variavel(int index, vector<string> pct);
	int ativacao_de_procedimentos(int index, vector<string> pct);
	int lista_de_expressoes(int index, vector<string> pct);
	int lista_de_expressoes_auxiliar(int index, vector<string> pct);
	int expressao(int index, vector<string> pct);
	int expressao_simples(int index, vector<string> pct);
	int expressao_simples_auxiliar(int index, vector<string> pct);
	int termo(int index, vector<string> pct);
	int termo_auxiliar(int index, vector<string> pct);
	int fator(int index, vector<string> pct);
	int sinal(int index, vector<string> pct);
	int op_relacional(int index, vector<string> pct);
	int op_aditivo(int index, vector<string> pct);
	int op_multiplicativo(int index, vector<string> pct);
private:
	vector<string> *tokens;
	vector<string> *classes;
	vector<string> *linhas;
	vector<string> *pilha;
	vector<string> *tipos;
};