#include <vector>

using namespace std;

class SyntacticAnalyzer {
public:
	/* Funcoes de Classe */
	SyntacticAnalyzer(string filename);
	~SyntacticAnalyzer();
	/* Funcoes de Leitura */
	string getClass(int index);
	string getToken(int index);
	string getLinha(int index);
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
	int comando(int index);
	int parte_else(int index);
	int variavel(int index);
	int ativacao_de_procedimentos(int index);
	int lista_de_expressoes(int index);
	int lista_de_expressoes_auxiliar(int index);
	int expressao(int index);
	int expressao_simples(int index);
	int expressao_simples_auxiliar(int index);
	int termo(int index);
	int termo_auxiliar(int index);
	int fator(int index);
	int sinal(int index);
	int op_relacional(int index);
	int op_aditivo(int index);
	int op_multiplicativo(int index);
private:
	vector<string> *tokens;
	vector<string> *classes;
	vector<string> *linhas;
};