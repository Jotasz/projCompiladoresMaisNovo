#include <cstring>
#include <fstream>
#include <string>
#include "SyntacticAnalyzer.h"
#include <iostream>
using namespace std;

/* Funcoes de Classe */
SyntacticAnalyzer::SyntacticAnalyzer(string filename){
	ifstream *file = new ifstream(filename.c_str());
	string leitura;
	tokens  = new vector<string>();
	classes = new vector<string>();
	linhas  = new vector<string>();


	/* Leitura de Cabeçalho */
	getline(*file, leitura);
	getline(*file, leitura);
	//getline(*file, leitura);

	/* Leitura de Arquivo */
	getline(*file, leitura);
	while (!file->eof()){
		char *token;
		char *str = new char[leitura.length()];
		strcpy(str,leitura.c_str());
		token = strtok(str, " ");
		string tok(token);
		tokens->push_back(tok);
		token = strtok(NULL, " ");
		string cla(token);
		classes->push_back(cla);
		tok = strtok(NULL, " ");
		string lin(tok);
		linhas->push_back(lin);
		getline(*file, leitura);
	}

	/* Adição de um elemento final ao array para caso de segurança (não é bom correr o risco de chegar no fim do array e tomar uma falha de segmentação */
	tokens->push_back("1");
	classes->push_back("Neutro");
	linhas->push_back("0");

	file->close();
}

SyntacticAnalyzer::~SyntacticAnalyzer(){
	delete tokens;
	delete classes;
	delete linhas;
}

/* Funcoes de Leitura */
string SyntacticAnalyzer::getToken(int index){
	return tokens->at(index);
}

string SyntacticAnalyzer::getClass(int index){
	return classes->at(index);
}

string SyntacticAnalyzer::getLinha(int index){
	return linhas->at(index);
}

/* Funcoes de Sintaxe */
void SyntacticAnalyzer::analyze(){
	int index = 0;
	index = programa(index);
	
	string classeLida, linhaLida;
	classeLida = getClass(index);
	linhaLida = getLinha(index);
	if (classeLida.compare("Neutro")){
		/* Significa que tem código após o ponto "." */
		cout << "ERRO: " << linhaLida << " Não deve haver código após o delimitador ponto '.' para final de programa. Pode apenas haver comentários" << endl;
		exit(1);
	}
}

int SyntacticAnalyzer::programa(int index){
	
	string tokenLido, classeLida, linhaLida, linhaBuffer;
	
	tokenLido = getToken(index);
	linhaLida = getLinha(index);
	index++;
	if(tokenLido.compare("program")){
		cout << "ERRO: " << linhaLida << "    O programa precisa iniciar com a palavra-chave program " << endl;
		exit(1);
	}
	
	tokenLido  = getToken(index);
	classeLida = getClass(index); 
	index++;
	if(classeLida.compare("Identificador")){
		cout << "ERRO: "<< linhaLida << "     Depois da palavra-chave program, deve haver um identificador" << endl;
		exit(1);
	}

	tokenLido = getToken(index);
	index++;
	if(tokenLido.compare(";")){
		cout << "ERRO: " << linhaLida << "    Faltando o ;" << endl;
		exit(1);
	}

	index = declaracoes_variaveis(index);
	index = declaracoes_de_subprogramas(index);
	index = comando_composto(index);
	
	tokenLido = getToken(index);
	index++;
	if (tokenLido.compare(".")){
		cout << "ERRO: " << linhaLida << "    Faltando o ." << endl;
		exit(1);
	}
	
	return index;
	
}

int SyntacticAnalyzer::declaracoes_variaveis(int index){
	
	string tokenLido, classeLida, linhaLida, linhaBuffer;

	
	tokenLido = getToken(index);
	if(!tokenLido.compare("var")){
		index++;
		index = lista_declaracoes_variaveis(index);
		return index;
	}
	return index;
}

