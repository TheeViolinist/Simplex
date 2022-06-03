#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <algorithm> // min_element
#include <iterator> // iterator
#include <cfloat> // DLB_MAX
#include <iomanip>
#include <string.h>
#include <math.h>

using namespace std;





// Função responsável por receber uma string e verificar o tipo de desigualdade
//
//
// Retorno:
// 		0-> caso seja uma igualdade
// 		1-> caso seja de menor igual
// 		2-> caso seja de maior igual
int tipoDesigualdade(const string& desigualdade)
{
	if(desigualdade == "=")
	{
		return 0;
	}
	else if(desigualdade == "<=")
	{
		return 1;
	}
	return 2;
}

int achouDesigualdade(const char desigualdade)
{
	if(desigualdade == '=')
	{
		return  1;
	}
	else if(desigualdade == '<')
	{
		return 2;
	}
	
	else if(desigualdade == '>')
	{
		return 3;
	}
	return 0;

}



/***
 * Função responsavel por pegar os valores de uma linha e o converter double
 */
void transformaLinhaEmVetor(vector < double > &valores, string &linha)
{

	int quantidade = 0;
	char *buffer;
	double valorNumerico; // variavel que irá armazenar o valor tratado
	string temporaria; // string temporaria para armazenar o restante do buffer;

	while(true)
	{
		valorNumerico = strtod(linha.c_str(), &buffer); // Pega o primeiro valor até achar um espaço 
		valores.push_back(valorNumerico); // Adiciona em valor Numerico
		
		
		// Caso o tamanho do buffer seja igual a zero, é porque ja foi pego o ultimo valor
		// Linha == 1, pois ao colocar a restrição o tamanho de linha é somente o espaço, ent devemos parar o codigo
		if(strlen(buffer) == 0 || linha.size() == 1)
		{
			break;
		}

		// Para melhorar a restrição, percorremos a linha e tentamos achar alguma palavra, caso tenha ela, continua o algoritmo, caso não da break
		for(int i = 0; i < linha.size(); i++)
		{
			if(linha[i] != ' ' && linha[i] != '\0' && linha[i] != '\n')
			{
				quantidade++;
			}
		}

		if(quantidade == 0)
		{
			break;
		}
		// Caso continue, vamos transformar buffer em uma string para repetir os processo
		for(int i = 0; i < strlen(buffer); i++)
		{
			temporaria.push_back(buffer[i]);
		}
		linha = temporaria; // reseta o novo valor de linha
		
		temporaria.clear(); // limpa a string temporaria

	}
	// Retira o \0
	
	if(valores[valores.size() - 1] == 0)
	{
		valores.pop_back();
	}

}





/* receberInformacoes():
 *
 * Parametros:
 * 		coeficientesDaFuncao: coeficientes da função objetivo
 * 		std::vector coeficientesRestricoes : Coeficies do vetor A
 * 		std::vector restricoes : vetor que contém as relações das restrições de cada linha
 * 		std::vector coeficieteB : vetor que contém os valores das restrições para cada linha
 * 		std::vector restricoesVariaveis: vetor que possui as relações de restrição para cada variável
 * Retorno:
 * 		Nenhum
 *
 */
void recebeInformacoes(bool &ehMinimizacao, vector <double>& coeficientesDaFuncao, vector< vector < double> >& coeficientesRestricoes, vector <int>& restricoes, vector < double >& coeficienteB, vector <int> &restricoesVariaveis)
{
	
	cout << "Digite 1 para uma funcao de minimizacao e 0 para uma funcao maximizacao: ";
	cin >> ehMinimizacao;
	cin.ignore();


	cout << "Digite os valores dos coeficientes da funcao objetivo\n";
	
	string coeficientesLinha;
	getline(cin, coeficientesLinha);
	transformaLinhaEmVetor(coeficientesDaFuncao, coeficientesLinha);
	
	// Caso nao seja de minimização, transforma-se o problema em minimização
	if(!ehMinimizacao)
	{
		for(int i = 0; i < coeficientesDaFuncao.size(); i++)
		{
			coeficientesDaFuncao[i] *= - 1;
		}
	}

	cout << "Digite os valores dos coeficientes das restricoes, digite a linha toda antes de apertar enter e digite 0 para terminar a insercao\n";
	

	string linha; // linha que será tratada e armazenar os valores dos vetores
	
	string desigualdade; // String que contém a desigualdade da linha
	string coeficientesA; // string que possui os valores dos coeficientesA
	vector <double> coeficientesTemporariosRestricao;// Vetor que possui temporariamente os valores da linha de A
	double valorB; // Valor de subject_to
	string coeficienteValorB; // String que armazena temporariamente o valor de B para conversão
	char *buffer; // Armazena o buffer de conversão das string
	
	// Algoritmo para armazenar os coeficientes das restrições	
	while(true)
	{
		// Armazena a linha, caso ele digite somente 0, devemos terminar de pegar as informações 
		getline(cin, linha);
		if(linha == "0")
			break;
		

		bool achou = false; // Variavel que indica se achou a desigualdade
		int posicaoDesigualdade; // Armazena a posição inicial da desigualdade
		

		// Percorre a determinada linha até achar sua desigualdade e armazena sua posição, além de colocar toda a desigualdade em uma string
		for(int i = 0; i < linha.size(); i++)
		{
			// Verifica se acha ou não o incio da desigualdade
			if(achouDesigualdade(linha[i]))
			{
				desigualdade.push_back(linha[i]);
				// Guarda a posicao inical dela
				if(!achou)
				{
					posicaoDesigualdade = i;
					achou = true;
				}	
			}
	
		}

		// Cria a string dos coeficientes da linha da restrição
		for(int i = 0; i  < posicaoDesigualdade; i++)
		{
			coeficientesA.push_back(linha[i]);
		}

		int inicioValorB = 0;
		// caso seja uma igualdade, colocamos a posicao inicial no proximo caractere, caso não, colocamos duas casas a frente
		inicioValorB = tipoDesigualdade(desigualdade) == 0 ? posicaoDesigualdade + 1 : posicaoDesigualdade + 2;
		
		// Cria a string correspondente ao valor de subject, que está após a desigualdade
		for(int i = inicioValorB; i  < linha.size(); i++)
		{
			coeficienteValorB.push_back(linha[i]);
		}
		
		// Transforma toda aquela linha em um valor, e armazena o resto em buffer
		valorB = strtod(coeficienteValorB.c_str(), &buffer);
		coeficienteB.push_back(valorB); // Joga o valor em valor de B
		

		
		// Transforma coeficientesTemporarios em um vetor de double com os valores da linha
		transformaLinhaEmVetor(coeficientesTemporariosRestricao, coeficientesA);
		
		


		coeficientesRestricoes.push_back(coeficientesTemporariosRestricao); // adiciona ele aos coeficientes restrições
		

		coeficientesTemporariosRestricao.clear(); // limpa o vetor de A de valores double
		coeficientesA.clear(); // limpa a string de coeficientes do vetor A

		coeficienteValorB.clear(); // limpa a string do valor de b
		
		
		restricoes.push_back(tipoDesigualdade(desigualdade));// Adiciona o tipo de desigualdade ao vetor de restrições

		desigualdade.clear(); // limpa a string desigualdade
		
			
		
	}
	

	

}



