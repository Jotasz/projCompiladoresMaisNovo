#include <cstring>
#include <fstream>
#include <string>
#include "SemanticAnalyzer.h"
#include <iostream>
using namespace std;

/* Funcoes de Classe */
SemanticAnalyzer::SemanticAnalyzer(string filename){
	ifstream *file = new ifstream(filename.c_str());
	string leitura;
	tokens  = new vector<string>();
	classes = new vector<string>();
	linhas  = new vector<string>();
	pilha   = new vector<string>();
	tipos	= new vector<string>();

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

SemanticAnalyzer::~SemanticAnalyzer(){
	delete tokens;
	delete classes;
	delete linhas;
	delete pilha;
	delete tipos;
}

/* Funções de Semântica */
string SemanticAnalyzer::checkTable(string op, string val){
	if (!val.compare("Tipo_Invalido")){
		return "Tipo_Invalido";
	}
	
	if (!op.compare("+") || !op.compare("-")){
		/* Operações de Sinal */
		if (!val.compare("Booleano")){
			return "Tipo_Invalido";
		}
		if (!val.compare("Inteiro")){
			return "Inteiro";
		}
		if (!val.compare("Real")){
			return "Real";
		}
	}
	
	if (!op.compare("not")){
		if (!val.compare("Booleano")){
			return "Booleano";
		}
		return "Tipo_Invalido";
	}

	return "Chamada_Invalida";
}

string SemanticAnalyzer::checkTable(string op, string valLeft, string valRight){
	if (!valLeft.compare("Tipo_Invalido") || !valRight.compare("Tipo_Invalido")){
		return "Tipo_Invalido";
	}
	
	if (!op.compare("+") || !op.compare("-") || !op.compare("*") || !op.compare("/")){
		/* Operações aritméticas */
		if (!valLeft.compare("Booleano") || !valRight.compare("Booleano")){
			/* Um dos dois é booleano */
			return "Tipo_Invalido";
		}
		if (!valLeft.compare("Real") || !valRight.compare("Real")){
			/* Um dos dois é real */
			return "Real";
		}
		if (!valLeft.compare("Inteiro") && !valRight.compare("Inteiro")){
			/* Ambos são inteiros */
			return "Inteiro";
		}
	}

	if (!op.compare("=") || !op.compare("<>")){
		/* Operações relacionais genéricas */
		if (!valLeft.compare("Booleano") && !valRight.compare("Booleano")){
			/* Ambos são booleanos */
			return "Booleano";
		}
		if (!valLeft.compare("Booleano") || !valRight.compare("Booleano")){
			/* Um dos dois é booleano, mas o outro não */
			return "Tipo_Invalido";
		}
		/* Nenhum é booleano */
		return "Booleano";
	}

	if (!op.compare("<") || !op.compare(">") || !op.compare("<=") || !op.compare(">=")){
		/* Operações relacionais aritméticas */
		if (!valLeft.compare("Booleano") || !valRight.compare("Booleano")){
			/* Um dos dois é booleano */
			return "Tipo_Invalido";
		}
		/* Nenhum dos dois é booleano */
		return "Booleano";
	}

	if (!op.compare("and") || !op.compare("or")){
		/* Operações lógicas */
		if (!valLeft.compare("Booleano") && !valRight.compare("Booleano")){
			/* Ambos são booleanos */
			return "Booleano";
		}
		/* Um dos dois não é booleano */
		return "Tipo_Invalido";
	}

	if (!op.compare(":=")){
		/* Operação de Atribuição */
		if (!valLeft.compare("Booleano") && !valRight.compare("Booleano")){
			/* Ambos são booleanos */
			return "Booleano";
		}
		if (!valLeft.compare("Booleano") || !valRight.compare("Booleano")){
			/* Um dos dois é booleano */
			return "Tipo_Invalido";
		}
		if (!valLeft.compare("Real")){
			/* O recebedor é real */
			return "Real";
		}
		if (!valLeft.compare("Inteiro")){
			/*O recebedor é inteiro */
			return "Inteiro";
		}
	}
	
	return "Chamada_Invalida";
}

/* Funcoes de Vetores */
string SemanticAnalyzer::getToken(int index){
	return tokens->at(index);
}

string SemanticAnalyzer::getClass(int index){
	return classes->at(index);
}

string SemanticAnalyzer::getLinha(int index){
	return linhas->at(index);
}

string SemanticAnalyzer::getTipo(string token){
	int i;
	for (i = pilha->size() - 1; i >= 0; i--){
		if (!pilha->at(i).compare(token)){
			// Se for o token procurado
			return tipos->at(i);
		}
	}
	// Se não encontrar o token
	return "";
}

bool SemanticAnalyzer::isThere(string token){
	int i;
	for (i = pilha->size() - 1; i >= 0; i--){
		if (!pilha->at(i).compare("$")) break;
		else if (!pilha->at(i).compare(token)) return true;
	}
	return false;
}

bool SemanticAnalyzer::isSomewhere(string token){
	int i;
	for (i = pilha->size() - 1; i >= 0; i--){
		if (!pilha->at(i).compare(token)) return true;
	}
	return false;
}

void SemanticAnalyzer::removeMark(){
	int i;
	for (i = pilha->size() - 1; i >= 0; i--){
		if (!pilha->at(i).compare("$"))	break;
		pilha->pop_back();
	}
	pilha->pop_back();
}

void SemanticAnalyzer::removeTipo(){
	int i;
	for (i = tipos->size() - 1; i >= 0; i--){
		if (!tipos->at(i).compare("$"))	break;
		tipos->pop_back();
	}
	tipos->pop_back();
}

/* Funcoes de Sintaxe */
void SemanticAnalyzer::analyze(){
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

int SemanticAnalyzer::programa(int index){
	
	string tokenLido, classeLida, linhaLida, linhaBuffer;
	
	pilha->push_back("$");
	tipos->push_back("$");
	
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

int SemanticAnalyzer::declaracoes_variaveis(int index){
	
	string tokenLido, classeLida, linhaLida, linhaBuffer;
	
	tokenLido = getToken(index);
	if(!tokenLido.compare("var")){
		index++;
		index = lista_declaracoes_variaveis(index);
		return index;
	}
	return index;
}

int SemanticAnalyzer::lista_declaracoes_variaveis(int index){
	
	string tokenLido, classeLida, linhaLida;
	index = lista_de_identificadores(index);

	linhaLida   = getLinha(index);
	tokenLido   = getToken(index); 
	index++;
	if(tokenLido.compare(":")){
		cout <<"ERRO: " << linhaLida << "    Necessario ':' após lista de declaracoes de variaveis" << endl;
		exit(1);
	}

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

int SemanticAnalyzer::lista_declaracoes_variaveis_auxiliar(int index){
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

int SemanticAnalyzer::lista_de_identificadores(int index){
	string tokenLido, classeLida, linhaLida;
	
	linhaLida   = getLinha(index);
	tokenLido   = getToken(index);
	classeLida  = getClass(index);
	index++;
	
	if(classeLida.compare("Identificador")){
		cout << "ERRO: "<< linhaLida << "     Esperado um identificador" << endl;
		exit(1);
	}

	if(isThere(tokenLido)){
		cout << "ERRO: "<< linhaLida << "!    Redefinicao do identificador " << tokenLido << "!" << endl;
		exit(2);
	}

	pilha->push_back(tokenLido);

	index = lista_de_identificadores_auxiliar(index);
	return index;
}

int SemanticAnalyzer::lista_de_identificadores_auxiliar(int index){
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
			
			if(isThere(tokenLido)){
				cout << "ERRO: "<< linhaLida << "!  Redefinicao do identificador " << tokenLido << "!" << endl;
				exit(2);	
			}
			pilha->push_back(tokenLido);
			index = lista_de_identificadores_auxiliar(index);
			return index;
		}else{
			cout << "ERRO: "<< linhaLida << "     Esperado um identificador" << endl;
			exit(1);
		}
	}
	return index;
}

int SemanticAnalyzer::tipo(int index){
	string tokenLido, classeLida, linhaLida;

	linhaLida   = getLinha(index);
	tokenLido   = getToken(index);
	classeLida  = getClass(index);
	index++;

	if(tokenLido.compare("integer") && tokenLido.compare("real") && tokenLido.compare("boolean")){
		cout <<"ERRO: " << linhaLida << "   Type expected" << endl;
		exit(1);
	}

	while (tipos->size() != pilha->size()){
		if (!tokenLido.compare("integer")){
			tipos->push_back("Inteiro");
		}
		else if (!tokenLido.compare("real")){
			tipos->push_back("Real");
		}
		else if (!tokenLido.compare("boolean")){
			tipos->push_back("Booleano");
		}
		else {
			tipos->push_back("ERRO em alguam coisa que eu fiz!");
		}
	}

	return index;

}

int SemanticAnalyzer::declaracoes_de_subprogramas(int index){
	string tokenLido, classeLida, linhaLida;
	index = declaracoes_de_subprogramas_auxiliar(index);
	return index;
}

int SemanticAnalyzer::declaracoes_de_subprogramas_auxiliar(int index){
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

int SemanticAnalyzer::declaracao_de_subprograma(int index){
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

	
	if(isThere(tokenLido)){
		cout << "ERRO: "<< linhaLida << "    Redefinicao do identificador " << tokenLido << "!" << endl;
		exit(2);	
	}
	
	pilha->push_back(tokenLido);
	pilha->push_back("$");
	
	tipos->push_back("Tipo_Invalido");
	tipos->push_back("$");

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

int SemanticAnalyzer::argumentos(int index){
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

int SemanticAnalyzer::lista_de_parametros(int index){
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

int SemanticAnalyzer::lista_de_parametros_auxiliar(int index){
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

int SemanticAnalyzer::comando_composto(int index){
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

	removeMark();
	return index;
}

int SemanticAnalyzer::comandos_opcionais(int index){
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

int SemanticAnalyzer::lista_de_comandos(int index){
	index = comando(index);
	index = lista_de_comandos_auxiliar(index);
	return index;
}
int SemanticAnalyzer::lista_de_comandos_auxiliar(int index){
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

int SemanticAnalyzer::comando(int index){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);
	vector<string> *pct = new vector<string>();

	if (!classeLida.compare("Identificador")){
		if(!isSomewhere(tokenLido)){
			cout << "Identificador " << tokenLido << " não encontrado nesse escopo!" << endl;
			exit(2);
		}
		/* Significa que é um identificador */
		pct->push_back(getTipo(tokenLido));
		index++;
		
		tokenLido = getToken(index);
		classeLida = getClass(index);
		linhaLida = getLinha(index);

		if (tokenLido.compare(":=")){
			/* Significa que é ativação de procedimento */
			index = ativacao_de_procedimentos(index,pct);
			return index;
		}
		pct->push_back(":=");
		index++;
		index = expressao(index, pct);

		string vd = pct->back();
		pct->pop_back();
		string operador = pct->back();
		pct->pop_back();
		string ve = pct->back();
		pct->pop_back();
		pct->push_back(checkTable(operador, ve, vd));

		/*Verificar se tem só um tipo na pct*/
		if(pct->size() > 1){
			cout <<"ERRO: " << linhaLida << "    Operacao mal estruturada " << endl;
		}else if (!pct->back().compare("Tipo_Invalido")){
			cout <<"ERRO: " << linhaLida << "    Tipos incompativeis " << endl;
		}else if (!pct->back().compare("Chamada_Invalida")){
			cout <<"ERRO: " << linhaLida << "    Checktable errado " << endl;
		}

		delete pct;

		return index;
	}

	if (!tokenLido.compare("begin")){
		/* Significa que é comando composto */
		index = comando_composto(index);
		return index;
	}

	if (!tokenLido.compare("if")){
		index++;

		index = expressao(index,pct);

		if(pct->size() > 1){
			cout <<"ERRO: " << linhaLida << "    Operacao mal estruturada " << endl;
		}else if (!pct->back().compare("Tipo_Invalido")){
			cout <<"ERRO: " << linhaLida << "    Tipos incompativeis " << endl;
		}else if (!pct->back().compare("Chamada_Invalida")){
			cout <<"ERRO: " << linhaLida << "    Checktable errado " << endl;
		}else if(pct->back().compare("Booleano")){
			cout <<"ERRO: " << linhaLida << "    Esperada uma expressao booleana como condicao do if " << endl;
		}

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

		delete pct;
		return index;
	}

	if (!tokenLido.compare("while")){
		index++;

		index = expressao(index,pct);

		if(pct->size() > 1){
			cout <<"ERRO: " << linhaLida << "    Operacao mal estruturada " << endl;
		}else if (!pct->back().compare("Tipo_Invalido")){
			cout <<"ERRO: " << linhaLida << "    Tipos incompativeis " << endl;
		}else if (!pct->back().compare("Chamada_Invalida")){
			cout <<"ERRO: " << linhaLida << "    Checktable errado " << endl;
		}else if(pct->back().compare("Booleano")){
			cout <<"ERRO: " << linhaLida << "    Esperada uma expressao booleana como condicao do while " << endl;
		}

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

		delete pct;
		return index;
	}

	/* Significa que não é um comando */
	cout << "ERRO: " << linhaLida << " Esperado um comando" << endl;
	exit(1);
}

int SemanticAnalyzer::parte_else(int index){
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

int SemanticAnalyzer::variavel(int index,vector<string> *pct){
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

	if(!isSomewhere(tokenLido)){
		cout << "Identificador " << tokenLido << " não encontrado nesse escopo!" << endl;
	}

	return index;
}

int SemanticAnalyzer::ativacao_de_procedimentos(int index,vector<string> *pct){
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

	if(!isSomewhere(tokenLido)){
		cout << "Identificador " << tokenLido << " não encontrado nesse escopo!" << endl;
	}

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);

	if (tokenLido.compare("(")){
		/* Significa que é um identificador isolado */;
		return index;
	}

	index++;
	index = lista_de_expressoes(index,pct);

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

int SemanticAnalyzer::lista_de_expressoes(int index,vector<string> *pct){
	index = expressao(index,pct);
	index = lista_declaracoes_variaveis_auxiliar(index);
	return index;
}

int SemanticAnalyzer::lista_de_expressoes_auxiliar(int index,vector<string> *pct){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);

	if (tokenLido.compare(",")){
		/* Significa que é vazio */
		return index;
	}

	index++;
	index = expressao(index,pct);
	index = lista_de_expressoes_auxiliar(index,pct);
	return index;
}

int SemanticAnalyzer::expressao(int index, vector<string> *pct){
	string tokenLido, classeLida, linhaLida;

	index = expressao_simples(index,pct);

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);

	if (classeLida.compare("Operador_Relacional")) {
		/* Significa que é uma expressão simples isolada */
		return index;
	}

	index = op_relacional(index,pct);
	index = expressao_simples(index,pct);

	string vd = pct->back();
	pct->pop_back();
	string operador = pct->back();
	pct->pop_back();
	string ve = pct->back();
	pct->pop_back();
	pct->push_back(checkTable(operador, ve, vd));

	return index;
}

int SemanticAnalyzer::expressao_simples(int index, vector<string> *pct){
	string tokenLido, classeLida, linhaLida;
	bool opSinal = false;
	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);

	if (!tokenLido.compare("+") || !tokenLido.compare("-")){
		/* Significa que começa com sinal */
		index = sinal(index,pct);
		opSinal = true;
	}

	index = termo(index, pct);
	index = expressao_simples_auxiliar(index, pct);

	/*string vd = pct->back();
	pct->pop_back();
	string operador = pct->back();
	pct->pop_back();
	string ve = pct->back();
	pct->pop_back();
	pct->push_back(checkTable(operador, ve, vd));*/

	if (opSinal){
		string vd = pct->back();
		pct->pop_back();
		string operador = pct->back();
		pct->pop_back();
		pct->push_back(checkTable(operador,vd));
	}

	return index;


}

int SemanticAnalyzer::expressao_simples_auxiliar(int index,vector<string> *pct){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);

	if (tokenLido.compare("+") && tokenLido.compare("-") && tokenLido.compare("or")) {
		/* Significa que é vazio */
		return index;
	}

	index = op_aditivo(index,pct);
	index = termo(index,pct);
	index = expressao_simples_auxiliar(index,pct);

	string vd = pct->back();
	pct->pop_back();
	string operador = pct->back();
	pct->pop_back();
	string ve = pct->back();
	pct->pop_back();
	pct->push_back(checkTable(operador, ve, vd));

	return index;
}

int SemanticAnalyzer::termo(int index,vector<string> *pct){
	index = fator(index,pct);
	index = termo_auxiliar(index,pct);

	/*string vd = pct->back();
	pct->pop_back();
	string operador = pct->back();
	pct->pop_back();
	string ve = pct->back();
	pct->pop_back();
	pct->push_back(checkTable(operador, ve, vd));*/

	return index;
}

int SemanticAnalyzer::termo_auxiliar(int index,vector<string> *pct){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);

	if (tokenLido.compare("*") && tokenLido.compare("/") && tokenLido.compare("and")) {
		/* Significa que é vazio */
		return index;
	}

	/* Significa que não é vazio. Não precisa incrementar já que op_multiplicativo lerá essa mesma linha */
	index = op_multiplicativo(index,pct);
	index = fator(index,pct);
	index = termo_auxiliar(index,pct);

	string vd = pct->back();
	pct->pop_back();
	string operador = pct->back();
	pct->pop_back();
	string ve = pct->back();
	pct->pop_back();
	pct->push_back(checkTable(operador, ve, vd));

	return index;
}

int SemanticAnalyzer::fator(int index,vector<string> *pct){
	string tokenLido, classeLida, linhaLida;

	tokenLido = getToken(index);
	classeLida = getClass(index);
	linhaLida = getLinha(index);
	index++;

	if (!classeLida.compare("Identificador")){
		/* Significa que inicia com Identificador */

		if(!isSomewhere(tokenLido)){
			cout << "Identificador " << tokenLido << " não encontrado nesse escopo!" << endl;
		}
		
		pct->push_back(getTipo(tokenLido));
		tokenLido = getToken(index);
		classeLida = getClass(index);
		linhaLida = getLinha(index);
		

		if (!tokenLido.compare("(")){
			/* Significa que o parêntese foi aberto */
			index++;

			index = lista_de_expressoes(index,pct);

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
		pct->push_back(classeLida);
		return index;
	}

	if (!classeLida.compare("Real")){
		pct->push_back(classeLida);
		return index;
	}

	if (!tokenLido.compare("true")){
		pct->push_back(classeLida);
		return index;
	}

	if (!tokenLido.compare("false")){
		pct->push_back(classeLida);
		return index;
	}

	if (!tokenLido.compare("not")){
		pct->push_back(tokenLido);
		index = fator(index,pct);
		string vd = pct->back();
		pct->pop_back();
		string operador = pct->back();
		pct->pop_back();
		pct->push_back(checkTable(operador, vd));

		return index;
	}

	if (!tokenLido.compare("(")){
		index = expressao(index,pct);

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

int SemanticAnalyzer::sinal(int index,vector<string> *pct){
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

	pct->push_back(tokenLido);

	return index;
}

int SemanticAnalyzer::op_relacional(int index,vector<string> *pct){
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

	pct->push_back(tokenLido);

	return index;
}

int SemanticAnalyzer::op_aditivo(int index,vector<string> *pct){
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

	pct->push_back(tokenLido);
	return index;
}

int SemanticAnalyzer::op_multiplicativo(int index,vector<string> *pct){
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

	pct->push_back(tokenLido);

	return index;
}