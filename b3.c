#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **split(char *line, char *sep, unsigned *length){
    int max = 64;
    *length = 0;
    char **array = NULL;

    array = (char**) malloc(sizeof(char*) * max);
    char *token = strtok(line, sep);
    while(token != NULL){
        array[*length] = token;
        (*length)++;
        if(*length==max){
            max <<= 1;
            array = realloc(array, max);
        }
        token = strtok(NULL, sep);
    }
    array = realloc(array, *length * sizeof(char*));
    return array;
}

void check_endline(FILE *data, char *line, int i){
    char c = fgetc(data);
    if( c != '\n' ){
        ungetc(c, data);
    }
}

char* readline(FILE *data){
    int size = 4096;
    char *line = malloc(sizeof(char) * size);
    int i = 0;
    do{
        fread(&line[i], sizeof(char), 1, data);
        i++;
        if( i == size ){
            size<<=1;
            line = realloc(line, sizeof(char) * size);
        }
        if(feof(data)){
          return line = NULL;
        }
    }while( line[i-1] != '\n' && line[i-1] != '\r' );
    line[i-1] = '\0';
    check_endline(data, line, i);
    line = realloc(line, strlen(line)+1);
    return line;
}

void printline(char **tokens, unsigned size){
  for(int i = 0; i < size; i++){
    if(i>=7 && i <= 9)
      printf("%9s | ", tokens[i]);
    else
    printf("%s | ", tokens[i]);
  }
  printf("\n");
}

int main(int argc, char **argv){
  FILE *arquivo = fopen(argv[1], "r");
  FILE *arquivo2 = fopen(argv[2], "r");
  char **tokens = NULL;
  unsigned int size;
  char *line;

  typedef struct dados{
    char RptDt;
    char TckrSymb[size];
    char Asst;
    char XprtnDt;
    char ExrcPric;
    char OptnStyle;
    char CvrdQty;
    char TtlBlckPos;
    char UcvrdQty;

  } dados;
  
  dados *coluna = calloc( 17, sizeof(dados));

  printf("=====Dados dos Derivativos=====\n");
  for(int i = 0; i >= 0; i++){
    line = readline(arquivo);
    if (line == NULL)
    {
      break;
    }

    tokens = split(line, ";", &size);
    coluna = realloc(coluna, sizeof(dados) * 17);
    for (int j = 0; j < 17; j++)
    {
      if (j == 1)
      {
        strcpy(coluna->TckrSymb, tokens[j]);
      }
      /*if (j == 9)
      {
        strcpy(coluna->CvrdQty, tokens[j]);
      }*/

    }
    //printf("%s  |", coluna->CvrdQty);
    printf("%s\n", coluna->TckrSymb);
    //printline(tokens, size);
    free(tokens);
    free(line);
  }

  printf("\n\n=====Dados dos Instrumentos=====\n");
  for(int i = 0; i >= 0; i++){
    
    line = readline(arquivo2);
    if(line == NULL){
      break;
    }
    tokens = split(line, ";", &size);
    //printline(tokens, size);
    free(tokens);
    free(line);
  }
  printf("size :%i", size);
  fclose(arquivo);
  fclose(arquivo2);
}

//chamar readline em um laço para contar quantas linhas tem o arquivo e usar no tamanho dos dados da estrutura
