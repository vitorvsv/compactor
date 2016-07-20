#include <stdio.h>
#include <string.h>

/**
 * Função de compressao do arquivo
 *
 * Retorna 0 em caso de sucesso e -1 em caso de falha
 *
 * input  -> Arquivo de entrada
 * output -> Arquivo de saida
 */
int compress(FILE *input, FILE *output)
{
    //Lê a primeira posição do arquivo
    int c = fgetc(input); //Contém a leitura do primeiro byte do arquivo
    int temp = 'N'; //Contém a variável temporaria para comparação
    int cont = 1;

    while (temp != EOF)
    {
        //Primeira vez que entra no laço
        if (temp == 'N')
        {
            temp = c;
        }else{
            //Se a leitura atual do byte do arquivo for igual a anterior
            if (c == temp)
            {
                cont++;
            }else
            {
                int f1 = fputc(cont,output);
                int f2 = fputc(temp,output);
                cont = 1;
                temp = c;

                //Validação para erros
                if (f1 == -1 || f2 == -1)
                {
                    return -1;
                }
            }
        }

        c = fgetc(input);
    }

    return 0;
}

/**
 * Função de descompressao do arquivo
 *
 * Retorna 0 em caso de sucesso e -1 em caso de falha
 *
 * input  -> Arquivo de entrada
 * output -> Arquivo de saida
 */
int decompress(FILE *input, FILE *output)
{
    int c = fgetc(input); //Le o primeiro byte
    int i; //Contem o byte temporario
    //Conta a variavel de quantia do byte
    int qntd;
    //Controla se é um caractere do arquivo ou uma quantia
    //Usa par ou impar para isso
    int cont = 1;

    while (c != EOF)
    {
        //Numero Par
        if ((cont % 2) != 0) {
            qntd = c;
        //Numero impar
        }else{
            for (i = 0; i < qntd; i++)
            {
                fputc(c,output);
            }
        }

        cont++;
        c = fgetc(input);
    }

    return 0;
}

/**
 * Printa a mensagem final com os totais dos arquivos e a taxa de compressão
 */
void printaMsgFinal(int tamI, int tamO,char *arg1, char *arg2,int status)
{
    printf("Arquivo de entrada: %s (%d B) \n",arg1,tamI);
    printf("Arquivo de saida: %s (%d B) \n",arg2,tamO);

    if (tamI > tamO) {
        printf("Taxa de compressao: %f%% \n",(100.0 - ((100.0 * tamO) / tamI)));
    }else{
        printf("Taxa de compressao %f%% (arquivo de saida ficou maior) \n", (0 - ((100.0 * (tamO -tamI)) / tamI)));
    }

    if (status == -1)
    {
        printf("Houve um erro com o arquivo.");
    }else{
        printf("O arquivo foi gravado com sucesso.");
    };
}

/**
 * Conta o tamanho do arquivo passado por parâmetro
 *
 */
int getTamanhoArquivo(FILE *f)
{
    int tam = 0;
    if (f != NULL)
    {
        //Movimenta o cursor para o fim do arquivo
        fseek(f, 0, SEEK_END);

        //Pega o tamanho em bytes do ultimo caractere ('onde esta o cursor')
        tam = ftell(f);

        //Movimenta o cursor para o inicio do arquivo('para continuar a leitura')
        fseek(f, 0, SEEK_SET);
    }

    return tam;
}


int main(int argc, char *args[])
{
    //Valida a quantidade de argumentos passados por parametro
    if (argc != 4 || (strcmp(args[1],"-c") != 0 && strcmp(args[1],"-x") != 0) )
    {
        printf("Você deve informar a opção desejada(-c,-x), o nome do arquivo de entrada/leitura, e -o nome do arquivo de saida/escrita");
        return -1;
    }

    //Abre o arquivo de leitura
    FILE *fileI = fopen(args[2],"r");

    //Testando se o arquivo foi aberto com sucesso
    if (fileI == NULL)
    {
        printf("Nao foi possivel ler o arquivo de entrada");
        return -1;
    }

    //Armazena o tamanho do arquivo de leitura
    int tamFileI = getTamanhoArquivo(fileI);

    //Concatena a extensão do arquivo de saida caso for uma compactacao
    if (strcmp(args[1],"-c") == 0) {
        strcat(args[3],".vsv");
    }

    //Abre o arquivo de destino/escrita
    FILE *fileO = fopen(args[3],"w");

    //Testa o arquivo de destino/escrita
    if (fileO == NULL)
    {
        printf("Nao foi possivel criar o arquivo de destino");
        return -1;
    }

    //Guarda o status da compressao/descompressao
    int status;

    //Testa a opção escolhida pelo usuário
    if (strcmp(args[1],"-c") == 0)
    {
        //Comprimindo o arquivo
        status = compress(fileI,fileO);
    }else
    {
        //Descomprimindo o arquivo
        status = decompress(fileI,fileO);
    }

    //Pega o tamanho do arquivo de saida
    int tamFileO = getTamanhoArquivo(fileO);

    //Printa as mensagens finais
    printaMsgFinal(tamFileI,tamFileO,args[2],args[3],status);

    //Fechando os arquivos
    fclose(fileI);
    fclose(fileO);

    return status;
}