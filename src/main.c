#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#include "btree.h"

#define BUFF_SIZE 100
int size;
BTA *dictionary_anh_viet;
BTA *dictionary_viet_anh;

int get_int()
{
	char string[256];
	do
	{
		fgets(string, 256, stdin);
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
		fgets(string, 256, stdin);
	} while (string[0] == '\n');
	if (string[strlen(string) - 1] == '\n')
	{
		string[strlen(string) - 1] = '\0';
	}
}

char **suggest_completion(const char *, int, int);
char *suggest_generator(const char *, int);

int main()
{
	rl_attempted_completion_function = suggest_completion;
	int n;
	int index_1;
	// int index_2;
	char value[BUFF_SIZE];
	char key[BUFF_SIZE];
	char *buff = NULL;
	btinit();
	dictionary_anh_viet = btopn("anh_viet", 0, 0);
	dictionary_viet_anh = btopn("viet_anh", 0, 0);
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
			btins(dictionary_anh_viet, strdup(key), strdup(value), BUFF_SIZE);
			btins(dictionary_viet_anh, strdup(value), strdup(key), BUFF_SIZE);
			break;
		case 2:
			buff = readline("Key: ");
			if (buff[strlen(buff) - 1] == ' ')
			{
				buff[strlen(buff) - 1] = '\0';
			}
			if (btsel(dictionary_anh_viet, buff, value, BUFF_SIZE, &size))
				printf("Not found value for %s!\n", buff);
			else
				printf("Value of %s is %s\n", buff, value);
			free(buff);
			break;
		case 3:
			printf("Key: ");
			get_line(key);
			btdel(dictionary_anh_viet, key);
			break;
		case 4:
			index_1 = 0;
			// index_2 = 0;
			btsel(dictionary_anh_viet, "", value, BUFF_SIZE, &size);
			printf("------------------------------------------------------\n");
			printf("|%-10s|%-20s|%-20s|\n------------------------------------------------------\n", "INDEX", "KEY", "VALUE");
			while (btseln(dictionary_anh_viet, key, value, BUFF_SIZE, &size) == 0)
			{
				index_1++;
				printf("|%-10d|%-20s|%-20s|\n------------------------------------------------------\n", index_1, key, value);
			}
			// btsel(dictionary_viet_anh, "", value, BUFF_SIZE, &size);
			// printf("|%-10s|%-20s|%-20s|\n------------------------------------------------------\n", "INDEX", "KEY", "VALUE");
			// while (btseln(dictionary_viet_anh, key, value, BUFF_SIZE, &size) == 0)
			// {
			// 	index_2++;
			// 	printf("|%-10d|%-20s|%-20s|\n------------------------------------------------------\n", index_2, key, value);
			// }
			break;
		case 5:
			btcls(dictionary_anh_viet);
			btcls(dictionary_viet_anh);
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
	static int len, n = 0;
	char value[BUFF_SIZE], key[BUFF_SIZE];
	if (!state)
	{
		n = 0;
		strcpy(key, text);
		len = strlen(text);
	}

	if (!btsel_n(dictionary_anh_viet, key, value, BUFF_SIZE, &size, len) && n < 10)
	{
		if (!btsel(dictionary_viet_anh, value, key, BUFF_SIZE, &size))
		{
			n++;
			return strdup(key);
		}
	}
	return NULL;
}