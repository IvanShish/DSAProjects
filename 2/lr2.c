#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Node{                          //объявление структуры
    char c;               //символ
    struct Node *next;             //указатель на следующий элемент в списке                  
    struct Node *child;                //указатель на дочерний элемент в списке
    int isAtom;                     //проверка элемента на атом
}Node;

Node *func(char *s, int count, int *flag);    //главная функция
Node *atom();                          //функция создания атома
void printAndCheckTree(Node *tree, char elem, int *flag, int count);    //функция вывода дерева и нахождения в нем заданного элемента
void indent(int count);           //функция для печати отступов

int curr = -1;

int main(){

    char *file = NULL;           //блок переменных для считывания файлов
    char **files = NULL;
    char string[255];
    FILE *f;
    int textOrType = 0;
    int i = 1;

    char symbol;      //введенный атом, который нужно найти
    int flag = 0; 
    Node *list = NULL;


    printf("Как будет введена строка?\n1. Из файла\n2. Вручную\n");
    scanf("%d", &textOrType);
    getchar();
    printf("\n");

    if (textOrType != 1 && textOrType != 2){
        printf("Нужно ввести 1 или 2!\n");
        return 0;
    }

    else if (textOrType == 2){
        printf("Введите строку:\n");
        fgets(string, 255, stdin);
    }

    else if (textOrType == 1){
        printf("Выберите файл:\n");
        DIR *dir = opendir("directory");
        struct dirent *ent;
        if (dir){
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
    if (f){
        getchar();
        fgets(string, 255, f);
        fclose(f);
        file = NULL;
        free(file);
    }

    printf("Введенная строка: %s\nВведите атом, который нужно найти: ", string);
    scanf("%c", &symbol);
    if (symbol == '(' || symbol == ')'){
        printf("Атом введен неверно!\n");
        return 0;
    }

    printf("\n");

    printf("Построение иерархического списка...\n");
    list = func(string, 0, &flag);
    if (string[curr + 1] != '\n'){
        free(list);
        printf("Введены лишние символы!\n");
        return 0;
    }
    if(flag > 0){
        free(list);
        printf("Отсутсвует (\n");
        return 0;
    }
    else if(flag < 0){
        free(list);
        printf("Отсутсвует )\n");
        return 0;
    }

    flag = 0;
    printf("\nВывод элементов списка и поиск в нем атома '%c':\n", symbol);
    printAndCheckTree(list, symbol, &flag, 0);
    printf("\n");
    if (flag == 1) printf("Запрашиваемый элемент был найден!\n");
    else printf("Запрашиваемый элемент не был найден!\n");

    free(list);
    return 0;
}

Node *func(char *s, int count, int *flag){
    Node *elem = atom();
    curr++;

    if(*flag == 0 && isalpha(s[curr])){
        printf("Добавление в список %c\n", s[curr]);
        elem->c = s[curr];
        elem->isAtom = 1;
        return elem;
    }
    
    if(s[curr] == '('){                                           // при встрече ( начинает записывать в branch
        indent(count);
        elem->isAtom = 0;
        printf("Добавление ответвления\n");
        (*flag)++;
        elem->child = func(s, count + 1, flag);
        
        if(elem->child == NULL){
            return NULL;
        }
        
        if(*flag == 0){
            return elem;
        }
        
        elem->next = func(s, count, flag);
        
        if(elem->next == NULL){
            return NULL;
        }
        /*else if(elem->next->isAtom == 0){
            free(elem->next);
            elem->next = NULL;
        }*/
        
        return elem;
    }
    
    else if(isalpha(s[curr])){                                        // при встрече атома продолжает записывать в next
        
        if(*flag == 0){
            printf("Отсутствует (\n");
            return NULL;
        }
        
        indent(count);
        printf("Добавление в список %c\n", s[curr]);
        elem->c = s[curr];
        elem->isAtom = 1;
        elem->next = func(s, count, flag);
        
        if(elem->next == NULL){
            return 0;
        }
        return elem;
    }
    else if(s[curr] == ')'){                                     //  при встрече ) завершает запись next
        (*flag)--;
        elem->isAtom = 0;
        indent(count);
        printf("Завершение добавления ответвления\n");
        return elem;
    }
    
    else if(!isalpha(s[curr])){                                   // при встрече неверных символов завершает работу
        printf("Неверный сивол\n");
    }
    
    return NULL;
}

Node *atom(){
    Node *elem = malloc(sizeof(Node));
    elem->child = NULL;
    elem->next = NULL;
    return elem;
}

void printAndCheckTree(Node *tree, char elem, int *flag, int count){
    do{
        if(tree->isAtom){
            indent(count);
            printf("Проверка атома %c - ", tree->c);
            if (tree->c == elem){
                printf("запрашиваемый атом найден!!!\n");
                *flag = 1;
            }
            else printf("запрашиваемый атом не найден\n");
        }
        if(tree->child != NULL){
            indent(count);
            printf("Вызывается рекурсивная функция для дочернего списка\n");
            printAndCheckTree(tree->child, elem, flag, count + 1);
        }
        tree = tree->next;
    }while (tree != NULL);
}

void indent(int count){
    for(int i = 0; i < count; i++){
        printf("  ");
    }
}