/* Recebe o vector dos coeficientes da função ojetiva, os coeficientes do vetor A e os tipos de restrições */
void transformaProblemaPadraoPL(vector <double > &coeficientesDaFuncao, vector < vector <double> > &coeficientesRestricoes, vector <int> &restricoes, vector <double> &coeficientesB, vector <int> &artificiaisIndices, vector <int > &artificiaisColunas, bool &possuiArtificial)
{
	// Algortimo para mudar o valor de b caso ele seja negativo
	for(int i = 0; i < coeficientesB.size(); i++)
	{
		if(coeficientesB[i] < 0)
		{
			coeficientesB[i] *= -1;
			// For uma restricao de >=
			if(restricoes[i] == 2)
			{
				// Torna-se restricao de <=
				restricoes[i] = 1;
			}
			// For uma restricao de <=
			else if(restricoes[i] == 1)
			{

				// Torna-se restricao de >=
				restricoes[i] = 2;
			}
			
			// Acessa cada valor daquela linha e multiplica por -1
			for(double &valor : coeficientesRestricoes[i])
			{
				valor *= -1;
			}
		}
	}
	


	int quantidadeArtificiais = 0; // Armazena a quantidade de artificiais que pode estar presente no problema 
	int quantidadeIgualdade = 0; // Quantidade de restrições de igualdade

	// Percorre os tipos de restrições e verifica a quantidade de artificiais e igualdades
	for(int i = 0; i < restricoes.size(); i++)
	{
		if(restricoes[i] != 1)
		{
			quantidadeArtificiais++;
		}
		if(restricoes[i] == 0)
		{
			quantidadeIgualdade++;
		}
	}
	
	// Atualizamos a flag indicando que há variáveis artificias
	if(quantidadeArtificiais != 0)
	{
		possuiArtificial = true;
	}

	// Primeiramente, percorre todas as linhas das restrições e adiciona as variáveis de folga e excesso
	for(int i = 0; i < coeficientesB.size(); i++)
	{
		// Primeiro adiciona no início uma quantidade relativa ao que já foi adicionado
		coeficientesRestricoes[i].insert(coeficientesRestricoes[i].end(), i, 0);
		// Caso seja uma restrição de <=, coloca-se variável de folga
		if(restricoes[i] == 1)
		{
			coeficientesRestricoes[i].push_back(1);
			coeficientesDaFuncao.push_back(0);
		}
		// Caso seja uma restrição de >=, coloca-se variável de excesso e adiciona a função objetivo
		if(restricoes[i] == 2)
		{
			coeficientesRestricoes[i].push_back(-1);
			coeficientesDaFuncao.push_back(0);
		}
		// Coloca-se no final da linha de restrição a quantidade relativa ao que ainda será colocado
		coeficientesRestricoes[i].insert(coeficientesRestricoes[i].end(), coeficientesB.size() - (i + 1), 0);
	}	
	
	// Caso possua artificial
	if(possuiArtificial)
	{
		// Vamos percorrer todas as restrições diferentes de igualdade e retirar um valor, pois as restrições de igualdade só possuem variável artificial
		for(int i = 0; i < coeficientesB.size(); i++)
		{
			if(restricoes[i] != 0)
			{
				for(int j = 0; j < quantidadeIgualdade; j++)
				{
					coeficientesRestricoes[i].pop_back();
				}
			}
		}
	
		int artificiaisAdicionadas = 0;

		// Percorre todas as linhas e se ela for diferente de <=, coloca-se uma variável artificial
		for(int i = 0; i < coeficientesB.size(); i++)
		{
			coeficientesRestricoes[i].insert(coeficientesRestricoes[i].end(), artificiaisAdicionadas, 0);
			if(restricoes[i] != 1)
			{
				artificiaisAdicionadas++;
				coeficientesRestricoes[i].push_back(1); // adiciona variavel artificial
				coeficientesDaFuncao.push_back(0); // adiciona a função objetivo
				artificiaisIndices.push_back(i); // adiciona a linha da artificial
				artificiaisColunas.push_back(coeficientesDaFuncao.size() - 1); // Adiciona a coluna dela
			}
			coeficientesRestricoes[i].insert(coeficientesRestricoes[i].end(), quantidadeArtificiais - artificiaisAdicionadas, 0);
		}
	}
	coeficientesDaFuncao.push_back(0); // valor da função objetivo
	
}