int SyntacticAnalyzer::lista_declaracoes_variaveis(int index){
	
	string tokenLido, classeLida, linhaLida;
	index = lista_de_identificadores(index);

	linhaLida   = getLinha(index);
	tokenLido   = getToken(index); 
	index++;
	if(tokenLido.compare(":")){
		cout <<"ERRO: " << linhaLida << "    Necessario ':' após lista de declaracoes de variaveis" << endl;
		exit(1);
	}

	/*linhaLida   = getLinha(index);
	tokenLido   = getToken(index);
	classeLida  = getClass(index);
	index++;

	if(classeLida.compare("Inteiro") && classeLida.compare("Real") && classeLida.compare("Booleano")){
		cout <<"ERRO: " << linhaLida << "   Necessario tipo após lista de variaveis" << endl;
		exit(1);
	}*/

	index = tipo(index);

	linhaLida   = getLinha(index);
	tokenLido   = getToken(index);
	classeLida  = getClass(index);
	index++;
	if(tokenLido.compare(";")){
		cout <<"ERRO: " << linhaLida << "     Esperado ';' " << endl;
		exit(1);
	}

	index = lista_declaracoes_variaveis_auxiliar(index);
	return index;


}

int SyntacticAnalyzer::lista_declaracoes_variaveis_auxiliar(int index){
	string tokenLido, classeLida, linhaLida;



	linhaLida   = getLinha(index);
	tokenLido   = getToken(index);
	classeLida  = getClass(index);
	//index++;
	
	if(!classeLida.compare("Identificador")){
		index = lista_de_identificadores(index);
		tokenLido = getToken(index);
		index++;
		if(!tokenLido.compare(":")){
			index = tipo(index);
			tokenLido = getToken(index);
			index++;
			if(!tokenLido.compare(";")){
				index = lista_declaracoes_variaveis_auxiliar(index);
				return index;
			}else {
				cout <<"ERRO: " << linhaLida << "     Esperado ';' " << endl;
				exit(1);
			}
		}else{
			cout <<"ERRO: " << linhaLida << "     Esperado ':' " << endl;
			exit(1);
		}
	}

	return index;
}

int SyntacticAnalyzer::lista_de_identificadores(int index){
	string tokenLido, classeLida, linhaLida;
	
	linhaLida   = getLinha(index);
	tokenLido   = getToken(index);
	classeLida  = getClass(index);
	index++;

	if(classeLida.compare("Identificador")){
		cout << "ERRO: "<< linhaLida << "     Esperado um identificador" << endl;
		exit(1);
	}

	index = lista_de_identificadores_auxiliar(index);
	return index;
}

int SyntacticAnalyzer::lista_de_identificadores_auxiliar(int index){
	string tokenLido, classeLida, linhaLida;
	
	linhaLida   = getLinha(index);
	tokenLido   = getToken(index);
	classeLida  = getClass(index);
	
	if(!tokenLido.compare(",")){
		index++;
		linhaLida   = getLinha(index);
		tokenLido   = getToken(index);
		classeLida  = getClass(index);
		index++;
		if(!classeLida.compare("Identificador")){
			index = lista_de_identificadores_auxiliar(index);
			return index;
		}else{
			cout << "ERRO: "<< linhaLida << "     Esperado um identificador" << endl;
			exit(1);
		}
	}
	return index;
}

int SyntacticAnalyzer::tipo(int index){
	string tokenLido, classeLida, linhaLida;

	linhaLida   = getLinha(index);
	tokenLido   = getToken(index);
	classeLida  = getClass(index);
	index++;

	if(tokenLido.compare("integer") && tokenLido.compare("real") && tokenLido.compare("boolean")){
		cout <<"ERRO: " << linhaLida << "   Type expected" << endl;
		exit(1);
	}

	return index;

}

int SyntacticAnalyzer::declaracoes_de_subprogramas(int index){
	string tokenLido, classeLida, linhaLida;
	index = declaracoes_de_subprogramas_auxiliar(index);
	return index;
}

int SyntacticAnalyzer::declaracoes_de_subprogramas_auxiliar(int index){
	string tokenLido, classeLida, linhaLida;
	
	//Tenho que verificar se comeca com procedure
	tokenLido  = getToken(index);
	if(!tokenLido.compare("procedure")){
		index = declaracao_de_subprograma(index);
		tokenLido  = getToken(index);
		classeLida = getClass(index); 
		index++;

		if(tokenLido.compare(";")){
			cout <<"ERRO: " << linhaLida << "     Esperado ';' " << endl;
			exit(1);
		}
		index = declaracoes_de_subprogramas_auxiliar(index);

	}
	return index;
	
}

