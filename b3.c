//o codigo ainda nao esta completo, estou quase finalizando...


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int get_split_size(char *line)
{
  unsigned size = 0;
  char *c = line;
  while (*c != '\0')
  {
    if (*c == ';')
      size++;
    c++;
  }
  return size + 1;
}

char **malloc_tokens(char *line, unsigned *size)
{
  *size = get_split_size(line);
  char **tokens = malloc(sizeof(char *) * (*size));
  tokens[0] = line;
  return tokens;
}

char **split(char *line, char *delim, unsigned *size)
{
  unsigned pos = 0;
  char *c = NULL;

  char **tokens = malloc_tokens(line, size);
  for (c = tokens[0] + 1; *c != '\0'; c++)
  {
    if (*c == delim[0])
    {
      *c = '\0';
      pos++;
      tokens[pos] = c + 1;
    }
  }

  return tokens;
}

void check_endline(FILE *data)
{
  char c = fgetc(data);
  if (c != '\n')
  {
    ungetc(c, data);
  }
}

char *readline(FILE *data)
{
  int size = 4096;
  size_t len_read = 0;
  char *line = (char *)malloc(sizeof(char) * size);
  int line_lenght = 0;
  do
  {
    len_read = fread(&line[line_lenght], sizeof(char), 1, data);
    if (len_read > 0)
    {
      line_lenght++;
      if (line_lenght == size)
      {
        size <<= 1;
        line = realloc(line, sizeof(char) * size);
      }
    }
  } while (line[line_lenght - 1] != '\n' && line[line_lenght - 1] != '\r' && len_read > 0);

  if (line_lenght == 0){
    return NULL;
  }
  line[line_lenght - 1] = '\0';
  check_endline(data);

  line = realloc(line, line_lenght);
  return line;
}

void free_tokens(char **tokens)
{
  free(tokens[0]);
  free(tokens);
  tokens = NULL;
}

void display_on_screen(/*recebe os tipos de filtros*/){

  
}

typedef struct colunas
  {
    char RptDt[20];
    char TckrSymb[100];
    char Asst[20];
    char XprtnDt[20];
    char ExrcPric[20];
    char OptnStyle[20];
    char CvrdQty[20];
    char TtlBlckPos[20];
    char UcvrdQty[20];
  } 
  dados;

void save_to_arquive(long int *total_linhas, dados *coluna){
  FILE *save = fopen("save.csv", "w");
  
  for(int i = 0; i < *total_linhas; i++){
    fprintf(save, "%s;%s;%s;", coluna[i].RptDt, coluna[i].TckrSymb, coluna[i].Asst);
    fprintf(save, "%s;%s;%s;", coluna[i].XprtnDt, coluna[i].ExrcPric, coluna[i].OptnStyle);
    fprintf(save, "%s;%s;%s\n", coluna[i].CvrdQty, coluna[i].TtlBlckPos, coluna[i].UcvrdQty);
  }
  fclose(save);
}


