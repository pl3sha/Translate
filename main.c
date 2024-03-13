#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int countENRUwords(FILE* eng, FILE* rus) 
{
    char ch;
    int RUcount = 0, ENcount = 0;;

    while ((ch = fgetc(eng)) != EOF)
    {
        if(ch == '\n')
            ENcount++;
    }

    while ((ch = fgetc(rus)) != EOF)
    {
        if(ch == '\n')
            RUcount++; 
    }
    
    if (RUcount > ENcount)
    {
        printf("Lines in File whith English words less than File with Russian words\n");
        printf("Either you are missing a word in the file with English words\n");
        printf("Or there is no empty line after the last word this is necessary for the word count to work correctly)...\n");
    }
    else if (ENcount > RUcount)
    {
         printf("Lines in File whith Russian words less than File with English words\n");
         printf("Either you are missing a word in the file with Russian words\n");
         printf("Or there is no empty line after the last word this is necessary for the word count to work correctly)...\n");
    }

    return RUcount;
}

void SaveTxt(FILE* eng, FILE* rus, char (*RUbufer)[256], char (*ENbufer)[256], int countWords)
{
    fseek(rus, 0, SEEK_SET);
    fseek(eng, 0, SEEK_SET);

    for (int i = 0; i < countWords; i++) {
        fgets(RUbufer[i], sizeof(RUbufer[i]), rus);
        int len = strlen(RUbufer[i]);
        while (len > 0 && (RUbufer[i][len - 1] == ' ' || RUbufer[i][len - 1] == '\t' || RUbufer[i][len - 1] == '\n'))
        {
            RUbufer[i][len - 1] = '\0';
            len--;
        }
        
    }
    for (int i = 0; i < countWords; i++) {
        fgets(ENbufer[i], sizeof(ENbufer[i]), eng);
        int len2 = strlen(ENbufer[i]);
        while (len2 > 0 && (ENbufer[i][len2 - 1] == ' ' || ENbufer[i][len2 - 1] == '\t' || ENbufer[i][len2 - 1] == '\n'))
        {
            ENbufer[i][len2 - 1] = '\0';
            len2--;
        }
    }
}

void clearInBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int RU_EN(char (*ENbufer)[256] ,char (*RUbufer)[256], char (*writewords)[256], char *worduser, int countWords, int count) 
{
    srand(time(NULL));
    int ramdomENRUword;

    for (int i = 0; i <= countWords; ++i)
    {
        ramdomENRUword = rand() % countWords;
        printf("Введите: %s\n", RUbufer[ramdomENRUword]);
        printf("(Для выхода '~'): ");
        fgets(worduser, 255, stdin);
        if (worduser[0] == '~')
        {
           break;
        }
        worduser[strcspn(worduser, "\n")] = '\0';

        if (strlen(writewords[i]) + strlen(ENbufer[ramdomENRUword]) + strlen(worduser) + 3 < sizeof(writewords[i]))
        {   
            strcpy(writewords[i], ENbufer[ramdomENRUword]);
            strcat(writewords[i], " - ");
            strcat(writewords[i], worduser);
        }
        else
        {
            printf("Недостаточно место в writewords[i] = %ld", sizeof(writewords[i]));
            break;
        }
        
        count++;
    }

    return count;
}

int EN_RU(char (*ENbufer)[256] , char (*RUbufer)[256], char (*writewords)[256], char *worduser, int countWords, int count) 
{   
    srand(time(NULL));
    int ramdomENRUword;

    for (int i = 0; i <= countWords; ++i)
    {
        ramdomENRUword = rand() % countWords;
        printf("Введите: %s\n", ENbufer[ramdomENRUword]);
        printf("(Для выхода '~'): ");
        fgets(worduser, 255, stdin);
        if (worduser[0] == '~')
        {
           break;
        }
        worduser[strcspn(worduser, "\n")] = '\0';
        
        if (strlen(writewords[i]) + strlen(RUbufer[ramdomENRUword]) + strlen(worduser) + 3 < sizeof(writewords[i]))
        {   
            strcpy(writewords[i], RUbufer[ramdomENRUword]);
            strcat(writewords[i], " - ");
            strcat(writewords[i], worduser);
        }
        else
        {
            printf("Недостаточно место в writewords[i] = %ld", sizeof(writewords[i]));
            break;
        }
        count++;
    }

    return count;
}

int main(int argc,char *argv[])
{
	FILE *rus = fopen(argv[1],"r");
    FILE *eng = fopen(argv[2],"r");

    if(argc != 3){
        printf("Error argument\n");
        return 0;
    }
	if(rus == NULL || eng == NULL){
        printf("Can't open file\n");
        return 0;
    }
    
    int countWords = countENRUwords(eng, rus), count = 0;
    char RUbufer[countWords][256], ENbufer[countWords][256], worduser[256], writewords[countWords][256], workprogramm, Corect;

    SaveTxt(eng, rus, RUbufer, ENbufer, countWords);
   
    printf("Слова какого языка хотите переводить (E/R): ");
    workprogramm = getchar();
    clearInBuffer();
    while(workprogramm != '~') 
    {
        if (workprogramm == 'R')
        {
            count = RU_EN(ENbufer ,RUbufer, writewords, worduser, countWords, count);
        }
        if(workprogramm == 'E') {

            count = EN_RU(ENbufer, RUbufer, writewords, worduser, countWords, count);
        }
        for (int i = 0; i < count; i++)
        {
            printf("%d) %s\n", i+1, writewords[i]);
        }
        count = 0;

        printf("Поменять язык(E/R)/Закончить(~): ");
        workprogramm = getchar();
        clearInBuffer();
    }

    fclose(rus);
    fclose(eng);

    return 0;
}