int SyntacticAnalyzer::declaracao_de_subprograma(int index){
	string tokenLido, classeLida, linhaLida;
	linhaLida   = getLinha(index);
	tokenLido   = getToken(index);
	classeLida  = getClass(index);
	index++;
	if(tokenLido.compare("procedure")){
		cout <<"ERRO: " << linhaLida << "     Esperado 'procedure' " << endl;
		exit(1);
	}

	linhaLida   = getLinha(index);
	tokenLido   = getToken(index);
	classeLida  = getClass(index);
	index++;

	if(classeLida.compare("Identificador")){
		cout <<"ERRO: " << linhaLida << "     Esperado identificador " << endl;
		exit(1);
	}

	index = argumentos(index);
	tokenLido = getToken(index);
	index++;
	if(tokenLido.compare(";")){
		cout <<"ERRO: " << linhaLida << "     Esperado ';' " << endl;
		exit(1);
	}

	index = declaracoes_variaveis(index);
	index = declaracoes_de_subprogramas(index);
	index = comando_composto(index);
	return index;

}

int SyntacticAnalyzer::argumentos(int index){
	string tokenLido, classeLida, linhaLida;
	linhaLida   = getLinha(index);
	tokenLido   = getToken(index);
	classeLida  = getClass(index);
	index++;
	
	if(!tokenLido.compare("(")){
		index = lista_de_parametros(index);
		linhaLida   = getLinha(index);
		tokenLido   = getToken(index);
		classeLida  = getClass(index);
		index++;

		if(tokenLido.compare(")")){
			cout <<"ERRO: " << linhaLida << "     Esperado ')' " << endl;
			exit(1);
		}
		return index;
	}else return index;

}

int SyntacticAnalyzer::lista_de_parametros(int index){
	string tokenLido, classeLida, linhaLida;
	index = lista_de_identificadores(index);
	tokenLido = getToken(index);
	index++;
	if(tokenLido.compare(":")){
		cout <<"ERRO: " << linhaLida << "     Esperado ':' " << endl;
		exit(1);
	}
	index = tipo(index);
	index = lista_de_parametros_auxiliar(index);
	return index;
}

int SyntacticAnalyzer::lista_de_parametros_auxiliar(int index){
	string tokenLido, classeLida, linhaLida;
	linhaLida   = getLinha(index);
	tokenLido   = getToken(index);
	classeLida  = getClass(index);
	
	if(!tokenLido.compare(";")){
		index++;
		index = lista_de_identificadores(index);
		
		tokenLido = getToken(index);
		index++;
		if(tokenLido.compare(":")){
			cout <<"ERRO: " << linhaLida << "     Esperado ':' " << endl;
			exit(1);
		}

		index = tipo(index);
		index = lista_de_parametros_auxiliar(index);
		return index;

	}else{
		return index;
	}
}

int SyntacticAnalyzer::comando_composto(int index){
	string tokenLido, classeLida, linhaLida;
	linhaLida   = getLinha(index);
	tokenLido   = getToken(index);
	classeLida  = getClass(index);
	index++;
	if(tokenLido.compare("begin")){
		cout <<"ERRO: " << linhaLida << "     Esperado 'begin' " << endl;
		exit(1);
	}

	index = comandos_opcionais(index);
	tokenLido = getToken(index);
	index++;

	if(tokenLido.compare("end")){
		cout <<"ERRO: " << linhaLida << "     Esperado 'end' " << endl;
		exit(1);
	}
	return index;
}

int SyntacticAnalyzer::comandos_opcionais(int index){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);

	if (classeLida.compare("Identificador") && tokenLido.compare("begin") && tokenLido.compare("if") && tokenLido.compare("while")){
		/* Significa que é vazio */
        return index;
	}

	index = lista_de_comandos(index);
	return index;
}

