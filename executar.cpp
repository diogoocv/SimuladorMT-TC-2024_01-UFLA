#include <iostream>
#include <vector>

using namespace std;

// Número de iterações da MT para considerar que talvez haja um loop infinito
#define LOOP 2000

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

    // Sobrecarga do operador == para comparar dois estados
    bool operator==(const estado &other) const
    {
        return nome == other.nome &&
               estadoInicial == other.estadoInicial &&
               estadoFinal == other.estadoFinal;
    }
};

// Representa uma transição da função de transição (δ) da MT
struct transicao
{
    estado estadoOrigem;  // Estado de origem da transição
    string simboloLido;   // Simbolo lido pela cabeça de leitura
    estado estadoDestino; // Estado de destino da transição
    string novoSimbolo;   // Simbolo a ser escrito na fita
    char direcao;         // Direcao para a qual a cabeça de leitura e escrita será movida (E ou D)
};

// Verifica se uma transição é válida (estados pertencem ao conjunto de estados da MT e símbolos pertencem ao alfabeto da fita)
bool transicaoValida(pair<estado *, int> *estados, string **alfabetoFita, int qtdSimbolosFita, string estadoOrigem, string estadoDestino, string simboloLido, string novoSimbolo, char direcao)
{
    // Verificando se os estados de origem e destino são válidos
    bool origemEncontrada = false;
    bool destinoEncontrado = false;
    bool estadosValidos = false;
    for (int i = 0; i < estados->second; i++)
    {
        if (estados->first[i].nome == estadoOrigem)
        {
            origemEncontrada = true; // Estado de origem está contido em (Q)
        }

        if (estados->first[i].nome == estadoDestino)
        {
            destinoEncontrado = true; // Estado de destino está contido em (Q)
        }

        if (origemEncontrada and destinoEncontrado)
        {
            estadosValidos = true;
            i = estados->second;
        }
    }

    // Retornando false caso os estados não sejam válidos
    if (!estadosValidos)
    {
        return false;
    }

    // Verificando se os símbolos são válidos
    bool simboloLidoEncontrado = false;
    bool novoSimboloEncontrado = false;
    bool simbolosValidos = false;
    for (int i = 0; i < qtdSimbolosFita; i++)
    {
        if ((*alfabetoFita)[i] == simboloLido)
        {
            simboloLidoEncontrado = true; // Símbolo lido pertence ao alfabeto da fita
        }

        if ((*alfabetoFita)[i] == novoSimbolo)
        {
            novoSimboloEncontrado = true; // Novo símbolo pertence ao alfabeto da fita
        }

        if (simboloLidoEncontrado and novoSimboloEncontrado)
        {
            simbolosValidos = true;
            i = qtdSimbolosFita;
        }
    }

    bool direcaoValida = false;
    if (direcao == 'E' or direcao == 'D')
    {
        direcaoValida = true;
    }

    // Verificando se o estado, símbolo e direção são válidos
    if (estadosValidos and simbolosValidos and direcaoValida)
    {
        return true;
    }

    return false;
}

// Encontra um estado a partir de seu nome e o retorna
estado encontrarEstado(pair<estado *, int> estados, string nomeEstado)
{
    estado est;

    for (int i = 0; i < estados.second; i++)
    {
        if (estados.first[i].nome == nomeEstado)
        {
            est = estados.first[i];
        }
    }

    return est;
}

// Verifica se o alfabeto da fita contém todos os símbolos do alfabeto de entrada
bool verificarAlfabetoFita(string **alfabeto, string **alfabetoFita, int qtdSimbolosEntrada, int qtdSimbolosFita)
{
    // Verificando, para cada símbolo do alfabeto, se ele está presente no símbolo da fita
    for (int i = 0; i < qtdSimbolosEntrada; i++)
    {
        bool simboloEncontrado = false;

        for (int j = 0; j < qtdSimbolosFita; j++)
        {
            if ((*alfabeto)[i] == (*alfabetoFita)[j])
            {
                simboloEncontrado = true;
                j = qtdSimbolosFita;
                continue;
            }
        }

        if (!simboloEncontrado)
        {
            return false;
        }
    }

    return true;
}

