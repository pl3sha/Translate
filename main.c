#include <stdio.h>

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

    

}