#include <iostream>
#include <vector>

using namespace std;

// Um estado pode ser não final, ou ser um estado final de aceitação ou rejeição
enum EstadoFinal
{
    NAO_FINAL,
    ACEITACAO,
    REJEICAO
};

// Representa um estado de uma MT
struct estado
{
    string nome;             // Nome do estado
    bool estadoInicial;      // Indica se um estado é ou não inicial
    EstadoFinal estadoFinal; // Indica se um estado não é final, ou se é final de aceitação ou rejeição
};

// Representa uma transição da função de transição (δ) da MT
struct transicao
{
    string estadoOrigem;  // Nome do estado de origem da transição
    string simboloLido;   // Simbolo lido pela cabeça de leitura
    string estadoDestino; // Nome do estado de destino
    string novoSimbolo;   // Simbolo a ser escrito na fita
    char direcao;         // Direcao para a qual a cabeça de leitura e escrita será movida (E ou D)
};

// Verifica se uma transição é válida (estados pertencem ao conjunto de estados da MT e símbolos pertencem ao alfabeto da fita)
bool transicaoValida(pair<estado*, int> *estados, string **alfabetoFita, int qtdSimbolosFita, string estado, string simbolo, char direcao)
{
    // Verificando se o estado é válido
    bool estadoValido = false;
    for (int i = 0; i < estados->second; i++)
    {
        if (estados->first[i].nome == estado)
        {
            estadoValido = true;
            i = estados->second;
        }
    }

    // Retornando false caso o estado não seja válido
    if (!estadoValido)
    {
        return false;
    }

    // Verificando se o símbolo é válido
    bool simboloValido = false;
    for (int i = 0; i < qtdSimbolosFita; i++)
    {
        if ((*alfabetoFita)[i] == simbolo)
        {
            simboloValido = true;
            i = qtdSimbolosFita;
        }
    }

    bool direcaoValida = false;
    if (direcao == 'E' or direcao == 'D')
    {
        direcaoValida = true;
    }

    // Verificando se o estado, símbolo e direção são válidos
    if (estadoValido and simboloValido and direcaoValida)
    {
        return true;
    }

    return false;
}