// Realiza o teste da razão mínima para achar a linha pivo
int retornaLinhaPivo(vector < double > &coeficientesB, vector < vector <double> > &coeficientesRestricoes, int indiceColunaPivo)
{

	// Inicia a primeira divisao como o menor valor
	double menorValor = coeficientesB[0] / coeficientesRestricoes[0][indiceColunaPivo];
	
	// Caso, o menor valor inicial seja menor do que zero, inicia ele como infinito positivo
	if(menorValor < 0)
	{
		menorValor = DBL_MAX;
	}	
	
	double divisao;
	int tentativas = 1;
	
	// inicializa a linha pivo com o primeiro valor
	int linhaPivo = 0;

	for(int  i = 1; i < coeficientesB.size(); i++)
	{
	
		// Mantenho fixo a coluna e vario a linha da matriz
		// Verifica-se se o valor é diferente de zero e maior que zero
		// Caso for, ignora esse elemento da linha e vai para o próximo
		if(coeficientesRestricoes[i][indiceColunaPivo] != 0 && coeficientesRestricoes[i][indiceColunaPivo] > 0)
		{

			divisao = coeficientesB[i] / coeficientesRestricoes[i][indiceColunaPivo];
			
			if(divisao < menorValor)
			{
				menorValor = divisao;
				linhaPivo = i;
			}
		}
		else
		{

			tentativas++;
			continue;
		}
		
	}
	if(tentativas == coeficientesB.size())
	{
	
		return -1;
	}
	

	return linhaPivo;
}




/* RetornaNovoVetorCoeficientesFuncao() : retorna novo vetor dos coeficientes da função
*
* Parametros:
* 		coeficientesDaFuncao: vetor dos coeficientes da funcao
* 		novaLinhaPivo: vetor da nova linha pivo
* 		valorDeBNovaLinha: valor de B na linha pivo
* 		indiceColunaPivo: valor da coluna pivo
*/
void retornaNovoVetorCoeficientesFuncao(vector < double > &coeficientesDaFuncao, vector < double > &novaLinhaPivo, double valorDeBNovaLinha, int indiceColunaPivo)
{
	// Pega-se o valor multiplicativo para reduzir aquele valor da coluna a zero
	double valorMultiplicativo = (-1) * coeficientesDaFuncao[indiceColunaPivo];
	
	// Valor que será somado à linha que queremos zerar
	double valorCorrecao;
	for(int i = 0; i < novaLinhaPivo.size(); i++)
	{
		// Multiplica pela linha pivo
		valorCorrecao = novaLinhaPivo[i] * valorMultiplicativo;
		
		// Soma-se e adiciona o novo valor
		coeficientesDaFuncao[i] += valorCorrecao;
	}
	
	// Muda o valor da função objetivo
	coeficientesDaFuncao[coeficientesDaFuncao.size() - 1] +=  (valorDeBNovaLinha* valorMultiplicativo) ;
}




/*RetornaNovaMatrizesCoeficientes: Muda os valores do vetor A fazendo soma algébricas
 *
 *
 *Parametros:
 *		coeficientesA : matriz dos coeficientes das restrições
 *		novaLinhaPivo : linha pivo modificada
 * 		valorDeBNovaLinha : valor de B na linha pivo
 *		indiceColunaPivo: indice da coluna Pivo
 *		linhaPivo: indice da linha pivo
 *		coeficientesB : valores das restrições
 * Retorno:
 * 		Nenhum
*/
void retornaNovaMatrizCoeficientesRestricoes(vector < vector < double > > &coeficientesA, vector <double> &novaLinhaPivo, double valorDeBNovaLinha, int indiceColunaPivo, int linhaPivo, vector <double> &coeficientesB)
{
	/* Basicamente, imagine que temos a linha na restrição: 
	 * 2 4 5 6
	 * E a minha nova linha seja
	 * 2 1 9 7
	 * e queremos zerar a coluna de indice 1
	 * Logo, o 4 da primeira equação deve ser zerado.
	 * Para isso, devemos acessar esse valor de 4 na linha e coluna correspondente e multiplicá-lo por -1
	 * Após isso, vamos multiplicar a linha de baixo por esse valor que é -4 e somar ao seu correspondente na outra linha
	 * a primeira operacao seria: (-4 * 2) + 2 = -6 e assim por diante
	 */

	double valorMultiplicativo;
	double valorCorrecao;
	
	// O tamanho de coeficientesB é o mesmo que a quantidade de restricoes
	for(int i = 0; i < coeficientesB.size(); i++)
	{
		if(i != linhaPivo)
		{
			// Pega-se o valor multiplicativo daquela linha, na colunaPivo que queremos zerar
			valorMultiplicativo = (-1) * coeficientesA[i][indiceColunaPivo];
			// Estamos acessando cada linha e percorrendo os valores da coluna
			for(int j = 0; j < coeficientesA[i].size(); j++)
			{
				//percorre a linha, multiplicando o valor pelo elemento da linha e somando o valorCorrecao com coeficientesA
				valorCorrecao = novaLinhaPivo[j] * valorMultiplicativo;
				coeficientesA[i][j] += valorCorrecao;

			}
			coeficientesB[i] += (valorDeBNovaLinha * valorMultiplicativo);
		}

	}
				
}





