/*
Crie um programa em python em que você vai criar sua própria TODO LIST;
-Uma tarefa deve possuir:
  .Identificador (melhor usar um  numérico)
  .Título
  .Descrição
  .Status(feito ou não)
  .Data de Entrega
-O programa deve ficar rodando "direto" esperando o comando do usuário. Dentre eles temos: 
.1 - Listar no terminal todas as tarefas 
.2 - Acessar uma tarefa específica (através do id) 
.3 - Registra uma nova tarefa 
.4 - Atualizar uma tarefa (através do id) 
.5 - Deletar uma tarefa (através do id) 
-Registrar tarefas em um arquivo txt, alterando sempre que possível; 
-Carregar o txt no início do programa 
-Fazer todas as validações necessárias: 
  .comando inválido
  .Parar o programa
  .Id inexistente

EXTRA(300XP)
-Usar classe do python ou struct no C p/ apresentar a tarefa;
-Implementar um comando que busca um dado específico em uma página (EASTER EGG)
-> Web scrapping
*/

//#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME_ARQ "tarefas.txt"

typedef struct tarefa Tarefa;
typedef struct no No;

struct tarefa {
    int id;
    char titulo[200];
    char descricao[200];
    char status[200];
    char data[11];
};

struct no {
    Tarefa tarefa;
    No* pProx;
};

 No* readArq1Person(FILE* pArq);
 No* add(No* pNew, No* pHead);
 void read(No* pHead);
 void search(No* pHead, int id);
 No* insert(No* pHead);
 void update(No* pHead, int id);
 No* del(No* pHead, int id);
 void writeArq(No* pHead, FILE* ArqOut);
 void freeMemory(No* pHead);


int main(void) {
    FILE* ArqIn = NULL;
    FILE* ArqOut = NULL;
    char op;
    int n;
    No* pHead = NULL;
    No* pNew = NULL;
    int id;
    
    ArqIn = fopen(NAME_ARQ, "r");   
    if (!ArqIn) {
        printf("Erro ao abrir o arquivo.\n");
    }
    else{
        while (1) {
          pNew = readArq1Person(ArqIn);
          if (pNew) {
            pHead = add(pNew, pHead);
          } 
          else if (feof(ArqIn)) {
            break;
          }
          else {
            printf("Erro na leitura do arquivo!\n");
            break;
          }
          n = fscanf(ArqIn, "\n");
          if (n == EOF) {
            break;
          }
        }
          /*if (!pNew){
            break;
          }*/         
      fclose(ArqIn);
    }
    
    do {
        printf("Insira a opcao desejada (se deseja terminar o programa, insira o numero 6): ");
        scanf(" %c", &op);

        switch (op)
        {
        case '1': //ler todas
            read(pHead);
            //printf("1");
            break;
        case '2': //achar um
            printf("Insira o id da tarefa que deseja encontrar (o id e numerico): ");
            scanf(" %d", &id);
            search(pHead, id);
            //printf("2");
            break;
        case '3':
            pHead = insert(pHead);
            //printf("3");
            break;
        case '4':
            printf("Insira o id da tarefa que deseja atualizar (o id e numerico): ");
            scanf(" %d", &id);
            update(pHead, id);
            //printf("4");
            break;
        case '5':
            printf("Insira o id da tarefa que deseja excluir (o id e numerico): ");
            scanf(" %d", &id);
            pHead = del(pHead, id); 
            //printf("5");
            break;
        case '6': // se for 6, o codigo para de rodar
            break;
        default:
            printf("Codigo para comando inexistente, insire um numero valido");
            break;
        }
        printf("\n");
        
    } while (op != '6');

    ArqOut = fopen(NAME_ARQ, "w");
    if (!ArqOut) {
        printf("Erro ao abrir arquivos\n");
        exit(3);
    }

    writeArq(pHead, ArqOut);
    fclose(ArqOut);

    freeMemory(pHead);

    return 0;
}



No* readArq1Person(FILE* pArq) {
    char line[1000];
    int id;
    char titulo[200] = {'\0'};
    char descricao[200] = {'\0'};
    char status[200] = {'\0'};
    char data[11] = {'\0'};
    No* pAux;
    int n;

    if (fgets(line, sizeof(line), pArq) != NULL) {
      n = sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,]\n", &id, titulo, descricao, status, data); //espaço na frente do %d tiraria o /n
      if (n == 5) {
          pAux = (No*)malloc(sizeof(No));
          if (!pAux) {
              return NULL;
          }
          pAux->tarefa.id = id;
          strcpy(pAux->tarefa.titulo, titulo);
          strcpy(pAux->tarefa.descricao, descricao);
          strcpy(pAux->tarefa.status, status);
          strcpy(pAux->tarefa.data, data);
          return pAux;
      }
    }
    
    return NULL;
  
}

No* add(No* pNew, No* pHead) {
    pNew->pProx = pHead;
    pHead = pNew;
    return pHead;
}