int SyntacticAnalyzer::lista_de_comandos(int index){
	index = comando(index);
	index = lista_de_comandos_auxiliar(index);
	return index;
}
int SyntacticAnalyzer::lista_de_comandos_auxiliar(int index){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);

	if (tokenLido.compare(";")){
		/* Significa que é vazio */
		return index;
	}

	index++;
	index = comando(index);
	index = lista_de_comandos_auxiliar(index);
	return index;
}

int SyntacticAnalyzer::comando(int index){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);

	if (!classeLida.compare("Identificador")){
		/* Significa que é um identificador */
		index++;

		tokenLido = getToken(index);
		classeLida = getClass(index);
		linhaLida = getLinha(index);

		if (tokenLido.compare(":=")){
			/* Significa que é ativação de procedimento */
			index = ativacao_de_procedimentos(index);
			return index;
		}

		index++;
		index = expressao(index);
		return index;
	}

	if (!tokenLido.compare("begin")){
		/* Significa que é comando composto */
		index = comando_composto(index);
		return index;
	}

	if (!tokenLido.compare("if")){
		index++;

		index = expressao(index);

		tokenLido = getToken(index);
		classeLida = getClass(index);
		linhaLida = getLinha(index);

		if (tokenLido.compare("then")){
			/* Significa que está faltando then */
			cout << "ERRO: " << linhaLida << " Comando if exige then após expressão" << endl;
			exit(1);
		}

		index++;
		index = comando(index);
		index = parte_else(index);
		return index;
	}

	if (!tokenLido.compare("while")){
		index++;

		index = expressao(index);

		tokenLido = getToken(index);
		classeLida = getClass(index);
		linhaLida = getLinha(index);

		if (tokenLido.compare("do")){
			/* Significa que está faltando do */
			cout << "ERRO: " << linhaLida << " Comando while exige do após expressão" << endl;
			exit(1);
		}

		index++;
		index = comando(index);
		return index;
	}

	/* Significa que não é um comando */
	cout << "ERRO: " << linhaLida << " Esperado um comando" << endl;
	exit(1);
}

int SyntacticAnalyzer::parte_else(int index){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);

	if (tokenLido.compare("else")){
		/* Significa que é vazio */
		return index;
	}

	index++;
	index = comando(index);
	return index;
}

int SyntacticAnalyzer::variavel(int index){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);
	index++;

	if (classeLida.compare("Identificador")){
		/* Significa que NÃO é um identificador */
		cout << "ERRO: " << linhaLida << " Esperado um identificador" << endl;
		exit(1);
	}

	return index;
}

int SyntacticAnalyzer::ativacao_de_procedimentos(int index){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);
	index++;

	if (classeLida.compare("Identificador")){
		/* Significa que NÃO começou com identificador */
		cout << "ERRO: " << linhaLida << " Esperado um identificador" << endl;
		exit(1);
	}

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);

	if (tokenLido.compare("(")){
		/* Significa que é um identificador isolado */;
		return index;
	}

	index++;
	index = lista_de_expressoes(index);

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);
	index++;

	if (tokenLido.compare(")")){
		/* Significa que NÃO foi fechado */
		cout << "ERRO: " << linhaLida << " Parêntese aberto e não fechado" << endl;
		exit(1);
	}

	return index;
}

int SyntacticAnalyzer::lista_de_expressoes(int index){
	index = expressao(index);
	index = lista_declaracoes_variaveis_auxiliar(index);
	return index;
}

int SyntacticAnalyzer::lista_de_expressoes_auxiliar(int index){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);

	if (tokenLido.compare(",")){
		/* Significa que é vazio */
		return index;
	}

	index++;
	index = expressao(index);
	index = lista_de_expressoes_auxiliar(index);
	return index;
}

int SyntacticAnalyzer::expressao(int index){
	string tokenLido, classeLida, linhaLida;

	index = expressao_simples(index);

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);

	if (classeLida.compare("Operador_Relacional")) {
		/* Significa que é uma expressão simples isolada */
		return index;
	}

	index = op_relacional(index);
	index = expressao_simples(index);

	return index;
}

int SyntacticAnalyzer::expressao_simples(int index){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);

	if (!tokenLido.compare("+") && !tokenLido.compare("-")){
		/* Significa que começa com sinal */
		index = sinal(index);
	}

	index = termo(index);
	index = expressao_simples_auxiliar(index);
	return index;


}

