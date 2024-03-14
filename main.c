#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void clearInBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void clearConsole() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

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

char checkRU_EN(char (*ENbufer)[256], int countWord, char *worduser) 
{
    char correct;
    correct = '+';
    for (int j = 0; ENbufer[countWord][j] != '\0'; j++)
    {
        if (ENbufer[countWord][j] != worduser[j])
        {
            correct = '-';
            break;
        }
    }
    return correct;
}

int RU_EN(char (*ENbufer)[256] ,char (*RUbufer)[256], char (*writewords)[256], char *worduser, int countWords, int count) 
{   
    char correct;
    int countWord;

    for (int i = 0; i < countWords; ++i)
    {   
        clearConsole();
        countWord = i;
        printf("(%d)Введите: %s\n",i+1, RUbufer[countWord]);
        printf("(Для выхода '~'): ");
        fgets(worduser, 255, stdin);
        if (worduser[0] == '~')
           break;

        worduser[strcspn(worduser, "\n")] = '\0';

        correct = checkRU_EN(ENbufer, countWord, worduser);

        size_t totalLength = strlen(ENbufer[countWord]) + strlen(worduser) + 5;

        if (totalLength < sizeof(writewords[i]))
        {   
            sprintf(writewords[i], "%c %d) %s\n     %s - %s", correct, i+1, RUbufer[countWord], ENbufer[countWord], worduser);
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

char checkEN_RU(char (*RUbufer)[256], int countWord, char *worduser)
{
    char correct;
    correct = '+';
    for (int j = 0; RUbufer[countWord][j] != '\0'; j++)
    {
        if (RUbufer[countWord][j] != worduser[j])
        {
            correct = '-';
            break;
        }
    }
    return correct;
}

int EN_RU(char (*ENbufer)[256] , char (*RUbufer)[256], char (*writewords)[256], char *worduser, int countWords, int count) 
{   
    char correct;
    int countWord;

    for (int i = 0; i < countWords; ++i)
    {
        clearConsole();
        countWord = i;
        printf("(%d)Введите: %s\n",i+1, ENbufer[countWord]);
        printf("(Для выхода '~'): ");
        fgets(worduser, 255, stdin);
        if (worduser[0] == '~')
           break;

        worduser[strcspn(worduser, "\n")] = '\0';
        
        correct = checkRU_EN(RUbufer, countWord, worduser);

        size_t totalLength = strlen(RUbufer[countWord]) + strlen(worduser) + 5;

        if (totalLength < sizeof(writewords[i]))
        {   
            sprintf(writewords[i], "%c %d) %s\n     %s - %s", correct, i+1, ENbufer[countWord],  RUbufer[countWord], worduser);
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

int main(int argc,char *argv[]) //ʕ ᵔᴥᵔ ʔ
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

    int countWords = countENRUwords(eng, rus), count = 0, Right = 0;
    char RUbufer[countWords][256], ENbufer[countWords][256], worduser[256], writewords[countWords][256], workprogramm;

    SaveTxt(eng, rus, RUbufer, ENbufer, countWords);
   
    printf("Слова какого языка хотите переводить (E/R): ");
    workprogramm = getchar();
    clearInBuffer();
    while(workprogramm != '~') 
    {
        clearConsole();
        if (workprogramm == 'R')
        {
            count = RU_EN(ENbufer ,RUbufer, writewords, worduser, countWords, count);
        }
        if(workprogramm == 'E') {

            count = EN_RU(ENbufer, RUbufer, writewords, worduser, countWords, count);
        }

        clearConsole();
        for (int i = 0; i < count; i++)
        {
            printf("%s\n", writewords[i]);
            printf("\n");
        }
        for (int i = 0, j = 0; i < count; i++)
        {
            
            if (writewords[i][j] == '+')
            {
                Right++;
            }
            
        }
        printf("Напис./Всего: %d/%d\n", count, countWords);
        printf("Правильные/Напис: %d/%d\n", Right ,count);
        
        count = 0;

        printf("Поменять язык(E/R)/Закончить(~): ");
        scanf(" %c", &workprogramm);
        clearInBuffer();
    }

    fclose(rus);
    fclose(eng);

    return 0;
}