void printaTableau(vector <double> &coeficientesDaFuncao, vector < vector <double> > &coeficientesRestricoes, vector <double> &coeficientesB)
{
	 
    for(int i = 0; i < coeficientesDaFuncao.size(); i++)
    {
        cout << setw(10) << setprecision(2) << coeficientesDaFuncao[i] << " ";
    }
    cout << endl;
	int k  = 0;
    for(vector <double > &linha : coeficientesRestricoes)
    {
        for( double  &valor : linha )
        {
            cout << setw(10) <<  valor << " ";
        }
		cout <<  setw(10) << setprecision(5) << coeficientesB[k];
		k++;
        cout << endl;

    }


    cout << "------------------------------------------------------------------------------------------------" << endl;
    
	getchar();
}




/****
 * CriaVetorW: função responsável por criar o vetor W que queremos minimizar no método duas fases
 * 
 * 
 * Parâmetros:
 * 				coeficientesW: coeficientes de W
 * 				coeficientesRestricoes: matriz das restrições
 * 				artificiaisIndices: indices das variaveis artificiais
 * 				artificiaisColunas: indices das colunas das variáveis artificiais
 * 				coeficientesB : valores da função subject_to
 */
void criaVetorW(vector < double > &coeficientesW, const vector <vector <double > > &coeficientesRestricoes, vector <int> &artificiaisIndices, vector <int> &artificiaisColunas, vector <double> &coeficientesB)
{

	// Estamos acessando cada linha que está armazenado no vetor
	for(int &indiceArtificial : artificiaisIndices)
	{
		// Percorremos a linha da restricao
		for(int i = 0; i < coeficientesRestricoes[indiceArtificial].size(); i++)
		{
			//somamos as linhas que possuem variavel artificial e então soma-se os valores e coloca em w com sinal oposto	
			coeficientesW[i] += coeficientesRestricoes[indiceArtificial][i] * -1; 
			cout << coeficientesW[i] << " ";	
				
		}
		cout << endl;
		// coloca no ultimo valor de W os valores de B, o qual iremos minimizar
		coeficientesW[coeficientesW.size() - 1] += coeficientesB[indiceArtificial] * -1;
		
		
	}

	// Zera os valores de W que correspondem aos indices das colunas artificiais
	for(int indice : artificiaisColunas)
	{
		coeficientesW[indice] = 0;
	}
	

}




/***
 * RemoveArtificiais() : Remove as variaveis artificiais da função objetivo e das restrições
 * 
 * 
 * 
 */
void removeArtificiais(vector <double> &coeficientesFuncao, vector < vector <double> > &coeficientesA, vector <double> &coeficientesB, vector <int> &artificiaisColunas)
{
	
	// Fazemos um loop referente a quantidade de variaveis artificiais
	for(int i = 0; i < artificiaisColunas.size(); i++)
	{
		// Como artificiais colunas possui o indice da coluna que está a nossa variável artificial, retiramos ela e utilizamos -i
		// pois a cada iteração removemos um valor e as posições mudam em -1
		coeficientesFuncao.erase(coeficientesFuncao.begin() + artificiaisColunas[i] - i);
	}
	
	// Faz a mesma logica anteriormente, só que percorendo cada linha de restrições
	for(int i = 0; i < artificiaisColunas.size(); i++)
	{
		for(int j = 0; j < coeficientesB.size(); j++)
		{
			coeficientesA[j].erase(coeficientesA[j].begin() + artificiaisColunas[i] - i);
		}
	}
}







