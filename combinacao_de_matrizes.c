#include <cs50.h>
#include <stdio.h>

void obter_matriz(int altura, int largura, float m[altura][largura] ){
    for (int i = 0; i < altura; i++){
        for (int j = 0; j < largura; j++)
        {
            m[i][j] = get_float("Digite um número inteiro para m[%i] [%i]: ", i, j);
        }
    }
}

void imprimir_matriz(int altura, int largura, float m[altura][largura] ){
    for (int i = 0; i < altura; i++)
    {
        for (int j = 0; j < largura; j++)
        {
            printf(" %.1f ", m[i][j]);
        }
        printf("\n");
    }
}

void processar_matriz(int altura, int largura, float a[altura][largura], float b[altura][largura], float c[altura][largura], double t){
    for (int i = 0; i < altura; i++){
        for (int j = 0; j < largura; j++)
        {
            c[i][j] = (((1.0 - t) * (a[i][j])) + (t * (b[i][j])));
        }
    }
}

int main(){
    int largura, altura, numero_matriz = 1; double t = 0.0;

    altura = get_int("Digite a altura da matriz: ");
    largura = get_int("Digite a largura da matriz: ");

    float matriz1[altura][largura], matriz2[altura][largura], matriz3[altura][largura];

    printf("\nDados da matriz 1\n");
    obter_matriz(altura, largura, matriz1);

    printf("\nDados da matriz 2\n");
    obter_matriz(altura, largura, matriz2);

    printf("\nA matriz 1 é:\n");
    imprimir_matriz(altura, largura, matriz1);
    printf("A matriz 2 é:\n");
    imprimir_matriz(altura, largura, matriz2);
    printf("\n");

    while (t < 1.0) //imprimir as 11 matrizes com cada valor de t
    {   
        printf("Matriz n° %i --> Para t = %.1f\n", numero_matriz,t);
        processar_matriz(altura, largura, matriz1, matriz2, matriz3, t);
        imprimir_matriz(altura, largura, matriz3);
        printf("\n");
        numero_matriz++;
        t = t + 0.1;
    }
    return 0;
}