int SyntacticAnalyzer::expressao_simples_auxiliar(int index){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);

	if (tokenLido.compare("+") && tokenLido.compare("-") && tokenLido.compare("or")) {
		/* Significa que é vazio */
		return index;
	}

	index = op_aditivo(index);
	index = termo(index);
	index = expressao_simples_auxiliar(index);

	return index;
}

int SyntacticAnalyzer::termo(int index){
	index = fator(index);
	index = termo_auxiliar(index);
	return index;
}

int SyntacticAnalyzer::termo_auxiliar(int index){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);

	if (tokenLido.compare("*") && tokenLido.compare("/") && tokenLido.compare("and")) {
		/* Significa que é vazio */
		return index;
	}

	/* Significa que não é vazio. Não precisa incrementar já que op_multiplicativo lerá essa mesma linha */
	index = op_multiplicativo(index);
	index = fator(index);
	index = termo_auxiliar(index);
	return index;
}

int SyntacticAnalyzer::fator(int index){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);
	index++;

	if (!classeLida.compare("Identificador")){
		/* Significa que inicia com Identificador */

		tokenLido = getToken(index);
		classeLida = getClass(index);
		linhaLida = getLinha(index);

		if (!tokenLido.compare("(")){
			/* Significa que o parêntese foi aberto */
			index++;

			index = lista_de_expressoes(index);

			tokenLido = getToken(index);
			classeLida = getClass(index);
			linhaLida = getLinha(index);
			index++;
			if (tokenLido.compare(")")){
				/* Significa que NÃO fechou corretamente */
				cout << "ERRO: " << linhaLida << " Parêntese aberto e não fechado" << endl;
				exit(1);
			}

			return index;

		}
		else {
			/* Significa que é um identificador isolado */
			return index;
		}
	}

	if (!classeLida.compare("Inteiro")){
		return index;
	}

	if (!classeLida.compare("Real")){
		return index;
	}

	if (!tokenLido.compare("true")){
		return index;
	}

	if (!tokenLido.compare("false")){
		return index;
	}

	if (!tokenLido.compare("(")){
		index = expressao(index);

		tokenLido = getToken(index);
		classeLida = getClass(index);
		linhaLida = getLinha(index);
		index++;

		if (tokenLido.compare(")")){
			/* Significa que NÃO fechou corretamente */
			cout << "ERRO: " << linhaLida << " Parêntese aberto e não fechado" << endl;
			exit(1);
		}

		return index;
	}

	cout << "ERRO: " << linhaLida << " Esperado um fator" << endl;
	exit(1);
}

int SyntacticAnalyzer::sinal(int index){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);
	index++;

	if (tokenLido.compare("+") && tokenLido.compare("-")) {
		/* Significa que não é sinal */
		cout << "ERRO: " << linhaLida << " Esperado um sinal + -" << endl;
		exit(1);
	}

	return index;
}

int SyntacticAnalyzer::op_relacional(int index){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);
	index++;

	if (tokenLido.compare("=") && tokenLido.compare("<") && tokenLido.compare(">")
		&& tokenLido.compare("<=") && tokenLido.compare(">=") && tokenLido.compare("<>")) {
		/* Significa que não é relacional */
		cout << "ERRO: " << linhaLida << " Esperado um operador relacional = < > <= >= <>" << endl;
		exit(1);
	}

	return index;
}

int SyntacticAnalyzer::op_aditivo(int index){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);
	index++;

	if (tokenLido.compare("+") && tokenLido.compare("-") && tokenLido.compare("or")) {
		/* Significa que não é aditivo */
		cout << "ERRO: " << linhaLida << " Esperado um operador aditivo + - or" << endl;
		exit(1);
	}

	return index;
}

int SyntacticAnalyzer::op_multiplicativo(int index){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);
	index++;

	if (tokenLido.compare("*") && tokenLido.compare("/") && tokenLido.compare("and")) {
		/* Significa que não é multiplicativo */
		cout << "ERRO: " << linhaLida << " Esperado um operador multiplicativo + - and" << endl;
		exit(1);
	}

	return index;
}