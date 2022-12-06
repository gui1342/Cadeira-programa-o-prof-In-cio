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

  if (line_lenght == 0)
    return NULL;
  line[line_lenght - 1] = '\0';
  check_endline(data);

  line = realloc(line, line_lenght);
  return line;
}

void free_tokens(char **tokens)
{
  //free(tokens[0]);
  free(tokens);
  tokens = NULL;
}

void display_on_screen(/*recebe os tipos de filtros*/){
  
}

typedef struct colunas
  {
    char RptDt[11];
    char TckrSymb[50];
    char Asst[11];
    char XprtnDt[11];
    char ExrcPric[9];
    char OptnStyle[9];
    long int CvrdQty[16];
    long int TtlBlckPos[13];
    long int UcvrdQty[16];
  } 
  dados;

void save_to_struct(int tam , dados *coluna, char *line, char *line2, char **tokens,
 char **instruments, FILE *arquivo, FILE *arquivo2, unsigned int size, unsigned int size2){

  int fim = 0;
  for(int i = 0; i >= 0; i++){
    line2 = readline(arquivo2);
    if(line2 == NULL){
      break;
    }
    line = readline(arquivo);
    if(line == NULL){
      fim = 1;
    }
    tokens = split(line, ";", &size);
    instruments = split(line2, ";", &size2);
  
    strcpy(coluna[i].TckrSymb, instruments[1]);
    if (strcmp(tokens[1], coluna[i].TckrSymb) == 0 && line2 != NULL)
    {
      strcpy(coluna[i].RptDt, instruments[0]);
      strcpy(coluna[i].Asst, instruments[2]);
      strcpy(coluna[i].XprtnDt, instruments[7]);
      strcpy(coluna[i].ExrcPric, instruments[35]);
      strcpy(coluna[i].OptnStyle, instruments[36]);
      *coluna[i].CvrdQty = atoi(tokens[9]);
      *coluna[i].TtlBlckPos = atoi(tokens[10]);
      *coluna[i].UcvrdQty = atoi(tokens[11]);
      tam++;
    }
    else
    { coluna = realloc(coluna, sizeof(dados) * tam);
      strcpy(coluna[i].RptDt, instruments[0]);
      strcpy(coluna[i].Asst, instruments[2]);
      strcpy(coluna[i].XprtnDt, instruments[7]);
      strcpy(coluna[i].ExrcPric, instruments[35]);
      strcpy(coluna[i].OptnStyle, instruments[36]);
      *coluna[i].CvrdQty = 0;
      *coluna[i].TtlBlckPos = 0;
      *coluna[i].UcvrdQty = 0;
      tam++;
      coluna = realloc(coluna, sizeof(dados) * tam);
      if (fim == 0)
      {
        strcpy(coluna[i+1].TckrSymb, tokens[1]);
        strcpy(coluna[i+1].RptDt, tokens[0]);
        strcpy(coluna[i+1].Asst, tokens[3]);
        strcpy(coluna[i+1].XprtnDt, " \0");
        strcpy(coluna[i+1].ExrcPric, " \0");
        strcpy(coluna[i+1].OptnStyle, " \0");
        *coluna[i+1].CvrdQty = atoi(tokens[9]);
        *coluna[i+1].TtlBlckPos = atoi(tokens[10]);
        *coluna[i+1].UcvrdQty = atoi(tokens[11]);
        i++;
        tam++; 
        coluna = realloc(coluna, sizeof(dados) * tam);
      }
    }

    free_tokens(tokens);
    free_tokens(instruments);
    free(line);
    free(line2);
    line = NULL; 
    line2 = NULL;
  }
}



int main(int argc, char **argv)
{
  FILE *arquivo = fopen(argv[1], "r");
  FILE *arquivo2 = fopen(argv[2], "r");
  char **tokens = NULL, **instruments = NULL;
  unsigned int size, size2;
  char *line, *line2 = NULL; //line = arquivo 1, line2 = arquivo 2
  int tam = 5;
  dados *coluna;
  coluna = (dados *) (malloc(sizeof(dados) * tam));

  printf("=====Dados dos Derivativos=====\n");
  printf("|      Codigo      |    Ativo   |    Vcto    |  Strike  |   Tipo    |  Coberto  |   Travado   |  Descob.  |\n");
  save_to_struct(tam, coluna, line, line2, tokens, instruments, arquivo, arquivo2, size, size2);
  
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

  fclose(arquivo);
  fclose(arquivo2);
}