// Lê as informações da Máquina de Turing a ser simulada
void lerMT(pair<estado *, int> *estados, string **alfabeto, string **alfabetoFita, pair<vector<transicao> *, int> *funcaoTransicao, string &branco)
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
    while (true)
    {
        *alfabetoFita = new string[qtdSimbolosFita];
        cout << "Simbolos do alfabeto da fita (incluindo o branco): ";
        for (int i = 0; i < qtdSimbolosFita; i++)
        {
            string simbolo;
            cin >> simbolo;
            (*alfabetoFita)[i] = simbolo;
        }

        // Verificando se o alfabeto da fita contém todos os símbolos do alfabeto de entrada
        if (verificarAlfabetoFita(alfabeto, alfabetoFita, qtdSimbolosEntrada, qtdSimbolosFita))
        {
            break;
        }

        cout << "Alfabeto da fita invalido! Deve conter todos os simbolos do alfabeto de entrada." << endl;
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

    funcaoTransicao->first = new vector<transicao>; // Inicializando o vetor de transições da função de transição

    int contTransicao = 0;
    cout << "Digite todas as transicoes da funcao de transicao no formato: q0 0 q1 1 D " << endl;
    cout << "Para finalizar, digite: -1" << endl;

    // Lendo a função de transição da MT
    while (true)
    {
        string nomeOrigem;  // Nome do estado de origem da transição
        string simboloLido; // Simbolo lido pela cabeça de leitura
        string nomeDestino; // Nome do estado de destino após a transição
        string novoSimbolo; // Símbolo a ser escrito na fita
        char direcao;       // Direcao para a qual a cabeça de leitura/escrita será movida (E ou D)

        // Lendo a transição
        cin >> nomeOrigem;
        if (nomeOrigem == "-1")
        {
            break; // Finalizando a leitura da função de transição
        }
        cin >> simboloLido >> nomeDestino >> novoSimbolo >> direcao;

        // Verificando se a transição é válida
        if (transicaoValida(estados, alfabetoFita, qtdSimbolosFita, nomeOrigem, nomeDestino, simboloLido, novoSimbolo, direcao))
        {
            // Procurando os estados de origem e destino de acordo com os nomes informados
            estado estadoOrigem = encontrarEstado(*estados, nomeOrigem);
            estado estadoDestino = encontrarEstado(*estados, nomeDestino);

            // Atribuindo os dados à função de transição
            transicao novaTransicao = {estadoOrigem, simboloLido, estadoDestino, novoSimbolo, direcao};
            funcaoTransicao->first->push_back(novaTransicao);

            // Atualizando o tamanho do vetor de transições
            contTransicao++;
            funcaoTransicao->second = contTransicao;
        }
        else
        {
            cout << "Transicao invalida! Verifique os estados, simbolos e direcao informados." << endl;
        }
    }
}

// Lê a fita de entrada da MT
void lerFita(pair<vector<string> *, int> *fita, string branco)
{
    fita->first = new vector<string>; // Inicializando a fita

    // Lendo a fita de entrada
    int tamanhoEntrada = 0;
    cout << "Digite o tamanho (quantidade de simbolos) da fita de entrada: ";
    cin >> tamanhoEntrada;

    // Lendo cada um dos símbolos da fita de entrada
    if (tamanhoEntrada > 0)
    {
        fita->second = tamanhoEntrada + 1; // Tamanho da fita é, inicialmente, o tamanho da entrada + 1 (caractere branco no final)
        cout << "Digite cada um dos simbolos da fita de entrada: ";
        for (int i = 0; i < tamanhoEntrada; i++)
        {
            string entrada;
            cin >> entrada;
            fita->first->push_back(entrada);
        }
    }
    fita->first->push_back(branco); // Adicionando o símbolo branco ao final da fita
}