// ALgoritmo do método Simplex
void Simplex(vector < double > &coeficientesDaFuncao, vector< vector<double> > &coeficientesRestricoes, vector < double > &coeficientesB, vector <double> &coeficientesW, vector <int> &artificiaisColunas,
bool &temArtificiais)
{
	int indiceColunaPivo; // Coluna pivo
	int indiceLinhaElementoPivo; // Indice que vai indicar o elemento pivo
	double elementoDivisor; // Elemento que irá dividir os valores da linha pivo
	vector <double>::iterator menorValor; // iterator que irá armazenar endereço de memoria do menor valor

	while(true)
	{
		// Caso eu tenha variável artificial, vamos fazer todo o metodo simplex levando em conta o W como função objetivo que tende a minimizar
		if(temArtificiais)
		{
			// O menor valor então, será o menor valor do vetor W
			menorValor = min_element(coeficientesW.begin(), coeficientesW.end() - 1);

			// Caso o valor retornado como menor, seja >= 0 o valor de W seja 0, é porque terminamos a primeira fase
			if (*menorValor >= 0 && coeficientesW[coeficientesW.size() - 1] == 0)
			{
				// Remove-se as variaveis artificiais e indica que não tem mais variável artificial
				removeArtificiais(coeficientesDaFuncao, coeficientesRestricoes, coeficientesB, artificiaisColunas);
				temArtificiais = false;	
				continue;
			}
			// Caso seja >= 0 e o valor de W seja != 0 é porque o sistema não tem solução
			if(*menorValor >= 0 && coeficientesW[coeficientesW.size() - 1] != 0)
			{
				cout << "Sistema não tem solução" << endl;
				break;
			}
		}
		else
		{
				
			// Primeiro vamos procurar o menor elemento da coluna
		 	// min_element -> retorna o endereço de posição do menor elemento contido em um vector dado os seus ranges
			menorValor = min_element(coeficientesDaFuncao.begin(), coeficientesDaFuncao.end() - 1);

			if(*menorValor >= 0)
				break;
		}

		// Pega o valor do indice da colunaPivo
		// Como menorValor possui o endereço de um elemento no vector e .begin() retorna o endereço de inicial
		// Para saber a posicao é so fazer a diferença
		
		if(temArtificiais)
		{
			indiceColunaPivo = menorValor - coeficientesW.begin();
		}
		else
		{
			indiceColunaPivo = menorValor - coeficientesDaFuncao.begin();
		}
		
		
		//Armazena o indice do elementoPivo
		indiceLinhaElementoPivo = retornaLinhaPivo(coeficientesB, coeficientesRestricoes, indiceColunaPivo);
		if(indiceLinhaElementoPivo == -1)
		{
			cout << "Sistema sem solução." << endl;
			break;
		}

		cout << "A linha do elemento Pivo vale: " << indiceLinhaElementoPivo << endl;
		//Elemento que vamos dividir a linha pivo	
		cout << "O menor valor eh: " << *menorValor << endl;	
		elementoDivisor = coeficientesRestricoes[indiceLinhaElementoPivo][indiceColunaPivo];
		cout << "O elemento divisor eh " << elementoDivisor << endl;	
		// Arruma a linha do elemento pivo, dividindo todos os elementos pelo elementoDivisor
		for(int i = 0; i < coeficientesRestricoes[indiceLinhaElementoPivo].size(); i++)
		{
			// Mantém fixo a linha da matriz, que é a linha pivo, e divide os valores
			coeficientesRestricoes[indiceLinhaElementoPivo][i] /= elementoDivisor;
		  	 
		}
		// Muda o valor do coeficieteB que está na linha pivo
		coeficientesB[indiceLinhaElementoPivo] /= elementoDivisor;
		
		// Muda os valores da função objetivo para zerar o elemento da coluna pivo	
		retornaNovoVetorCoeficientesFuncao(coeficientesDaFuncao, coeficientesRestricoes[indiceLinhaElementoPivo], coeficientesB[indiceLinhaElementoPivo], indiceColunaPivo);
		
		
		// Agora devemos zerar todos os elementos da coluna pivo, menos a nova linha
			
		retornaNovaMatrizCoeficientesRestricoes(coeficientesRestricoes, coeficientesRestricoes[indiceLinhaElementoPivo], coeficientesB[indiceLinhaElementoPivo], indiceColunaPivo, indiceLinhaElementoPivo, coeficientesB );		
			
		
		if(temArtificiais)
		{
			// ajeita o vetor de W
			retornaNovoVetorCoeficientesFuncao(coeficientesW, coeficientesRestricoes[indiceLinhaElementoPivo], coeficientesB[indiceLinhaElementoPivo], indiceColunaPivo);
		}

		printaTableau(coeficientesDaFuncao, coeficientesRestricoes, coeficientesB);			

		

	}

}



// Armazena os indices da solução
void solucaoFinal(vector <int> &indiceSolucoes, vector < vector < double > > &coeficientesA, vector <double> &coeficientesB, vector <double> &coeficientesFuncao)
{
	double soma = 0;
	int j = 0;

	int indiceLinhaResposta;
	// i é o valor da coluna
	for(int i = 0; i < coeficientesFuncao.size(); i++)
	{
		// J é o valor da linha
		for( j = 0; j < coeficientesB.size(); j++)
		{
			if(coeficientesA[j][i] == 1)
			{
				soma++;
			}
		}
		if(soma == 1)
		{
			
			indiceSolucoes.push_back(i);
		}
		soma = 0;

	}

}


void printaSolucaoFinal(const vector < int > &indiceColuna, const vector < double > &coeficientesB, const vector < vector < double > > &coeficientesA)
{
	cout << "As variáveis na base são:\n";

	cout << "[ ";
	for(int  i = 0; i < indiceColuna.size(); i++)
	{
		cout << indiceColuna[i] + 1;

		cout << (i== indiceColuna.size() - 1 ? "]\n": " , ");
	}
	
	cout << endl;
	cout << "[ ";
        int k = 0;

	for(int j : indiceColuna)
	{
		
		for(int  i = 0; i < coeficientesB.size(); i++)
		{
			if(coeficientesA[i][j] == 1)
			{
				cout << coeficientesB[i];
				cout << (k == indiceColuna.size() - 1 ? "]\n" :" , ");
			}
		}
		k++;
		
		
	}

}




