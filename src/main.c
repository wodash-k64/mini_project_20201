#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#include "btree.h"

#define BUFF_SIZE 50

int size;
BTA *dict;

char **suggest_completion(const char *, int, int);

char *suggest_generator(const char *, int);

int main()
{
    rl_attempted_completion_function = suggest_completion;

    int n = 0;
    char *buff_1 = malloc(BUFF_SIZE);
    char *buff_2 = malloc(BUFF_SIZE);
    char *buff_3 = malloc(BUFF_SIZE);

    btinit();
    dict = btopn("dict_db", 0, 0);

    // Nếu chưa có dict_db, dùng dữ liệu từ data.txt để tạo ra
    if (!dict)
    {
        dict = btcrt("dict_db", 0, 0);
        FILE *f_data = fopen("data.txt", "r");
        if (f_data == NULL)
        {
            printf("Can't open file data.txt");
            return 0;
        }
        else
        {
            while (!feof(f_data))
            {
                memset(buff_1, 0, BUFF_SIZE);
                memset(buff_2, 0, BUFF_SIZE);
                fscanf(f_data, "%s", buff_1);
                fgetc(f_data);
                for (int i = 0; i < strlen(buff_1); ++i)
                {
                    if (buff_1[i] == 'z')
                    {
                        buff_2[i] = 'a';
                    }
                    else
                    {
                        buff_2[i] = (char)(buff_1[i] + 1);
                    }
                }
                btins(dict, buff_1, buff_2, BUFF_SIZE);
            }
        }
    }

    do
    {
        system("clear");
        printf("\n1.Add\n2.Search\n3.Delete\n4.Print\n5.Exit\nEnter your choice: ");
        __fpurge(stdin);
        scanf("%d", &n);
        switch (n)
        {
        case 1:
            // memset(buff_1, 0, BUFF_SIZE);
            // memset(buff_2, 0, BUFF_SIZE);
            printf("Key: ");
            __fpurge(stdin);
            fgets(buff_1, BUFF_SIZE, stdin);
            if (buff_1[strlen(buff_1) - 1] == '\n')
            {
                buff_1[strlen(buff_1) - 1] = '\0';
            }
            printf("Value: ");
            __fpurge(stdin);
            fgets(buff_2, BUFF_SIZE, stdin);
            if (buff_2[strlen(buff_1) - 1] == '\n')
            {
                buff_2[strlen(buff_1) - 1] = '\0';
            }
            btins(dict, buff_1, buff_2, BUFF_SIZE);
            break;
        case 2:
            // memset(buff_1, 0, BUFF_SIZE);
            // memset(buff_2, 0, BUFF_SIZE);
            buff_1 = readline("Search: ");
            if (buff_1[strlen(buff_1) - 1] == ' ')
            {
                buff_1[strlen(buff_1) - 1] = '\0';
            }
            btsel(dict, buff_1, buff_2, BUFF_SIZE, &size);
            printf("Value of %s is %s\n", buff_1, buff_2);
            break;
        case 3:
            break;
        case 4:
            // memset(buff_1, 0, BUFF_SIZE);
            // memset(buff_2, 0, BUFF_SIZE);
            // memset(buff_3, 0, BUFF_SIZE);
            printf("Start with: ");
            __fpurge(stdin);
            fgets(buff_1, BUFF_SIZE, stdin);
            if (buff_1[strlen(buff_1) - 1] == '\n')
            {
                buff_1[strlen(buff_1) - 1] = '\0';
            }
            int len = strlen(buff_1);
            strcpy(buff_3, buff_1);
            btsel(dict, buff_1, buff_2, BUFF_SIZE, &size);
            while (btseln(dict, buff_1, buff_2, BUFF_SIZE, &size) == 0)
            {
                if (strncmp(buff_3, buff_1, len) == 0)
                {
                    printf("|%20s|%20s|\n-------------------------------------------\n", buff_1, buff_2);
                }
                else
                {
                    break;
                }
            }
            break;
        default:
            break;
        }

        if (n == 2 || n == 4)
        {
            __fpurge(stdin);
            printf("Press enter to continue!!!");
            getchar();
        }

    } while (n != 5);
    free(buff_1);
    free(buff_2);
    free(buff_3);
    return 0;
}

char **suggest_completion(const char *text, int start, int end)
{
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, suggest_generator);
}

char *suggest_generator(const char *text, int state)
{
    static int len = 0;
    char buff1[BUFF_SIZE], buff2[BUFF_SIZE], buff3[BUFF_SIZE], *p;
    if (strlen(text) == 0)
    {
        return NULL;
    }
    if (!state)
    {
        memset(buff1, 0, BUFF_SIZE);
        memset(buff2, 0, BUFF_SIZE);
        strcpy(buff3, text);
        len = strlen(text);
        btsel(dict, buff3, buff2, BUFF_SIZE, &size);
    }
    while (btseln(dict, buff3, buff2, BUFF_SIZE, &size) == 0)
    {
        if (strncmp(buff3, text, len) == 0)
        {
            p = strdup(buff3);
            return p;
        }
        else
        {
            return NULL;
        }
    }
    return NULL;
}