void read(No* pHead) {
    No* pAux;
    pAux = pHead;

    if (!pAux) {
        printf("Nao ha tarefas previamente declaradas");
        return;
    }

    while (pAux) {
        printf("Id da tarefa: %d", pAux->tarefa.id);
        if (pAux->tarefa.titulo != NULL) {
            printf(", titulo: %s", pAux->tarefa.titulo);
        }
        if (pAux->tarefa.descricao != NULL) {
            printf(", descricao: %s", pAux->tarefa.descricao);
        }
        if (pAux->tarefa.status != NULL) {
            printf(", status: %s", pAux->tarefa.status);
        }
        if (pAux->tarefa.data != NULL) {
            printf(", data: %s", pAux->tarefa.data);
        }
        printf("\n");
        pAux = pAux->pProx;
    }
}



void search(No* pHead, int id){
    No* pAux = pHead;
    if (!pAux) {
        printf("Nao ha tarefas previamente declaradas");
        return;
    }
    while (pAux->tarefa.id != id) {
        pAux = pAux->pProx;
    }

    if (!pAux) {
        printf("Nao ha tarefas com esse id previamente declarado");
        return;
    }

    printf("Tarefa: %d", pAux->tarefa.id);
    if (pAux->tarefa.titulo != NULL) {
        printf(", titulo: %s", pAux->tarefa.titulo);
    }
    if (pAux->tarefa.descricao != NULL) {
        printf(", descricao: %s", pAux->tarefa.descricao);
    }
    if (pAux->tarefa.status != NULL) {
        printf(", status: %s", pAux->tarefa.status);
    }
    if (pAux->tarefa.data != NULL) {
        printf(", data: %s\n", pAux->tarefa.data);
    }
}

No* insert(No* pHead) {
    No* pAux;
    int id;
    char titulo[200] = {'\0'};
    char descricao[200] = {'\0'};
    char status[200] = {'\0'};
    char data[11] = {'\0'};
    pAux = (No*)malloc(sizeof(No));
    if (!pAux) {
        printf("Erro ao alocar memória");
        exit(4);
    }
    printf("Insira o id da nova tarefa: ");
    scanf("%d", &id);
    pAux->tarefa.id = id;
    fflush(stdin);
    printf("Insira o titulo da nova tarefa: ");
    scanf("%s", titulo);
    strcpy(pAux->tarefa.titulo, titulo);
    fflush(stdin);
    printf("Insira a descricao da nova tarefa: ");
    scanf("%s", descricao);
    strcpy(pAux->tarefa.descricao, descricao);
    fflush(stdin);
    printf("Insira o status da nova tarefa: ");
    scanf("%s", status);
    strcpy(pAux->tarefa.status, status);
    printf("Insira a data da nova tarefa: ");
    scanf("%s", data);
    strcpy(pAux->tarefa.data, data);
    fflush(stdin);
    pAux->pProx = pHead;
    return pAux;
}
void update(No* pHead, int id) {
    No* pAux = pHead;
    if (!pAux) {
        printf("Não ha tarefas previamente declaradas");
        return;
    }
    char* infos[] = {"titulo", "descricao", "status", "data"};
    
    while (pAux->tarefa.id != id && !pAux) {
        pAux = pAux->pProx;
    }

    if (!pAux) {
        printf("Não ha tarefas com esse id previamente declarado");
        return;
    }

    for (int i = 0; i < 4; i++) {            
            printf("Insira a atualização que deseja para o campo %s:", infos[i]);
            switch (i) {
            case 0:
                scanf("%s", pAux->tarefa.titulo);
                break;
            case 1:
                scanf("%s", pAux->tarefa.descricao);
                break;
            case 2:
                scanf("%s", pAux->tarefa.status);
                break;
            case 3:
                scanf("%s", pAux->tarefa.data);
                break;
            }
        
    }
}

No* del(No* pHead, int id) {
    No* pAux = pHead;
    No* pAnt = NULL;;

    if (!pAux) {
        printf("Nao ha tarefas previamente declaradas");
        return pHead;
    }
    while (pAux) {
        if (pAux->tarefa.id == id) {
            if (!pAnt) {
                //é o pHead
                pHead = pAux->pProx;
            }
            else {
                pAnt->pProx = pAux->pProx;
            }
            free(pAux);
            break;
        }
        pAnt = pAux;
        pAux = pAux->pProx;
    }

    if (!pAux) {
        printf("Nao ha tarefas com esse id previamente declarado");
    }

    return pHead;
}

void writeArq(No* pHead, FILE* ArqOut) {
    No* pAux = pHead;
    if (!pAux) {
        return;
    }
    while (pAux)
    {
        fprintf(ArqOut, "%d,%s,%s,%s,%s\n", pAux->tarefa.id, pAux->tarefa.titulo, pAux->tarefa.descricao,
            pAux->tarefa.status, pAux->tarefa.data);
        pAux = pAux->pProx;
    }    
}

void freeMemory(No* pHead) {
    No* pAux = pHead;
    No* pProx = NULL;

    while (pAux) {
        pProx = pAux->pProx;
        free(pAux);
        pAux = pProx;
    }
}