void retornaDualidadeCoeficientesFuncao(vector < double > &coeficientesFuncaoDual, const vector < double > &coeficientesBPrimal)
{
	// Cria coeficientes da função do dual
	for(int i = 0; i < coeficientesBPrimal.size(); i++)
	{
		coeficientesFuncaoDual.push_back(coeficientesBPrimal[i]);
	}
	
}


void retornaDualidadeCoeficientesBDual(bool ehMinimizacao, vector < double > &coeficientesBDual, const vector < double > &coeficientesFuncaoPrimal)
{
	// Vetor do subject_to do dual
	for(int i = 0; i < coeficientesFuncaoPrimal.size(); i++)
	{
		// Corrige o valor caso seja de maximização
		if(!ehMinimizacao)
		{
			coeficientesBDual.push_back( -1 * coeficientesFuncaoPrimal[i]);
		}
		else
		{
			coeficientesBDual.push_back(coeficientesFuncaoPrimal[i]);
		}
		
	}
	
}

void retornaDualidadeCoeficientesADual(vector < vector < double > > &coeficientesADual, const vector < double > &coeficientesFuncaoPrimal, const vector < double > &coeficientesBPrimal, const vector < vector < double > > &coeficientesAPrimal)
{
	vector < double > valoresATemporarios; // vector que armazena temporariamente os valores da coluna para armazenar nas restrições do dual
	// Manter fixo os valores da coluna
	for(int j = 0; j < coeficientesFuncaoPrimal.size(); j++)
	{
		// varia os valores da linha
		for(int i = 0; i < coeficientesBPrimal.size(); i++)
		{
			valoresATemporarios.push_back(coeficientesAPrimal[i][j]);
		}

		coeficientesADual.push_back(valoresATemporarios);
		valoresATemporarios.clear();
	}	

}


void retornaRestricoesVariaveisDual(bool ehMinimizacao, vector <double> &coeficientesBDual, vector < int > &restricoesDual, vector < int > &restricoesVariaveisDual, const vector < int > &restricoesPrimal)
{

	if(!ehMinimizacao)
	{
		// Como o problema é de maximização e todas as variáveis são >= 0, então todas as restrições do dual serão >=
		for(int i = 0; i < coeficientesBDual.size(); i++)
		{
			//Adiciona-se em todas as linhas o valor de 2, indicando que é uma resintrção de >=
			restricoesDual.push_back(2);
		}
		
		// Percorre pelas restrições do primal para indicar a relação de cada variável
		for(int i = 0; i < restricoesPrimal.size(); i++)
		{
			// Caso a restrição seja de igualdade
			// A variável do dual tem que ser livre, logo não coloca restriçao
			if(restricoesPrimal[i] == 0)
			{
				restricoesVariaveisDual.push_back(-1); // coloca-se -1 para indicar livre
			}
			// Caso seja uma restrição de <=
			// A variável tem que ser de >=
			else if(restricoesPrimal[i] == 1)
			{
				restricoesVariaveisDual.push_back(2);
			}
			// Caso seja uma restrição de >=
			// A variavel tem que ser de <=
			else if(restricoesPrimal[i] == 2)
			{
				restricoesVariaveisDual.push_back(1);
			}
		}
	}
	else
	{
		// Como o problema é de minimização e todas as variaveis são >= 0, as restrições serão <=
		for(int i = 0; i < coeficientesBDual.size(); i++)
		{
			// Adiciona as restrições de <=
			restricoesDual.push_back(1);
		}

		// Percorre pelas restrições do primal, para indicar a relação de cada variável
		for(int i = 0; i < restricoesPrimal.size(); i++)
		{
			// Caso a restrição do primal seja igual a zero, a variável é livre
			if(restricoesPrimal[i] == 0)
			{
				restricoesVariaveisDual.push_back(-1); // -1 para indicar que é livre
			}
			
			// Caso a restrição do primal seja igual de <=, a variável também tem que ser <=
			else if(restricoesPrimal[i] == 1)
			{
				restricoesVariaveisDual.push_back(1);
			}
			// Caso a restrição do primal seja de >=, a variável será >= 0
			else if(restricoesPrimal[i] == 2)
			{
				restricoesVariaveisDual.push_back(2);
			}
		}

	}
}



void printaModelo(const vector < double > &coeficientesDaFuncao,  vector < vector < double > > &coeficientesRestricoes, const vector < double > &coeficientesB, const vector < int > restricoes, const vector < int > restricoesVariaveis)
{
	for(int i = 0; i < coeficientesDaFuncao.size(); i++)
    {
        cout << setw(10) << setprecision(2) << coeficientesDaFuncao[i] << " ";
    }
    cout << endl;
	int k  = 0;
    for(vector <double > &linha : coeficientesRestricoes)
    {
        for( double  &valor : linha )
        {
            cout << setw(10) <<  valor << " ";
        }
		
		if(restricoes[k] == 0)
		{
			cout << " = ";
		}
		else if(restricoes[k] == 1)
		{
			cout << " <= ";
		}
		else if(restricoes[k] == 2)
		{
			cout << " >= ";
		}
		cout <<  setw(10) << setprecision(2) << coeficientesB[k];
		k++;
        cout << endl;

    }

	cout << "As variaveis são do tipo:\n";

	for(int i = 0; i < restricoesVariaveis.size(); i++)
	{
		cout << "x(" << i + 1 <<") ";
		if(restricoesVariaveis[i] == -1)
		{
			cout << "livre" << endl;
		}
		else if(restricoesVariaveis[i] == 1)
		{
			cout << " <= 0" << endl;
		}
		else if(restricoesVariaveis[i] == 2)
		{
			cout << " >= 0" << endl;
		}
	}


    cout << "------------------------------------------------------------------------------------------------" << endl;
    
	getchar();
}