// Simula uma MT
int simularMT(pair<estado *, int> *estados, pair<vector<transicao> *, int> *fTransicao, pair<vector<string> *, int> *fita, string branco)
{
    estado estadoAtual;    // Estado no qual será verificada a função de transição na iteração atual da MT
    int posFita = 0;       // Posição da cabeça de leitura/escrita na fita
    int contIteracoes = 0; // Contador de iteracoes da MT para identificar um possivel loop infinito

    // Atribuindo o estado inicial da MT ao estadoAtual
    for (int i = 0; i < estados->second; i++)
    {
        if (estados->first[i].estadoInicial)
        {
            estadoAtual = estados->first[i];
        }
    }

    while (true)
    {
        // Verificando se o estado atual não é final
        if (estadoAtual.estadoFinal == NAO_FINAL)
        {
            bool transicaoEncontrada = false;
            string simboloLido = (*fita->first)[posFita]; // Lendo o símbolo apontado pela cabeça de leitura/escrita

            // Verificando se existe uma transição do estado atual lendo o símbolo apontado pela cabeça de leitura/escrita
            for (int i = 0; i < fTransicao->second; i++)
            {
                if ((*fTransicao->first)[i].estadoOrigem == estadoAtual)
                {
                    if ((*fTransicao->first)[i].simboloLido == simboloLido)
                    {
                        estadoAtual = (*fTransicao->first)[i].estadoDestino;           // Indo para o estado de destino da transição
                        (*fita->first)[posFita] = (*fTransicao->first)[i].novoSimbolo; // Escrevendo na fita o novo símbolo da transição

                        // Atualizando a posição da cabeça de leitura e escrita
                        // Movendo para a direita
                        if ((*fTransicao->first)[i].direcao == 'D')
                        {
                            // Verificando se a cabeça de l/e será posicionada na última posição da fita
                            if (posFita >= (*fita).second - 1)
                            {
                                (*fita->first).push_back(branco); // Adicionando um símbolo branco ao final da fita
                                (*fita).second++;                 // Incrementando o tamanho da fita
                            }

                            posFita++; // Posicionando a cabeça de l/e uma posição à direita
                        }
                        // Tentando mover para a esquerda
                        else
                        {
                            // Verificando se a posição não é a primeira da fita
                            if (posFita > 0)
                            {
                                posFita--; // Posicionando a cabeça de l/e uma posição à esquerda
                            }
                            // Cabeça de l/e posicionada na primeira posição da fita
                            else
                            {
                                continue; // Não é possível mover para a esquerda, a cabeça de l/e permanece na mesma posição
                            }
                        }

                        transicaoEncontrada = true;
                        i = fTransicao->second;
                    }
                }
            }

            // Não existe uma transição para o estado atual e símbolo lido pela cabeça de l/e
            if (!transicaoEncontrada)
            {
                return 0; // Palavra rejeitada pela MT
            }
        }
        else
        {
            if (estadoAtual.estadoFinal == ACEITACAO)
            {
                return 1; // Palavra aceita pela MT
            }
            else
            {
                return 0; // Palavra rejeitada pela MT
            }
        }

        // Verificando se a maquina entrou em um possível loop
        if (contIteracoes > LOOP)
        {
            char op;
            cout << "A maquina de Turing pode ter entrado em loop. Deseja continuar? " << endl
                 << "(S/N): ";
            cin >> op;

            if (op != 's' and op != 'S')
            {
                return -1; // Execução da MT finalizada devido a um possível loop
            }
            else
            {
                contIteracoes = 0; // Reiniciando o contador de iterações
            }
        }
        contIteracoes++;
    }
}

// Imprime o conteúdo da fita
void imprimirFita(pair<vector<string> *, int> fita)
{
    for (int i = 0; i < fita.second; i++)
    {
        cout << (*fita.first)[i] << " ";
    }
    cout << endl;
}

int main()
{
    pair<estado *, int> estados;               // Par com o conjunto de estados (Q) e o seu tamanho (|Q|)
    string *alfabeto;                          // Alfabeto de entrada (∑)
    string *alfabetoFita;                      // Alfabeto da fita (Γ)
    pair<vector<transicao> *, int> fTransicao; // Par com a função de transição (δ) e o seu tamanho
    pair<vector<string> *, int> fita;          // Par com a fita de entrada e o seu tamanho
    string branco;                             // Símbolo branco (_)

    cout << "Simulador de Maquina de Turing Universal" << endl;

    lerMT(&estados, &alfabeto, &alfabetoFita, &fTransicao, branco); // Lendo a Máquina de Turing Específica (MTS)

    // Lendo fitas de entrada e simulando a MT com elas até que o usuário deseje finalizar o sistema
    while (true)
    {
        lerFita(&fita, branco);                                       // Lendo a fita de entrada
        int result = simularMT(&estados, &fTransicao, &fita, branco); // Simulando a MTS através da Máquina de Turing Universal (MTU)

        cout << endl
             << "SAIDA: " << endl;
        switch (result)
        {
        // Palavra aceita
        case 1:
        {
            cout << "A Maquina de Turing parou em um estado de ACEITACAO." << endl;
            cout << "A palavra de entrada pertence a linguagem da MT" << endl;
            cout << "Fita ao final da execucao: ";
            imprimirFita(fita);
            break;
        }

        // Palavra rejeitada
        case 0:
        {
            cout << "A Maquina de Turing parou em um estado de REJEICAO." << endl;
            cout << "A palavra de entrada nao pertence a linguagem da MT" << endl;
            cout << "Fita ao final da execucao: ";
            imprimirFita(fita);
            break;
        }

        // MT entrou em loop
        case -1:
        {
            cout << "A maquina entrou em um possivel loop. Nao foi possivel retornar um resultado" << endl;
            break;
        }

        default:
        {
            cout << "ERRO NA EXECUCAO!!" << endl;
            break;
        }
        }

        char op;
        cout << "Deseja executar a MT com outra fita de entrada? " << endl
             << "(S/N): ";
        cin >> op;

        if (op != 'S' and op != 's')
        {
            break; // Programa finalizado
        }
    }

    cout << "Programa finalizado!" << endl;

    // Deletando estruturas alocadas na memória
    delete[] estados.first;
    delete[] alfabeto;
    delete[] alfabetoFita;
    delete[] fTransicao.first;
    delete[] fita.first;

    return 0;
}