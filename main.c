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

int main(int argc,char *argv[])
{
	FILE *rus = fopen(argv[1],"r");
    FILE *eng = fopen(argv[2],"r");

	if(rus == NULL || eng == NULL){
        printf("Can't open file\n");
        return 0;
    }else if(argc != 3){
        printf("Error argument\n");
        return 0;
    }

    
    int countWords = countENRUwords(eng, rus);
    srand(time(NULL));
    
    char RUbufer[countWords][256], ENbufer[countWords][256], worduser[256], words[countWords][256], workprogramm, simbol;

    fseek(rus, 0, SEEK_SET);
    fseek(eng, 0, SEEK_SET);

    for (int i = 0; i < countWords; i++) {
        fgets(RUbufer[i], sizeof(RUbufer[i]), rus);
        RUbufer[i][strcspn(RUbufer[i], "\n")] = '\0';
    }
    for (int i = 0; i < countWords; i++) {
        fgets(ENbufer[i], sizeof(ENbufer[i]), eng);
        ENbufer[i][strcspn(ENbufer[i], "\n")] = '\0';
    }
    
    int ramdomENRUword;
    int mistakes = 0;

    printf("Слова какого языка хотите переводить (E/R): ");
    scanf("%c", &workprogramm);

    while(workprogramm != '~') 
    {
        if (workprogramm == 'R')
        {
           for (int i = 0; i < countWords; ++i)
            {
                ramdomENRUword = rand() % countWords + 1;
                printf("%s\n", RUbufer[ramdomENRUword]);
                printf("Введите слово(для выхода '~'): ");
                scanf("%s", worduser);

                for (int j = 0; simbol != '\0' ; ++j)
                {
                    words[i][j] = worduser[j];
                    printf("%c",worduser[j]);
                    printf("%c", words[i][j]);
                    simbol = worduser[j];
                }

                if (worduser[0] == '~')
                {
                    break;
                }
            }
        }
        else if(workprogramm == 'E') {
            for (int i = 0; i < countWords; ++i)
            {
                ramdomENRUword = rand() % countWords + 1;
                printf("%s\n", ENbufer[ramdomENRUword]);
                printf("Введите слово(для выхода '~'): ");
                scanf("%s", worduser);
                if (worduser[0] == '~')
                {
                    break;
                }
            }
        }
        printf("Поменять язык(E/R)/Закончить(~)");
        scanf("%c", &workprogramm);
    }

    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            printf("%c", words[i][j]);
        }
            
    }
    printf("chto za huina");
    
    
    fclose(rus);
    fclose(eng);

    return 0;
}