// Função responsável por armazenar em um vetor os precos sombra, que correspondem ao valor das variáveis de decisão
// Do problema dual, que dado a quantia de restrições, exemplo 3 restrições no problema primal, serão as 3 ultimas variaveis
// do vetor de valores da função objetivo 
void armazenaPrecoSombra(vector < double > &precosSombra, const vector < double > &valoresObjetivo, int quantiaRestricoes)
{
	// Como, dependendo da quantia de restrições, por exemplo, 5, os valores das variáveis de decisão do dual
	// Será dado pelos 5 ultimos elementos da primeira linha da ultima iteração do simplex.
	// então, pega-se o tamanho da função objetivo e retira 1, pois o indice inicia em 0 e o ultimo elemento eh o valor da função objetivo
	// e então soma pela quantia de restrições para ficar no indice do primeiro valor do dual
	int indice = valoresObjetivo.size() - (1 + quantiaRestricoes);
	
	while(indice < valoresObjetivo.size() - 1)
	{
		
		precosSombra.push_back(valoresObjetivo[indice]);
		indice++;

	}
}


// Função responsável por armazenar a matriz inversa da base, para poder calcular os range dos valores de B 
void armazenaBaseInversa(vector < vector < double > > &coeficientesInversoBase, const vector < vector < double > > &coeficientesRestricoes, int quantiaRestricoes, int quantiaColunas)
{
	int indiceColuna = quantiaColunas - (1 + quantiaRestricoes); // Vamos iniciar no indice da coluna da nossa primeira variável do dual
	vector < double > coeficientesInversoBaseTemporarios; // Vetor que armazena temporariamente os valores daquela linha
	
	// Então acessa a linha, copia os vallores a partir daquela da coluna do IndiceColuna até o fim da linha
	for(int i = 0; i < quantiaRestricoes; i++)
	{
		coeficientesInversoBaseTemporarios = {coeficientesRestricoes[i].begin() + indiceColuna, coeficientesRestricoes[i].end()};
		coeficientesInversoBase.push_back(coeficientesInversoBaseTemporarios);
	}
}


