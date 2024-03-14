#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void Clear_Buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void Clear_Console() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int Counting_words_in_files(FILE* eng, FILE* rus) 
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

void Saving_words_into_arrays(FILE* eng, FILE* rus, char (*RUwordsArr)[256], char (*ENwordsArr)[256], int NumOfWords)
{
    fseek(rus, 0, SEEK_SET);
    fseek(eng, 0, SEEK_SET);

    for (int i = 0; i < NumOfWords; i++) {
        fgets(RUwordsArr[i], sizeof(RUwordsArr[i]), rus);
        int len = strlen(RUwordsArr[i]);
        while (len > 0 && (RUwordsArr[i][len - 1] == ' ' || RUwordsArr[i][len - 1] == '\t' || RUwordsArr[i][len - 1] == '\n'))
        {
            RUwordsArr[i][len - 1] = '\0';
            len--;
        }
    }

    for (int i = 0; i < NumOfWords; i++) {
        fgets(ENwordsArr[i], sizeof(ENwordsArr[i]), eng);
        int len2 = strlen(ENwordsArr[i]);
        while (len2 > 0 && (ENwordsArr[i][len2 - 1] == ' ' || ENwordsArr[i][len2 - 1] == '\t' || ENwordsArr[i][len2 - 1] == '\n'))
        {
            ENwordsArr[i][len2 - 1] = '\0';
            len2--;
        }
    }
}

char checkRU_EN(char (*ENwordsArr)[256], int countWord, char *UserWords) 
{
    char correct;
    correct = '+';
    for (int j = 0; ENwordsArr[countWord][j] != '\0'; j++)
    {
        if (ENwordsArr[countWord][j] != UserWords[j])
        {
            correct = '-';
            break;
        }
    }
    return correct;
}

int RU_EN(char (*ENwordsArr)[256] ,char (*RUwordsArr)[256], char (*WritWords)[256], char *UserWords, int NumOfWords, int count) 
{   
    char correct;
    int countWord;

    for (int i = 0; i < NumOfWords; ++i)
    {   
        Clear_Console();
        countWord = i;
        printf("(%d)Введите: %s\n",i+1, RUwordsArr[countWord]);
        printf("(Для выхода '~'): ");
        fgets(UserWords, 255, stdin);
        if (UserWords[0] == '~')
           break;

        UserWords[strcspn(UserWords, "\n")] = '\0';

        correct = checkRU_EN(ENwordsArr, countWord, UserWords);

        size_t totalLength = strlen(ENwordsArr[countWord]) + strlen(UserWords) + 5;

        if (totalLength < sizeof(WritWords[i]))
        {   
            sprintf(WritWords[i], "%c %d) %s\n     %s - %s", correct, i+1, RUwordsArr[countWord], ENwordsArr[countWord], UserWords);
        }
        else
        {
            printf("Недостаточно место в WritWords[i] = %ld", sizeof(WritWords[i]));
            break;
        }
        count++;
    }
    return count;
}

char checkEN_RU(char (*RUwordsArr)[256], int countWord, char *UserWords)
{
    char correct;
    correct = '+';
    for (int j = 0; RUwordsArr[countWord][j] != '\0'; j++)
    {
        if (RUwordsArr[countWord][j] != UserWords[j])
        {
            correct = '-';
            break;
        }
    }
    return correct;
}

int EN_RU(char (*ENwordsArr)[256] , char (*RUwordsArr)[256], char (*WritWords)[256], char *UserWords, int NumOfWords, int count) 
{   
    char correct;
    int countWord;

    for (int i = 0; i < NumOfWords; ++i)
    {
        Clear_Console();
        countWord = i;
        printf("(%d)Введите: %s\n",i+1, ENwordsArr[countWord]);
        printf("(Для выхода '~'): ");
        fgets(UserWords, 255, stdin);
        if (UserWords[0] == '~')
           break;

        UserWords[strcspn(UserWords, "\n")] = '\0';
        
        correct = checkRU_EN(RUwordsArr, countWord, UserWords);

        size_t totalLength = strlen(RUwordsArr[countWord]) + strlen(UserWords) + 5;

        if (totalLength < sizeof(WritWords[i]))
        {   
            sprintf(WritWords[i], "%c %d) %s\n     %s - %s", correct, i+1, ENwordsArr[countWord],  RUwordsArr[countWord], UserWords);
        }
        else
        {
            printf("Недостаточно место в WritWords[i] = %ld", sizeof(WritWords[i]));
            break;
        }
        count++;
    }
    return count;
}

/*
void sortWritWords()
{

}
*/
int main(int argc,char *argv[]) /*ʕ ᵔᴥᵔ ʔ*/
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

    int NumOfWords = Counting_words_in_files(eng, rus), count = 0, Right = 0, mistake;
    char RUwordsArr[NumOfWords][256], ENwordsArr[NumOfWords][256], UserWords[256], WritWords[NumOfWords][256], workprogramm;

    Saving_words_into_arrays(eng, rus, RUwordsArr, ENwordsArr, NumOfWords);
   
    printf("Слова какого языка хотите переводить (E/R): ");
    workprogramm = getchar();
    Clear_Buffer();

    while(workprogramm != '~') 
    {
        Clear_Console();
        if (workprogramm == 'R')
        {
            count = RU_EN(ENwordsArr ,RUwordsArr, WritWords, UserWords, NumOfWords, count);
        }
        if(workprogramm == 'E') {

            count = EN_RU(ENwordsArr, RUwordsArr, WritWords, UserWords, NumOfWords, count);
        }

        Clear_Console();
        for (int i = 0; i < count; i++)
        {
            printf("%s\n", WritWords[i]);
            printf("\n");
        }
        for (int i = 0, j = 0; i < count; i++)
        {
            
            if (WritWords[i][j] == '+')
            {
                Right++;
            }
        }
        printf("Напис./Всего: %d/%d\n", count, NumOfWords);
        printf("Правильные/Напис: %d/%d\n", Right ,count);
        
        count = 0;

        printf("Сделать работу над ошибками(@)/Закончить(~): ");
        scanf(" %c", &workprogramm);
        Clear_Buffer();
        /*
        if (workprogramm == '@')
        {
            sortWritWords();
            work on mistakes();
        }
        */
    }

    fclose(rus);
    fclose(eng);

    return 0;
}