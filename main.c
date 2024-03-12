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
    int ramdomENRUword;
    char RUbufer[countWords][256],  ENbufer[countWords][256], workprogramm;

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

    for (int i = 0; i < countWords; i++)
    {
        printf("%s\n", RUbufer[i]);
    }
        for (int i = 0; i < countWords; i++)
    {
        printf("%s\n", ENbufer[i]);
    }
    

    

    /*while (workprogramm != 'q')
    {


        printf("For exit 'q': ");
        scanf("%s", &workprogramm);
    }
    */
    
    fclose(rus);
    fclose(eng);

    return 0;
}