// Lê as informações da Máquina de Turing a ser simulada
void lerMT(pair<estado*, int> *estados, string **alfabeto, string **alfabetoFita, transicao **funcaoTransicao, vector<string> *fita, string &branco)
{
    int qtdEstados = 0;         // Número de estados da MT
    int qtdSimbolosEntrada = 0; // Número de símbolos do alfabeto de entrada
    int qtdSimbolosFita = 0;    // Número de símbolos do alfabeto da fita

    cout << "Digite as informacoes da Maquina de Turing" << endl;

    // Lendo a quantidade de estados (No mínimo 2)
    while (true)
    {
        cout << "Quantidade de estados: ";
        cin >> qtdEstados;

        if (qtdEstados > 2)
        {
            break;
        }

        cout << "A MT deve ter ao menos dois estados (aceitacao e rejeicao)!" << endl;
    }

    // Lendo cada um dos estados
    estados->first = new estado[qtdEstados];
    estados->second = qtdEstados;
    cout << "Nome de cada um dos " << qtdEstados << " estados de sua MT: ";
    for (int i = 0; i < qtdEstados; i++)
    {
        string estado;
        cin >> estado;
        estados->first[i].nome = estado;
        estados->first[i].estadoInicial = false;   // Inicialmente nenhum estado é inicial
        estados->first[i].estadoFinal = NAO_FINAL; // Inicialmente todos são não_finais
    }

    // Lendo o estado inicial e verificando se ele existe na lista de estados
    while (true)
    {
        string estadoInicial; // Estado inicial da MT (q0)

        // Lendo o estado inicial
        cout << "Estado inicial: ";
        cin >> estadoInicial;

        // Verificando se o estado inicial é válido
        bool estadoEncontrado = false;
        for (int i = 0; i < qtdEstados; i++)
        {
            if (estados->first[i].nome == estadoInicial)
            {
                estados->first[i].estadoInicial = true;
                estadoEncontrado = true;
                i = qtdEstados;
            }
        }

        if (estadoEncontrado)
        {
            break;
        }

        cout << "Estado " << estadoInicial << " invalido! O estado inicial deve ser um dos estados informados anteriormente." << endl;
    }

    // Lendo os estados finais de aceitação e rejeição da MT
    cout << "Nome dos estados de aceitacao e rejeicao, respectivamente: ";
    while (true)
    {
        string aceitacao;
        string rejeicao;
        cin >> aceitacao;
        cin >> rejeicao;

        // Verificando se os dois estados são iguais
        if (aceitacao == rejeicao)
        {
            cout << "Um estado nao pode ser de aceitacao e rejeicao ao mesmo tempo!" << endl;
        }
        else
        {
            // Verificando se os estados existem
            bool aceitacaoEncontrado = false;
            bool rejeicaoEncontrado = false;
            for (int i = 0; i < qtdEstados; i++)
            {
                if (estados->first[i].nome == aceitacao)
                {
                    estados->first[i].estadoFinal = ACEITACAO;
                    aceitacaoEncontrado = true;
                }
                if (estados->first[i].nome == rejeicao)
                {
                    estados->first[i].estadoFinal = REJEICAO;
                    rejeicaoEncontrado = true;
                }

                // Encerrando o loop for
                if (aceitacaoEncontrado and rejeicaoEncontrado)
                {
                    i = qtdEstados;
                }
            }

            if (aceitacaoEncontrado and rejeicaoEncontrado)
            {
                break;
            }

            cout << "Algum dos estados eh invalido. Digite dois dos estados informados anteriormente" << endl;
        }
    }

    // Lendo a quantidade de símbolos do alfabeto de entrada
    while (true)
    {
        cout << "Quantidade de simbolos do alfabeto: ";
        cin >> qtdSimbolosEntrada;

        if (qtdSimbolosEntrada > 0)
        {
            break;
        }

        cout << "Alfabeto de entrada nao pode ser vazio!" << endl;
    }

    // Lendo o alfabeto de entrada da MT
    *alfabeto = new string[qtdSimbolosEntrada];
    cout << "Simbolos do alfabeto: ";
    for (int i = 0; i < qtdSimbolosEntrada; i++)
    {
        string simbolo;
        cin >> simbolo;
        (*alfabeto)[i] = simbolo;
    }

    // Lendo a quantidade de símbolos do alfabeto da fita
    while (true)
    {
        cout << "Quantidade de simbolos do alfabeto da fita (incluindo o branco): ";
        cin >> qtdSimbolosFita;

        if (qtdSimbolosFita >= qtdSimbolosEntrada + 1)
        {
            break;
        }

        cout << "Alfabeto da fita deve conter, ao menos, " << qtdSimbolosEntrada + 1 << " simbolos!" << endl;
    }

    // Lendo o alfabeto da fita
    *alfabetoFita = new string[qtdSimbolosFita];
    cout << "Simbolos do alfabeto da fita (incluindo o branco): ";
    for (int i = 0; i < qtdSimbolosFita; i++)
    {
        string simbolo;
        cin >> simbolo;
        (*alfabetoFita)[i] = simbolo;
    }

    // Lendo o símbolo branco
    while (true)
    {
        cout << "Simbolo do alfabeto da fita que corresponde ao branco: ";
        cin >> branco;

        bool brancoEncontrado = false;
        for (int i = 0; i < qtdSimbolosFita; i++)
        {
            if ((*alfabetoFita)[i] == branco)
            {
                i = qtdSimbolosFita;
                brancoEncontrado = true;
            }
        }

        if (brancoEncontrado)
        {
            break;
        }
        cout << "Simbolo invalido! O simbolo branco deve ser um dos simbolos informados para o alfabeto da fita." << endl;
    }

    // Número de transições na função de transição é igual a (número de estados -2) multiplicado pelo número de símbolos do alfabeto da fita
    // Estados de aceitação e rejeição não necessitam de função de transição pois finalizam a MT imediatamente
    if (qtdEstados > 2)
    {
        int qtdTransicoes = (qtdEstados - 2) * qtdSimbolosFita;
        *funcaoTransicao = new transicao[qtdTransicoes];

        // Lendo a função de transição completa da MT
        int contTransicao = 0;
        cout << "Digite a funcao de transicao para cada par de estado nao final e simbolo da fita: " << endl;
        for (int i = 0; i < qtdEstados - 2; i++)
        {
            for (int j = 0; j < qtdSimbolosFita; j++)
            {
                while (true)
                {
                    string estadoOrigem;  // Nome do estado de origem da transição
                    string simboloLido;   // Simbolo lido pela cabeça de leitura
                    string estadoDestino; // Nome do estado de destino
                    string novoSimbolo;   // Símbolo a ser escrito na fita
                    char direcao;         // Direcao para a qual a cabeça de leitura/escrita será movida (E ou D)

                    // Lendo a transição para o estado e símbolo atuais
                    estadoOrigem = estados->first[i].nome;
                    simboloLido = (*alfabetoFita)[j];
                    cout << "(" << estadoOrigem << ", " << simboloLido << ") -> ";
                    cin >> estadoDestino >> novoSimbolo >> direcao;

                    // Verificando se a transição é válida e atribuindo ela ao vetor da função de transição
                    if (transicaoValida(estados, alfabetoFita, qtdSimbolosFita, estadoDestino, novoSimbolo, direcao))
                    {
                        (*funcaoTransicao)[contTransicao].estadoOrigem = estadoOrigem;
                        (*funcaoTransicao)[contTransicao].simboloLido = simboloLido;
                        (*funcaoTransicao)[contTransicao].estadoDestino = estadoDestino;
                        (*funcaoTransicao)[contTransicao].novoSimbolo = novoSimbolo;
                        (*funcaoTransicao)[contTransicao].direcao = direcao;
                        contTransicao++;

                        break;
                    }

                    cout << "Transicao invalida! Verifique os estados, simbolos e direcao informados." << endl;
                }   
            }
        }
    }

    // Lendo a fita de entrada
    int tamanhoEntrada = 0;
    cout << "Digite o tamanho (quantidade de simbolos) da fita de entrada: ";
    cin >> tamanhoEntrada;

    // Lendo cada um dos símbolos da fita de entrada
    if (tamanhoEntrada > 0)
    {
        cout << "Digite cada um dos simbolos da fita de entrada: ";
        for (int i = 0; i < tamanhoEntrada; i++)
        {
            string entrada;
            cin >> entrada;
            fita->push_back(entrada);
        }
    }
    fita->push_back(branco); // Adicionando o símbolo branco ao final da fita
}


int main()
{
    cout << "Iniciado" << endl;
    pair<estado*, int> estados;                // Par com o conjunto de estados (Q) e o seu tamanho (|Q|)
    string *alfabeto;                          // Alfabeto de entrada (∑)
    string *alfabetoFita;                      // Alfabeto da fita (Γ)
    transicao *fTransicao;                     // Função de transição (δ)
    vector<string> *fita = new vector<string>; // Fita de entrada
    string branco;                             // Símbolo branco (_)

    cout << "Simulador de Maquina de Turing Universal" << endl;
    lerMT(&estados, &alfabeto, &alfabetoFita, &fTransicao, fita, branco);

   
    delete[] estados.first;
    delete[] alfabeto;
    delete[] alfabetoFita;
    delete[] fTransicao;
    delete[] fita;

    return 0;
}