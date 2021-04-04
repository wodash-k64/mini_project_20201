#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#include "btree.h"

#define BUFF_SIZE 100

int size;
BTA *dict;

int get_int();
void get_line(char *string);
void print(char *start, char *end);
char **suggest_completion(const char *, int, int);
char *suggest_generator(const char *, int);

int main()
{
    rl_attempted_completion_function = suggest_completion;

    int n;
    char value[BUFF_SIZE];
    char key[BUFF_SIZE];
    char *buff = NULL;

    btinit();
    dict = btopn("dict_db", 0, 0);
    do
    {
        system("clear");
        printf("1.Add\n2.Search\n3.Delete\n4.Print list\n5.Exit\nYour choice:");
        n = get_int();
        switch (n)
        {
        case 1:
            printf("Key: ");
            get_line(key);
            printf("Value: ");
            get_line(value);
            btins(dict, strdup(key), strdup(value), BUFF_SIZE);
            break;
        case 2:
            buff = readline("Key: ");
            if (buff[strlen(buff) - 1] == ' ')
            {
                buff[strlen(buff) - 1] = '\0';
            }
            if (btsel(dict, buff, value, BUFF_SIZE, &size))
                printf("Not found value for %s!\n", buff);
            else
                printf("Value of %s is %s\n", buff, value);
            free(buff);
            break;
        case 3:
            printf("Key: ");
            get_line(key);
            btdel(dict, key);
            break;
        case 4:
            printf("Start: ");
            get_line(key);
            printf("End: ");
            get_line(value);
            print(key, value);
            break;
        case 5:
            btcls(dict);
            break;
        default:
            break;
        }
        if (n == 4 || n == 2)
        {
            printf("Press enter to continue...");
            getchar();
        }
    } while (n != 5);

    return 0;
}

char **suggest_completion(const char *text, int start, int end)
{
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, suggest_generator);
}

char *suggest_generator(const char *text, int state)
{
    static char word[100][100] = {0}, ntext[100] = {0};
    static int i = 0;
    char *name;
    char value[BUFF_SIZE], key[BUFF_SIZE];

    if (!state)
    {
        memset(ntext, 0, 100);
        memset(word, 0, 100 * 100);
        strcpy(key, text);
        strcpy(ntext, text);
        ntext[strlen(ntext)] = 'z';
        btsel(dict, key, value, BUFF_SIZE, &size);
        btselp(dict, key, value, BUFF_SIZE, &size);
        while (btseln(dict, key, value, BUFF_SIZE, &size) == 0 && i < 90)
        {
            if (strcmp(key, ntext) >= 0)
            {
                break;
            }
            strcpy(word[i], key);
            i++;
        }
        strcpy(word[i], "");
        i = 0;
    }

    while (1)
    {
        name = word[i++];
        if (strcmp(word[i], "") == 0)
        {
            break;
        }
        return strdup(name);
    }

    return NULL;
}

void print(char *start, char *end)
{
    int index = 0;
    int size;
    char value[BUFF_SIZE], key[BUFF_SIZE];

    btsel(dict, start, value, BUFF_SIZE, &size);
    printf("------------------------------------------------------\n");
    printf("|%-10s|%-20s|%-20s|\n------------------------------------------------------\n", "INDEX", "KEY",
           "VALUE");
    while (btseln(dict, key, value, BUFF_SIZE, &size) == 0)
    {
        if (1 || index % 1000 == 1)
        {
            printf("|%-10d|%-20s|%-20s|\n------------------------------------------------------\n", index,
                   key, value);
        }
        if (strcmp(key, end) >= 0)
        {
            break;
        }
        index++;
    }
}
int get_int()
{
    char string[BUFF_SIZE];

    do
    {
        fgets(string, BUFF_SIZE, stdin);
    } while (string[0] == '\n');
    if (string[strlen(string) - 1] == '\n')
    {
        string[strlen(string) - 1] = '\0';
    }

    return (int)strtol(string, NULL, 10);
}
void get_line(char *string)
{
    do
    {
        fgets(string, BUFF_SIZE, stdin);
    } while (string[0] == '\n');
    if (string[strlen(string) - 1] == '\n')
    {
        string[strlen(string) - 1] = '\0';
    }
}