int main()
{
	bool ehMinimizacao; // Variavel que indica se eh ou não de minimização 
	
	// Vector contendo os coeficientes da função objetivo
	vector < double > coeficientesDaFuncao;

	// Matriz de restrições onde cada valor contido na coluna m corresponde ao coeficiente da variável Xm//
	// 1 2 3 
	// 3 5 6
	// 
	// 1 e 3 correspondem a coluna 0 no programa, mas vamos ler como 0 + 1, logo, corresponde a coluna 1 e são coeficientes da variável X1
	vector < vector < double > > coeficientesRestricoes;
	vector < int > indiceSolucaoFinal; // Indice das soluções finais	
	vector < double > valoresBase; // indice das linhas da solução final

	bool possuiArtificial = false; // variável booleana que indica se tem variavel artificial
	
	// Array que vai armazenar as desiguldades de cada coluna m
	// No qual cada indice representa a relacao daquela linha
	// 0 -> igualdade
	// 1- > menor igual
	// 2 -> maior igual
	vector <int> relacoesRestricoes;

	// Valores de cada valor da coluna B
	vector < double > coeficientesB;


	// Restricoes de cada variavel
	vector < int > restricoesVariaveis;

	// Recebe todas as variáveis como entrada
	recebeInformacoes(ehMinimizacao, coeficientesDaFuncao, coeficientesRestricoes, relacoesRestricoes, coeficientesB, restricoesVariaveis);
	

	vector < double > coeficientesFuncaoDual; // Função objetivo do problema dual
	vector < double > coeficientesBDual;    // coeficientes do subject do problema dual
    	vector < vector < double > > coeficientesADual;// coeficientes de A do problema dual
	vector < int > restricoesDual; // Tipos de restrições do problema dual	
	vector < int > restricoesVariaveisDual; // restrições das variaveis
	
	// Funções que criam os vetores do dual
	retornaDualidadeCoeficientesFuncao(coeficientesFuncaoDual, coeficientesB);
	retornaDualidadeCoeficientesBDual(ehMinimizacao, coeficientesBDual, coeficientesDaFuncao);
	retornaDualidadeCoeficientesADual(coeficientesADual, coeficientesDaFuncao, coeficientesB,  coeficientesRestricoes);
	retornaRestricoesVariaveisDual(ehMinimizacao, coeficientesBDual, restricoesDual, restricoesVariaveisDual, relacoesRestricoes);
	
	std::cout << "Printa modelo Dual\n";
	printaModelo(coeficientesFuncaoDual, coeficientesADual, coeficientesBDual, restricoesDual, restricoesVariaveisDual); // Printa o modelo dual
	vector <int > artificiaisIndices; // indices das linhas que foram adicionadas as variáveis artificiais
	vector <int> artificiaisColunas; // Coluna das variaveis artificiais para podermos zerar no vetor W	
	std::cout << "----------------------------------------------------------" << std::endl;

	// Transforma o problema no modelo standart
	transformaProblemaPadraoPL(coeficientesDaFuncao, coeficientesRestricoes, relacoesRestricoes, coeficientesB, artificiaisIndices, artificiaisColunas, possuiArtificial);

	printaTableau(coeficientesDaFuncao, coeficientesRestricoes, coeficientesB);// Printa o problema na forma padrão
	
	vector < double > coeficientesW; // Coeficientes do vetor auxiliar de W
	
	coeficientesW.insert(coeficientesW.end(), coeficientesDaFuncao.size(), 0); // inicializa com valores de 0
	// Se tivermos variaveis artificiais, cria-se o vetor W	
	if(artificiaisIndices.size() != 0)
	{
		
		criaVetorW(coeficientesW, coeficientesRestricoes, artificiaisIndices, artificiaisColunas, coeficientesB); // Vetor W referente aos custos das variaveis artificiais, o qual devemos zerar

	}

	Simplex(coeficientesDaFuncao, coeficientesRestricoes, coeficientesB, coeficientesW, artificiaisColunas, possuiArtificial);
	if(ehMinimizacao)
	{
		coeficientesDaFuncao[coeficientesDaFuncao.size() - 1] *= -1;
	}
	cout << coeficientesDaFuncao[coeficientesDaFuncao.size() - 1] << endl;
	
	solucaoFinal(indiceSolucaoFinal, coeficientesRestricoes, coeficientesB, coeficientesDaFuncao);

	printaSolucaoFinal(indiceSolucaoFinal, coeficientesB, coeficientesRestricoes);
	
	cout << endl;

	vector < double > precosSombra; // Valores do preço sombra, que são as variáveis do dual
	
	// CoeficientesB possui os valores de B, então seu .size() representa a quantia de restrições do problema
	armazenaPrecoSombra(precosSombra, coeficientesDaFuncao, coeficientesB.size());
	
	cout << "Precos Sombra: " << std::endl;
	for(int i = 0; i < precosSombra.size(); i++){
		cout << precosSombra[i] << " ";
	}
	cout << endl;
	vector < vector < double >  > coeficientesInversoBase; // Coeficiencies que vamos utilizar para calcular o range do lado direiro;
	
	// CoeficientesB.size() indica a quantia de restrições
	// CoeficientesDaFuncao.size() indica a quantia de coluna
	armazenaBaseInversa(coeficientesInversoBase, coeficientesRestricoes, coeficientesB.size(), coeficientesDaFuncao.size());
	
	cout << "Valores do inverso da base" << std::endl;
	/*
	for(int i = 0; i < coeficientesInversoBase.size(); i++)
	{
		for(int j = 0; j < coeficientesInversoBase[i].size(); j++)
		{
			cout << coeficientesInversoBase[i][j] << " ";
		}
		cout << endl;
	}
	*/ // Lembrar que coeficientesInversoBase.size() representa o numero de linhas da matriz
	
	// Para saber os range precisa-se do vetor b final e a matriz inversa de B 
	
	int linhaRequeridaRange = 0; // Variável que se refere a linha da restrição que estamos calculando o range de b
	
	vector < vector < double > > limitantes; // matriz dos limitantes de cada linha, cada indice refere-se a um vetor que são os limitantes daquela linha
	vector < vector < int > > limitantesRestricoes; // Matriz de restricoes de cada linha, onde teremos um vetor que representa a relação de cada elemento sendo 1 -> <= e 2 >=
																	
	// Ou seja, enquanto essa linha for menor que a quantia de restrições, iremos repetir o loop
	while(linhaRequeridaRange < coeficientesB.size())
	{
		// A linhaRequeridaRange representa a coluna da matriz inversa B que vamos multiplicar por certa icógnita e somar com a mesma linha
		// no vetor b e fazer >= 0
		
		// Percorre-se as linhas, que é exatamente o tamanho da matriz e mantém fixo o valor da coluna que é o valor de linhaRequeridaRange
		// Para saber qual valor ele deve ser >= ou <=, vamos dividir o valor da matriz inversa de B daquela linha pelo valor de b inverso final
		// daquela mesma linha
		double limitante;
	       	vector < double> limitantesTemporarias;
		vector < int > limitantesRestricoesTemporarias;		
		for(int i  = 0; i < coeficientesInversoBase.size(); i++)
		{	
			if(coeficientesInversoBase[i][linhaRequeridaRange] != 0)
			{
				
				limitante = (-1 * coeficientesB[i]) / coeficientesInversoBase[i][linhaRequeridaRange];
			}
			else
			{
				limitante = INFINITY; // Transforma o valor em infinito para ignorar
			}
			limitantesTemporarias.push_back(limitante);
			
			// Coloca 0 na restrição daquele valor, que vai nos indicar adiante para ignorar essa restrição
			if(limitante == INFINITY)
			{
				limitantesRestricoesTemporarias.push_back(0);
			}
			
			// Se o valor for menor do que zero, é porque vamos multiplicar aquela linha por -1, então mudamos a restrição original de >= para menor <=
			if(coeficientesInversoBase[i][linhaRequeridaRange] < 0)
			{
				limitantesRestricoesTemporarias.push_back(1);
			}
			else
			{
				limitantesRestricoesTemporarias.push_back(2);
			}
		}
		
		limitantes.push_back(limitantesTemporarias); // Adiciona o vetor a limites
		limitantesRestricoes.push_back(limitantesRestricoesTemporarias); // Adiciona o vetor a limitantesRestricoes
		linhaRequeridaRange++;	
	}
	
		
}	