dados * save_to_struct(int tam , dados *coluna, char *line, char *line2, char **tokens,
 char **instruments, FILE *arquivo, FILE *arquivo2, unsigned int size, unsigned int size2, long int *total_linhas){
  long int i = 0;
  int fim = 0;
  int fim1 = 0;
  for (i = 0; i >= 0; i++)
  { 
    if ((tam) >= i)
    {
      coluna = (dados *) realloc(coluna, (sizeof(dados) * (tam + 5)));
    }
    
    line = readline(arquivo);

    if (feof(arquivo))
    {
      tokens = NULL;
      fim = 1;
    }
    else tokens = split(line, ";", &size);

    line2 = readline(arquivo2);
    
    if (feof(arquivo2))
    {
      instruments = NULL;
      line2 = NULL;
      fim1 = 1;
      *total_linhas = i;
      return coluna;
    }
    else  instruments = split(line2, ";", &size2);
          strcpy(coluna[i].TckrSymb, instruments[1]);
          strcpy(coluna[i].RptDt, instruments[0]);
          strcpy(coluna[i].Asst, instruments[2]);
          strcpy(coluna[i].XprtnDt, instruments[7]);
          strcpy(coluna[i].ExrcPric, instruments[35]);
          strcpy(coluna[i].OptnStyle, instruments[36]);
          strcpy(coluna[i].CvrdQty, "");
          strcpy(coluna[i].TtlBlckPos, "");
          strcpy(coluna[i].UcvrdQty, "");
          //printf("i = %li %s\n", i, coluna[i].RptDt);
          tam++;
          //coluna = (dados *) realloc(coluna, sizeof(dados) * tam);
    
    if (tokens != NULL && instruments != NULL && strcmp(instruments[1], tokens[1]) == 0)
    {
      strcpy(coluna[i].CvrdQty, tokens[9]);
      strcpy(coluna[i].TtlBlckPos, tokens[10]);
      strcpy(coluna[i].UcvrdQty, tokens[11]);
      tam++;
      //coluna = (dados *) realloc(coluna, sizeof(dados) * tam);
      //printf("i = %li %s\n", i, coluna[i].RptDt);
    }

    else if (fim == 0)
    {
      i++;
      strcpy(coluna[i].TckrSymb, tokens[1]);
      strcpy(coluna[i].RptDt, tokens[0]);
      strcpy(coluna[i].Asst, tokens[3]);
      strcpy(coluna[i].XprtnDt, "");
      strcpy(coluna[i].ExrcPric, "");
      strcpy(coluna[i].OptnStyle, "");
      strcpy(coluna[i].CvrdQty, tokens[9]);
      strcpy(coluna[i].TtlBlckPos, tokens[10]);
      strcpy(coluna[i].UcvrdQty, tokens[11]);
      //printf("i = %li %s\n", i, coluna[i].RptDt);
      tam++;
      //coluna = (dados *) realloc(coluna, sizeof(dados) * tam);
    }
  }
}
   


int main(int argc, char **argv)
{
  FILE *arquivo = fopen(argv[1], "r");
  FILE *arquivo2 = fopen(argv[2], "r");
  char **tokens = NULL, **instruments = NULL;
  unsigned int size, size2;
  char *line = NULL, *line2 = NULL; //line = arquivo 1, line2 = arquivo 2
  int tam = 4;
  long int total_linhas = 0;
  dados *coluna;
  coluna = (malloc(sizeof(dados) * tam));

  printf("=====Dados dos Derivativos=====\n");
  printf("|      Codigo      |    Ativo   |    Vcto    |  Strike  |   Tipo    |  Coberto  |   Travado   |  Descob.  |\n");
  coluna = save_to_struct(tam, coluna, line, line2, tokens, instruments, arquivo, arquivo2, size, size2, &total_linhas);
  //printf("total de linhas: %ld\n", total_linhas);
  save_to_arquive(&total_linhas, coluna);
  /*for (int i = 0; i < 8; i++)
  {
    line = readline(arquivo);
    tokens = split(line, ";", &size);
    do
    {
      if (instruments != NULL)
      {
        free_tokens(instruments);
        instruments = NULL;
      }
      line2 = readline(arquivo2);
      if (line2 != NULL)
      {
        instruments = split(line2, ";", &size2);
      }
      
    } while ((strcmp(tokens[1], instruments[1]) != 0) && line2 != NULL);

    printf("|  %-15s | %-10s | %-10s ", tokens[1], instruments[2], instruments[10]);
    printf("| %8s | %9s ", instruments[35], instruments[36]);
    printf("| %9s | %11s | %9s |\n", tokens[9], tokens[10], tokens[11]);

    free_tokens(tokens);
    tam++;
    coluna = realloc(coluna, (tam * sizeof(dados)));
  } */
  
  //free_tokens(instruments);
  free(instruments);
  free(tokens);
  free(coluna);
  fclose(arquivo);
  fclose(arquivo2);
}
