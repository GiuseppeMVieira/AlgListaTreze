#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Definição da estrutura do Nó para dados inteiros
typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;

// Definição da estrutura do Nó para o Exemplo 26 (string)
#define MAX_LINE_SIZE 250
typedef struct LineNode {
    char line[MAX_LINE_SIZE];
    struct LineNode* prev;
    struct LineNode* next;
} LineNode;

// Definição da estrutura da Lista Duplamente Encadeada com Descritor
typedef struct DoublyLinkedList {
    Node* head;
    Node* tail;
    int size; // Para a questão 1 (e otimização das outras)
} DoublyLinkedList;

// Definição da estrutura da Lista para o Exemplo 26 com Descritor
typedef struct FileList {
    LineNode* head;
    LineNode* tail;
    int size;
} FileList;

// --- FUNÇÕES BÁSICAS DE NÓ E LISTA ---

// Função auxiliar para criar um novo nó (Node)
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Erro de alocação de memória.\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

// Função para inicializar a lista com descritor
void initList(DoublyLinkedList* list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// Função auxiliar para impressão (para testes)
void printList(DoublyLinkedList* list) {
    if (list->head == NULL) {
        printf("Lista vazia.\n");
        return;
    }
    Node* current = list->head;
    printf("Lista: ");
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL (Tamanho: %d)\n", list->size);
}

// --- EXERCÍCIOS 1 A 5: CONSULTA ---

// 1. Escreva uma função que retorne o número de elementos de uma lista linear duplamente encadeada.
int listSize(DoublyLinkedList* list) {
    if (list == NULL) return 0;
    // Se a lista usa um descritor que armazena o tamanho, o acesso é O(1)
    return list->size;
    /*
    // Alternativa: Contando (O(n))
    int count = 0;
    Node* current = list->head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
    */
}

// 2. Escreva uma função que retorne o conteúdo do primeiro nó de uma lista linear duplamente encadeada.
int getFirstElement(DoublyLinkedList* list) {
    if (list == NULL || list->head == NULL) {
        printf("Erro: Lista vazia ou nula.\n");
        return -1; // Valor sentinela de erro
    }
    return list->head->data;
}

// 3. Escreva uma função que retorne o conteúdo do último nó de uma lista linear duplamente encadeada.
int getLastElement(DoublyLinkedList* list) {
    if (list == NULL || list->tail == NULL) {
        printf("Erro: Lista vazia ou nula.\n");
        return -1; // Valor sentinela de erro
    }
    // Graças ao descritor com 'tail', o acesso é O(1)
    return list->tail->data;
}

// Função auxiliar para encontrar o n-ésimo nó (útil para questões 8, 9, 12)
Node* getNodeAtPosition(DoublyLinkedList* list, int n) {
    if (list == NULL || n < 1 || n > list->size) {
        return NULL;
    }

    Node* current = list->head;
    // Otimização: se 'n' está na segunda metade, começa pelo 'tail' e vai para trás
    if (n > list->size / 2) {
        current = list->tail;
        for (int i = list->size; i > n; i--) {
            current = current->prev;
        }
    }
    else {
        // Se 'n' está na primeira metade, começa pelo 'head' e vai para frente
        for (int i = 1; i < n; i++) {
            current = current->next;
        }
    }
    return current;
}

// 4. Escreva uma função que verifique se um determinado elemento pertence a uma lista linear duplamente encadeada.
bool containsElement(DoublyLinkedList* list, int element) {
    if (list == NULL || list->head == NULL) return false;

    Node* current = list->head;
    while (current != NULL) {
        if (current->data == element) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// 5. Escreva uma função que retorne o número de vezes que um determinado elemento ocorre numa lista linear duplamente encadeada.
int countOccurrences(DoublyLinkedList* list, int element) {
    if (list == NULL || list->head == NULL) return 0;

    int count = 0;
    Node* current = list->head;
    while (current != NULL) {
        if (current->data == element) {
            count++;
        }
        current = current->next;
    }
    return count;
}

// --- EXERCÍCIOS 6 A 9: INSERÇÃO ---

// 6. Escreva uma função para inserir um elemento no início de uma lista linear duplamente encadeada.
void insertAtBeginning(DoublyLinkedList* list, int data) {
    Node* newNode = createNode(data);

    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    }
    else {
        newNode->next = list->head;
        list->head->prev = newNode;
        list->head = newNode;
    }
    list->size++;
}

// 7. Escreva uma função para inserir um elemento no final de uma lista linear duplamente encadeada.
void insertAtEnd(DoublyLinkedList* list, int data) {
    Node* newNode = createNode(data);

    if (list->tail == NULL) {
        list->head = newNode;
        list->tail = newNode;
    }
    else {
        newNode->prev = list->tail;
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->size++;
}

// 8. Escreva uma função para inserir um elemento depois do n-ésimo elemento de uma lista linear duplamente encadeada.
void insertAfterNth(DoublyLinkedList* list, int data, int n) {
    if (n < 1 || n > list->size) {
        printf("Erro: Posição %d fora do intervalo para inserção (1 a %d).\n", n, list->size);
        return;
    }

    if (n == list->size) {
        // Se for depois do último, usa a função de inserir no final (otimizado)
        insertAtEnd(list, data);
        return;
    }

    Node* nodeN = getNodeAtPosition(list, n);
    if (nodeN == NULL) {
        // Isso não deve ocorrer devido à verificação do 'n', mas é uma segurança.
        printf("Erro: Nó na posição %d não encontrado.\n", n);
        return;
    }

    Node* newNode = createNode(data);

    newNode->next = nodeN->next;
    newNode->prev = nodeN;
    nodeN->next->prev = newNode;
    nodeN->next = newNode;

    list->size++;
}

// 9. Escreva uma função para inserir um elemento antes do n-ésimo elemento de uma lista linear duplamente encadeada.
void insertBeforeNth(DoublyLinkedList* list, int data, int n) {
    if (n < 1 || n > list->size + 1) { // Permite n = size + 1 para o caso de ser antes do final (equivalente a depois do último)
        printf("Erro: Posição %d fora do intervalo para inserção (1 a %d).\n", n, list->size + 1);
        return;
    }

    if (n == 1) {
        // Se for antes do primeiro, usa a função de inserir no início (otimizado)
        insertAtBeginning(list, data);
        return;
    }

    // Se n for list->size + 1, a função insertAfterNth(list, data, list->size) já cobre.
    // Mas a questão é inserir ANTES do n-ésimo nó.
    Node* nodeN = getNodeAtPosition(list, n);
    if (nodeN == NULL) {
        // Caso em que n = size + 1 e a lista não está vazia (erro na lógica ou na chamada)
        printf("Erro: Nó na posição %d não encontrado (provavelmente n > size e n != size + 1).\n", n);
        return;
    }

    Node* newNode = createNode(data);

    newNode->next = nodeN;
    newNode->prev = nodeN->prev;
    nodeN->prev->next = newNode;
    nodeN->prev = newNode;

    list->size++;
}

// --- EXERCÍCIOS 10 A 14: REMOÇÃO ---

// Função auxiliar para remover um nó genérico (útil para 13 e 14)
void removeNode(DoublyLinkedList* list, Node* node) {
    if (list == NULL || node == NULL || list->head == NULL) return;

    // Se o nó a ser removido é a CABEÇA
    if (node->prev == NULL) { // node == list->head
        list->head = node->next;
        if (list->head != NULL) {
            list->head->prev = NULL;
        }
        else {
            // Se a lista ficou vazia
            list->tail = NULL;
        }
    }
    // Se o nó a ser removido é a CAUDA
    else if (node->next == NULL) { // node == list->tail
        list->tail = node->prev;
        if (list->tail != NULL) {
            list->tail->next = NULL;
        }
        // Não precisamos de um 'else' aqui, pois se 'head' e 'tail' forem NULL, o caso 'node->prev == NULL' já teria sido tratado.
    }
    // Se o nó está no MEIO
    else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    free(node);
    list->size--;
}


// 10. Escreva uma função para remover o primeiro elemento de uma lista linear duplamente encadeada.
int removeFirst(DoublyLinkedList* list) {
    if (list == NULL || list->head == NULL) {
        printf("Erro: Lista vazia para remover o primeiro elemento.\n");
        return -1; // Valor sentinela de erro
    }

    int data = list->head->data;
    Node* nodeToRemove = list->head;

    removeNode(list, nodeToRemove);
    return data;
}

// 11. Escreva uma função para remover o último elemento de uma lista linear duplamente encadeada.
int removeLast(DoublyLinkedList* list) {
    if (list == NULL || list->tail == NULL) {
        printf("Erro: Lista vazia para remover o último elemento.\n");
        return -1; // Valor sentinela de erro
    }

    int data = list->tail->data;
    Node* nodeToRemove = list->tail;

    removeNode(list, nodeToRemove);
    return data;
}

// 12. Escreva uma função para remover o n-ésimo elemento de uma lista linear duplamente encadeada.
int removeNth(DoublyLinkedList* list, int n) {
    if (list == NULL || n < 1 || n > list->size) {
        printf("Erro: Posição %d fora do intervalo (1 a %d) ou lista vazia.\n", n, list->size);
        return -1; // Valor sentinela de erro
    }

    Node* nodeToRemove = getNodeAtPosition(list, n);
    int data = nodeToRemove->data;

    removeNode(list, nodeToRemove);
    return data;
}

// 13. Dada uma lista linear duplamente encadeada e um elemento, escreva uma função que remova da lista todas as ocorrências do elemento.
void removeAllOccurrences(DoublyLinkedList* list, int element) {
    if (list == NULL || list->head == NULL) return;

    Node* current = list->head;
    Node* nextNode;

    while (current != NULL) {
        nextNode = current->next; // Guarda o próximo nó antes de possivelmente desalocar 'current'

        if (current->data == element) {
            removeNode(list, current); // removeNode atualiza list->head e list->tail se necessário e free(current)
        }
        current = nextNode; // Vai para o próximo nó salvo
    }
}

// 14. Escreva uma função para excluir todos os nós de uma lista linear duplamente encadeada.
void destroyList(DoublyLinkedList* list) {
    if (list == NULL) return;

    Node* current = list->head;
    Node* nextNode;

    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    printf("Lista destruída e memória liberada.\n");
}

// --- EXERCÍCIOS 15 A 17: ORDENAÇÃO E REMOÇÃO ORDENADA ---

// 15. Desenvolva uma função que permita a inserção ordenada (crescente) de elementos em uma lista linear duplamente encadeada.
void insertOrdered(DoublyLinkedList* list, int data) {
    Node* newNode = createNode(data);

    // Caso 1: Lista vazia
    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
        list->size++;
        return;
    }

    Node* current = list->head;
    // Percorre a lista até encontrar um nó cuja data é maior ou igual à nova data
    while (current != NULL && current->data < data) {
        current = current->next;
    }

    // Caso 2: Inserir no início (data é menor que list->head->data)
    if (current == list->head) {
        newNode->next = list->head;
        list->head->prev = newNode;
        list->head = newNode;
    }
    // Caso 3: Inserir no final (data é maior que todos os elementos, 'current' é NULL)
    else if (current == NULL) {
        newNode->prev = list->tail;
        list->tail->next = newNode;
        list->tail = newNode;
    }
    // Caso 4: Inserir no meio
    else {
        newNode->next = current;
        newNode->prev = current->prev;
        current->prev->next = newNode;
        current->prev = newNode;
    }

    list->size++;
}

// 16. Desenvolva uma função que permita a remoção de um elemento em uma lista linear *simplesmente* encadeada ordenada. 
// OBS: A lista atual é duplamente encadeada, vamos adaptar a lógica de *remoção por valor* para a DLL.
bool removeElementByValue(DoublyLinkedList* list, int element) {
    if (list == NULL || list->head == NULL) return false;

    Node* current = list->head;

    // Busca o elemento (Assumindo a lista ordenada, a busca pode parar)
    while (current != NULL && current->data < element) {
        current = current->next;
    }

    // Se encontrou o elemento
    if (current != NULL && current->data == element) {
        removeNode(list, current);
        return true;
    }

    return false; // Elemento não encontrado ou a lista é vazia
}


// 17. Desenvolva uma função que remova de uma lista linear duplamente encadeada ordenada todas as ocorrências de um determinado elemento.
// OBS: Esta função é idêntica à 13 (removeAllOccurrences), mas pode ser otimizada para uma lista *ordenada*.
void removeAllOccurrencesOrdered(DoublyLinkedList* list, int element) {
    if (list == NULL || list->head == NULL) return;

    Node* current = list->head;
    Node* nextNode;

    // Otimização para lista ordenada: avança até encontrar o primeiro elemento
    while (current != NULL && current->data < element) {
        current = current->next;
    }

    // Remove todas as ocorrências do elemento
    while (current != NULL && current->data == element) {
        nextNode = current->next;
        removeNode(list, current);
        current = nextNode;
    }
    // O loop para quando current é NULL ou current->data > element
}


// --- EXERCÍCIOS 18 A 25: MANIPULAÇÃO AVANÇADA ---

// 18. Crie uma função que permita inverter o sentido das referências de uma lista linear duplamente encadeada.
void reverseList(DoublyLinkedList* list) {
    if (list == NULL || list->head == NULL || list->head == list->tail) return;

    Node* current = list->head;
    Node* temp = NULL;

    // Troca os ponteiros prev e next de cada nó
    while (current != NULL) {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev; // Move para o 'novo' próximo (o antigo anterior)
    }

    // Atualiza head e tail
    temp = list->head; // 'temp' é o antigo 'head'
    list->head = list->tail; // O antigo 'tail' é o novo 'head'
    list->tail = temp; // O antigo 'head' é o novo 'tail'
}

// 19. Faça uma função para concatenar duas listas encadeadas duplamente encadeada.
void concatenateLists(DoublyLinkedList* list1, DoublyLinkedList* list2) {
    if (list2 == NULL || list2->head == NULL) {
        // Nada a fazer se a segunda lista estiver vazia ou for nula
        return;
    }
    if (list1 == NULL || list1->head == NULL) {
        // Se a primeira lista estiver vazia, apenas transfere os ponteiros
        list1->head = list2->head;
        list1->tail = list2->tail;
        list1->size = list2->size;
    }
    else {
        // Conecta o tail da lista 1 ao head da lista 2
        list1->tail->next = list2->head;
        list2->head->prev = list1->tail;

        // Atualiza o tail e o size da lista 1
        list1->tail = list2->tail;
        list1->size += list2->size;
    }

    // A lista 2 não deve mais apontar para os nós, agora controlados pela lista 1
    list2->head = NULL;
    list2->tail = NULL;
    list2->size = 0;
}


// 20. Escreva uma função para retornar o endereço e o conteúdo do último nó de uma lista linear duplamente encadeada.
// OBS: Retorna o ponteiro para o último nó. O conteúdo é acessado por '->data'.
Node* getLastNode(DoublyLinkedList* list, int* content) {
    if (list == NULL || list->tail == NULL) {
        *content = -1; // Valor sentinela
        return NULL;
    }
    *content = list->tail->data;
    return list->tail;
}


// 21. Escreva uma função que retorne a soma dos números de uma lista linear duplamente encadeada.
int sumElements(DoublyLinkedList* list) {
    if (list == NULL || list->head == NULL) return 0;

    int sum = 0;
    Node* current = list->head;
    while (current != NULL) {
        sum += current->data;
        current = current->next;
    }
    return sum;
}


// 22. Escreva uma função para criar uma cópia de uma lista linear duplamente encadeada.
DoublyLinkedList* copyList(DoublyLinkedList* originalList) {
    DoublyLinkedList* newList = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
    if (newList == NULL) {
        printf("Erro de alocação de memória para a nova lista.\n");
        exit(EXIT_FAILURE);
    }
    initList(newList);

    if (originalList == NULL || originalList->head == NULL) {
        return newList; // Retorna lista vazia
    }

    Node* current = originalList->head;
    while (current != NULL) {
        // Insere o elemento no final da nova lista
        insertAtEnd(newList, current->data);
        current = current->next;
    }

    return newList;
}

// Estrutura para o resultado do Ex. 23
typedef struct FreqResult {
    int maxElement;
    int maxCount;
    int minElement;
    int minCount;
} FreqResult;


// 23. Desenvolva uma função, que dado uma lista linear duplamente encadeada de números inteiros positivos, forneça os elementos que aparecem o maior e o menor número de vezes.
FreqResult findMinMaxFrequency(DoublyLinkedList* list) {
    FreqResult result = { 0, 0, 0, list->size + 1 }; // Inicializa minCount maior que o máximo possível

    if (list == NULL || list->head == NULL) {
        result.minCount = 0;
        return result;
    }

    // Para evitar O(n^2), usamos uma tabela de hash simples ou um array auxiliar.
    // Como estamos restritos a C puro e não é especificada a faixa de valores, 
    // faremos a implementação O(n^2) que percorre a lista para cada elemento
    // e marca os elementos já contados para evitar recontagem.

    // A maneira mais limpa em C puro seria usar uma segunda lista para rastrear frequências únicas
    typedef struct FreqNode {
        int data;
        int count;
        struct FreqNode* next;
    } FreqNode;
    FreqNode* freqHead = NULL;
    FreqNode* freqCurrent;

    Node* current = list->head;
    while (current != NULL) {
        int data = current->data;

        // Verifica se o elemento já está na lista de frequências
        freqCurrent = freqHead;
        bool found = false;
        while (freqCurrent != NULL) {
            if (freqCurrent->data == data) {
                freqCurrent->count++;
                found = true;
                break;
            }
            freqCurrent = freqCurrent->next;
        }

        // Se não encontrou, cria um novo nó de frequência e insere no início
        if (!found) {
            FreqNode* newFreqNode = (FreqNode*)malloc(sizeof(FreqNode));
            newFreqNode->data = data;
            newFreqNode->count = 1;
            newFreqNode->next = freqHead;
            freqHead = newFreqNode;
        }

        current = current->next;
    }

    // Percorre a lista de frequências para encontrar o máximo e o mínimo
    freqCurrent = freqHead;
    if (freqCurrent != NULL) {
        result.maxElement = freqCurrent->data;
        result.maxCount = freqCurrent->count;
        result.minElement = freqCurrent->data;
        result.minCount = freqCurrent->count;

        freqCurrent = freqCurrent->next;
    }

    while (freqCurrent != NULL) {
        if (freqCurrent->count > result.maxCount) {
            result.maxCount = freqCurrent->count;
            result.maxElement = freqCurrent->data;
        }
        if (freqCurrent->count < result.minCount) {
            result.minCount = freqCurrent->count;
            result.minElement = freqCurrent->data;
        }
        freqCurrent = freqCurrent->next;
    }

    // Libera a memória da lista de frequências
    freqCurrent = freqHead;
    FreqNode* nextFreq;
    while (freqCurrent != NULL) {
        nextFreq = freqCurrent->next;
        free(freqCurrent);
        freqCurrent = nextFreq;
    }

    return result;
}


// 24. Escreva um função que faça uma cópia de uma lista linear duplamente encadeada, eliminando os elementos repetidos.
DoublyLinkedList* copyListUnique(DoublyLinkedList* originalList) {
    DoublyLinkedList* newList = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
    if (newList == NULL) {
        printf("Erro de alocação de memória para a nova lista.\n");
        exit(EXIT_FAILURE);
    }
    initList(newList);

    if (originalList == NULL || originalList->head == NULL) {
        return newList; // Retorna lista vazia
    }

    Node* current = originalList->head;
    while (current != NULL) {
        // Insere o elemento apenas se ele *não* estiver presente na nova lista
        if (!containsElement(newList, current->data)) { // Reutiliza a função 4 (containsElement)
            insertAtEnd(newList, current->data);
        }
        current = current->next;
    }

    return newList;
}

// 25. Faça uma função que verifique se uma lista linear duplamente encadeada está ordenada ou não (a ordem pode ser crescente ou decrescente).
typedef enum OrderType {
    NOT_ORDERED,
    ASCENDING,
    DESCENDING
} OrderType;

OrderType checkOrder(DoublyLinkedList* list) {
    if (list == NULL || list->size <= 1) {
        // Uma lista com 0 ou 1 elemento é considerada ordenada
        return ASCENDING;
    }

    Node* current = list->head->next;
    bool isAscending = true;
    bool isDescending = true;

    while (current != NULL) {
        // Verifica se a ordem crescente foi quebrada
        if (current->data < current->prev->data) {
            isAscending = false;
        }
        // Verifica se a ordem decrescente foi quebrada
        if (current->data > current->prev->data) {
            isDescending = false;
        }

        current = current->next;
    }

    if (isAscending && isDescending) { // Todos os elementos são iguais
        return ASCENDING;
    }
    else if (isAscending) {
        return ASCENDING;
    }
    else if (isDescending) {
        return DESCENDING;
    }
    else {
        return NOT_ORDERED;
    }
}


// --- EXERCÍCIO 26: MANIPULAÇÃO DE ARQUIVO (PARCIAL) ---

// Função auxiliar para criar um novo nó de linha (LineNode)
LineNode* createLineNode(const char* line) {
    LineNode* newNode = (LineNode*)malloc(sizeof(LineNode));
    if (newNode == NULL) {
        printf("Erro de alocação de memória.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->line, line, MAX_LINE_SIZE - 1);
    newNode->line[MAX_LINE_SIZE - 1] = '\0'; // Garantir terminação nula
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

// Função para inserir uma linha no final da lista de arquivos
void insertLineAtEnd(FileList* list, const char* line) {
    LineNode* newNode = createLineNode(line);

    if (list->tail == NULL) {
        list->head = newNode;
        list->tail = newNode;
    }
    else {
        newNode->prev = list->tail;
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->size++;
}

// 26. Faça um programa que leia um arquivo texto e carregue cada linha em um nó de uma lista linear duplamente encadeada com descritor.
FileList loadFileToList(const char* filename) {
    FileList list;
    list.head = NULL;
    list.tail = NULL;
    list.size = 0;

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return list; // Retorna lista vazia com erro
    }

    char buffer[MAX_LINE_SIZE];
    while (fgets(buffer, MAX_LINE_SIZE, file) != NULL) {
        // Remove o '\n' se presente (melhora a visualização da linha)
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        insertLineAtEnd(&list, buffer);
    }

    fclose(file);
    return list;
}

// Função para simular a navegação (parcial)
void navigateFile(FileList* list) {
    if (list == NULL || list->head == NULL) {
        printf("A lista de arquivos está vazia.\n");
        return;
    }

    LineNode* currentLine = list->head;
    char command;

    printf("\n--- Navegação do Arquivo (Simulação) ---\n");
    printf("Comandos: 's' (sobe), 'd' (desce), 'x' (sair)\n");
    printf("Linha Atual: %s\n", currentLine->line);

    while (true) {
        printf("Comando ('s'/'d'/'x'): ");
        // Limpa o buffer de entrada antes de ler o comando
        while (getchar() != '\n');

        if (scanf(" %c", &command) != 1) continue;

        if (command == 'x' || command == 'X') {
            break;
        }
        else if (command == 's' || command == 'S') {
            if (currentLine->prev != NULL) {
                currentLine = currentLine->prev;
            }
            else {
                printf("Já está na primeira linha (topo).\n");
            }
        }
        else if (command == 'd' || command == 'D') {
            if (currentLine->next != NULL) {
                currentLine = currentLine->next;
            }
            else {
                printf("Já está na última linha (base).\n");
            }
        }
        else {
            printf("Comando inválido.\n");
        }

        printf("Linha Atual: %s\n", currentLine->line);
    }
    printf("Fim da navegação.\n");
}


// Função para o programa principal e testes (Driver Code)
int main() {
    DoublyLinkedList myList;
    initList(&myList);

    printf("--- Testes das Funções da Lista Duplamente Encadeada ---\n");

    // Testes de Inserção (6, 7)
    printf("\n1. Inserção (6, 7):\n");
    insertAtEnd(&myList, 10); // 10
    insertAtBeginning(&myList, 5); // 5, 10
    insertAtEnd(&myList, 20); // 5, 10, 20
    insertAtBeginning(&myList, 2); // 2, 5, 10, 20
    insertAtEnd(&myList, 2); // 2, 5, 10, 20, 2
    printList(&myList); // (Tamanho: 5)

    // Teste de Consulta (1, 2, 3, 4, 5)
    printf("\n2. Consultas (1, 2, 3, 4, 5):\n");
    printf("Tamanho (1): %d\n", listSize(&myList));
    printf("Primeiro elemento (2): %d\n", getFirstElement(&myList));
    printf("Último elemento (3): %d\n", getLastElement(&myList));
    printf("Contém 10? (4): %s\n", containsElement(&myList, 10) ? "Sim" : "Não");
    printf("Ocorrências de 2 (5): %d\n", countOccurrences(&myList, 2));

    // Testes de Inserção em Posição (8, 9)
    printf("\n3. Inserção por Posição (8, 9):\n");
    // Inserir 15 depois do 3º (10). Lista: 2, 5, 10, 15, 20, 2
    insertAfterNth(&myList, 15, 3);
    // Inserir 7 antes do 3º (10). Lista: 2, 5, 7, 10, 15, 20, 2
    insertBeforeNth(&myList, 7, 4);
    printList(&myList); // (Tamanho: 7)

    // Testes de Remoção (10, 11, 12, 13)
    printf("\n4. Remoção (10, 11, 12, 13):\n");
    removeFirst(&myList); // Remove 2. Lista: 5, 7, 10, 15, 20, 2
    removeLast(&myList); // Remove 2. Lista: 5, 7, 10, 15, 20
    removeNth(&myList, 3); // Remove o 3º (10). Lista: 5, 7, 15, 20
    printList(&myList); // (Tamanho: 4)

    // Adiciona mais repetições para o teste 13
    insertAtEnd(&myList, 15);
    insertAtEnd(&myList, 5);
    insertAtEnd(&myList, 15);
    printList(&myList); // Lista: 5, 7, 15, 20, 15, 5, 15 (Tamanho: 7)
    removeAllOccurrences(&myList, 15); // Remove todos os 15s.
    printf("Após remover todos os 15s (13):\n");
    printList(&myList); // Lista: 5, 7, 20, 5 (Tamanho: 4)

    // Teste de Inserção Ordenada (15)
    printf("\n5. Inserção Ordenada (15):\n");
    destroyList(&myList); // Esvazia a lista (14)
    insertOrdered(&myList, 10);
    insertOrdered(&myList, 5);
    insertOrdered(&myList, 20);
    insertOrdered(&myList, 15);
    insertOrdered(&myList, 5);
    printf("Lista ordenada (15):\n");
    printList(&myList); // Lista: 5, 5, 10, 15, 20 (Tamanho: 5)

    // Teste de Remoção por Valor (16)
    printf("\n6. Remoção por Valor (16):\n");
    removeElementByValue(&myList, 10);
    printf("Após remover um '10' (16):\n");
    printList(&myList); // Lista: 5, 5, 15, 20 (Tamanho: 4)

    // Teste de Remoção de Múltiplas Ocorrências Ordenada (17)
    printf("\n7. Remoção de Múltiplas Ocorrências Ordenada (17):\n");
    removeAllOccurrencesOrdered(&myList, 5);
    printf("Após remover todos os '5's (17):\n");
    printList(&myList); // Lista: 15, 20 (Tamanho: 2)

    // Teste de Inversão (18)
    printf("\n8. Inversão (18):\n");
    insertAtBeginning(&myList, 10); // Lista: 10, 15, 20
    reverseList(&myList);
    printf("Lista após inversão (18):\n");
    printList(&myList); // Lista: 20, 15, 10 (Tamanho: 3)

    // Teste de Concatenação (19)
    printf("\n9. Concatenação (19):\n");
    DoublyLinkedList list2;
    initList(&list2);
    insertAtEnd(&list2, 1);
    insertAtEnd(&list2, 2); // list2: 1, 2
    concatenateLists(&myList, &list2);
    printf("Lista 1 após concatenação (19):\n");
    printList(&myList); // Lista: 20, 15, 10, 1, 2 (Tamanho: 5)

    // Teste de Endereço/Conteúdo do Último (20)
    printf("\n10. Último Nó (20):\n");
    int lastContent = 0;
    Node* lastNode = getLastNode(&myList, &lastContent);
    printf("Endereço do último nó (20): %p, Conteúdo: %d\n", (void*)lastNode, lastContent);

    // Teste de Soma (21)
    printf("\n11. Soma dos Elementos (21):\n");
    printf("Soma: %d\n", sumElements(&myList)); // 20+15+10+1+2 = 48

    // Teste de Cópia (22)
    printf("\n12. Cópia (22):\n");
    DoublyLinkedList* copy = copyList(&myList);
    printf("Cópia (22): ");
    printList(copy);
    destroyList(copy);

    // Teste de Frequência (23)
    printf("\n13. Frequência (23):\n");
    insertAtEnd(&myList, 20); // Lista: 20, 15, 10, 1, 2, 20 (Tamanho: 6)
    insertAtEnd(&myList, 1);
    FreqResult freq = findMinMaxFrequency(&myList);
    printf("Lista para Freq: ");
    printList(&myList); // Lista: 20, 15, 10, 1, 2, 20, 1 (Tamanho: 7)
    printf("Maior Frequência: Elemento %d (ocorrências: %d)\n", freq.maxElement, freq.maxCount); // 20 ou 1 (2 vezes)
    printf("Menor Frequência: Elemento %d (ocorrências: %d)\n", freq.minElement, freq.minCount); // 15, 10, 2 (1 vez)

    // Teste de Cópia com Únicos (24)
    printf("\n14. Cópia com Únicos (24):\n");
    DoublyLinkedList* uniqueCopy = copyListUnique(&myList);
    printf("Cópia Única (24): ");
    printList(uniqueCopy); // Lista: 20, 15, 10, 1, 2 (Tamanho: 5)
    destroyList(uniqueCopy);

    // Teste de Ordenação (25)
    printf("\n15. Verificação de Ordem (25):\n");
    printf("Lista atual: "); printList(&myList);
    OrderType order = checkOrder(&myList);
    printf("Ordem: %s\n", order == ASCENDING ? "Crescente" : (order == DESCENDING ? "Decrescente" : "Não Ordenada")); // Não Ordenada

    DoublyLinkedList orderedList; initList(&orderedList);
    insertAtEnd(&orderedList, 1);
    insertAtEnd(&orderedList, 5);
    insertAtEnd(&orderedList, 10);
    printf("Lista ordenada (25): "); printList(&orderedList);
    order = checkOrder(&orderedList);
    printf("Ordem: %s\n", order == ASCENDING ? "Crescente" : (order == DESCENDING ? "Decrescente" : "Não Ordenada")); // Crescente
    destroyList(&orderedList);

    // Teste de Destruição (14)
    printf("\n16. Destruição da Lista (14):\n");
    destroyList(&myList);

    // Teste de Arquivo (26)
    printf("\n17. Carregamento e Navegação de Arquivo (26):\n");

    // Crie um arquivo de exemplo para o teste
    FILE* example_file = fopen("exemplo_lista.txt", "w");
    if (example_file != NULL) {
        fprintf(example_file, "Linha 1 do arquivo\n");
        fprintf(example_file, "Linha 2\n");
        fprintf(example_file, "Última linha\n");
        fclose(example_file);
        printf("Arquivo 'exemplo_lista.txt' criado para o teste.\n");
    }

    FileList fileList = loadFileToList("exemplo_lista.txt");
    printf("Conteúdo do arquivo carregado (26):\n");
    LineNode* currentLine = fileList.head;
    int i = 1;
    while (currentLine != NULL) {
        printf("Linha %d: %s\n", i++, currentLine->line);
        currentLine = currentLine->next;
    }

    // Navegação interativa (simulação)
    // navigateFile(&fileList);

    // Limpeza da lista de arquivos
    LineNode* current = fileList.head;
    LineNode* nextNode;
    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }

    return 0;
}