#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
//A(A;A(A;A)(A);A);A
int semicolon(char *symbol, int check, FILE *f);

void printDepth(int indent){
    for (int i = 0; i < indent * 2; i++)
        printf(" ");
}

char reading(FILE *f){ 
    if(f == NULL){
        return getchar();
    }
    else{
        return fgetc(f);
    }
}

int parenthesis(char *symbol, int check, FILE *f){

    printDepth(check);
    printf("Вызывается функция parenthesis.\n");
    printDepth(check);
    if( (*symbol = reading(f)) != 'A'){
        printf("'%c' - неверно! Ожидалось 'A'\n", *symbol);
        printDepth(check);
        printf("Заканчивается функция parenthesis\n");
        return 0;
    }

    printf("'%c' - верно\n", *symbol);
    *symbol = reading(f);

    if(*symbol == ')'){
        //check--;
        printDepth(check);
        printf("'%c' - верно\n", *symbol);
        if((*symbol = reading(f)) == '\n'){
            printDepth(check);
            printf("Заканчивается функция parenthesis\n");
            return 1;
        }
        else if(*symbol == ')'){
            printDepth(check);
            printf("Заканчивается функция parenthesis\n");
            return 2;
        }
        else if(*symbol == ';'){
            printDepth(check);
            printf("'%c' - верно\n", *symbol);
            printDepth(check);
            printf("Заканчивается функция parenthesis\n");
            return semicolon(symbol, check+1, f);
        }
        else if(*symbol == '('){
            printDepth(check);
            printf("'%c' - верно\n", *symbol);
            printDepth(check);
            printf("Заканчивается функция parenthesis\n");
            return parenthesis(symbol, check+1, f);
        }
    }

    else if (*symbol == ';'){
        printDepth(check);
        printf("'%c' - верно\n", *symbol);
        if (semicolon(symbol, check+1, f) == 2){
            if (*symbol == ')'){
                printDepth(check);
                printf("'%c' - верно\n", *symbol);
                if ((*symbol = reading(f)) == '\n'){
                    printDepth(check);
                    printf("Заканчивается функция parenthesis\n");
                    return 1;
                }
                else if (*symbol == ')'){
                    printDepth(check);
                    printf("Заканчивается функция parenthesis\n");
                    return 2;
                }
                else if (*symbol == ';'){
                    printDepth(check);
                    printf("'%c' - верно\n", *symbol);
                    printDepth(check);
                    printf("Заканчивается функция parenthesis\n");
                    return semicolon(symbol, check+1, f);
                }
                else if (*symbol == '('){
                    printDepth(check);
                    printf("'%c' - верно\n", *symbol);
                    printDepth(check);
                    printf("Заканчивается функция parenthesis\n");
                    return parenthesis(symbol, check+1, f);
                }
            }
            printDepth(check);
            printf("Заканчивается функция parenthesis\n");
            return 0;
        }
    }

    else if(*symbol == '('){
        printDepth(check);
        printf("'%c' - верно\n", *symbol);
        if(parenthesis(symbol, check+1, f) == 2){
            if(*symbol == ')'){
                printDepth(check);
                printf("'%c' - верно\n", *symbol);
                if((*symbol = reading(f)) == '\n'){
                    printDepth(check);
                    printf("Заканчивается функция parenthesis\n");
                    return 1;
                }
                else if (*symbol == ')'){
                    printDepth(check);
                    printf("Заканчивается функция parenthesis\n");
                    return 2;
                }

            }

        }
    }

    return 0;
}

int semicolon(char *symbol, int check, FILE *f){

    printDepth(check);
    printf("Вызывается функция semicolon.\n");
    printDepth(check);
    if( (*symbol = reading(f)) != 'A'){
        printf("'%c' - неверно! Ожидалось 'A'\n", *symbol);
        printDepth(check);
        printf("Заканчивается функция semicolon\n");
        return 0;
    }

    printf("'%c' - верно\n", *symbol);
    *symbol = reading(f);

    if(*symbol == ')'){
        printDepth(check);
        printf("Заканчивается функция semicolon\n");
        return 2;
    }
    else if (*symbol == ';'){
        printDepth(check);
        printf("'%c' - верно\n", *symbol);
        printDepth(check);
        printf("Заканчивается функция semicolon\n");
        return semicolon(symbol, check+1, f);
    }
    else if (*symbol =='('){
        printDepth(check);
        printf("'%c' - верно\n", *symbol);
        printDepth(check);
        printf("Заканчивается функция semicolon\n");
        return parenthesis(symbol, check+1, f);
    }
    else if( *symbol == '\n'){
        printDepth(check);
        printf("Заканчивается функция semicolon\n");
        return 1;
    }
    printDepth(check);
    printf("Заканчивается функция semicolon\n");
    return 0;

}


int main(){

    char symbol;
    char *file = NULL;
    char **files = NULL;
    FILE *f;
    int check = 0;
    int textOrType = 0;
    int i = 1;

    printf("Как будет введена строка?\n1. Из файла\n2. Вручную\n");
    scanf("%d", &textOrType);
    printf("\n");
    if (textOrType != 1 && textOrType != 2){
        printf("Нужно ввести 1 или 2!\n");
        return 0;
    }
    else if (textOrType == 2)
        printf("Введите строку:\n");
    else if (textOrType == 1){
        printf("Выберите файл:\n");
        DIR *dir = opendir("directory");
        struct dirent *ent;
        if (dir){                                            //Выводит доступные файлы
            for (i = 1; (ent = readdir (dir)) != NULL; ){
                if (!strchr(ent->d_name, '.')){
                    files = realloc(files, i * sizeof(char*) + 1);
                    files[i-1] = malloc(strlen(ent->d_name) * sizeof(char) + 1);
                    strcpy(files[i-1], ent->d_name);
                    printf("%d. %s\n", i, files[i-1]);
                    i++;
                }
            }
        }
	    textOrType = -1;
        scanf("%d", &textOrType);
        if (textOrType > i-1 || textOrType < 1){
            printf("Неправильно выбран файл!\n");
            return 0;
        }

        file = realloc(file, (strlen(files[textOrType-1]) + 4 + 10) * sizeof(char) + 1);
        strcpy(file, "directory/");
        strcat(file, files[textOrType-1]);
        file[strlen(file)] = '\0';
        printf("Вы выбрали файл %s\n", files[textOrType-1]);

        for (int j = 0; j < i; j++){
            files[j] = NULL;
            free(files[j]);
        }
        files = NULL;
        free(files);
    }

    f = fopen(file, "r");
    getchar();
    symbol = reading(f);

    if( symbol !=  'A'){                //проверка 1-го считанного символа
        printf("Нет, это не СКОБКА (ожидалось, что строка начнется с 'A')\n");
        return 0;
    }

    printf("'%c' - верно\n", symbol);
    symbol = reading(f);

    if( symbol == '('){
        printf("'%c' - верно\n", symbol);
        if(parenthesis(&symbol, check+1, f) == 1){
            printf("\nДа, это СКОБКА\n");
        }
        else{
            printf("\nНет, это не СКОБКА\n");
        }
    }
    else if( symbol == ';'){
        printf("'%c' - верно\n", symbol);
        if(semicolon(&symbol, check+1, f) == 1){
            printf("\nДа, это СКОБКА\n");
        }
        else{
            printf("\nНет, это не СКОБКА\n");
        }

    }
    else if( symbol == '\n'){
        printf("\nДа, это СКОБКА\n");
    }
    else{
        printf("\nНет, это не СКОБКА (ожидалось, что вторым введеным символом будет ';' или '(')\n");
    }

    file = NULL;
    free(file);
    return 0;
}
