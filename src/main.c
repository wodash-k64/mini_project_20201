#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#include "btree.h"

#define BUFF_SIZE 50

// dict là con trỏ tới BTA được lưu trong dict_db,
int size;
BTA *dict;
// Hàm phục vụ chức năng autocomplete
char **suggest_completion(const char *, int, int);
// Hàm phục vụ chức năng autocomplete
char *suggest_generator(const char *, int);

int main()
{
    // Dùng cho chức năng autocomplete
    rl_attempted_completion_function = suggest_completion;
    // Biến lưu trữ lựa chọn người dùng
    int n = 0;
    // Các biến liên quan đến xâu
    char *buff_key = malloc(BUFF_SIZE);
    char *buff_value = malloc(BUFF_SIZE);
    char *buff_search = malloc(BUFF_SIZE);
    // Khởi tạo BTA
    btinit();
    dict = btopn("dict_db", 0, 0);
    // Nếu không có file dict_db khởi tạo từ dữ liệu lấy từ file data.txt
    if (!dict)
    {
        // Đọc file data.txt và tạo file dict_db
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
                memset(buff_key, 0, BUFF_SIZE);
                memset(buff_value, 0, BUFF_SIZE);
                fscanf(f_data, "%s", buff_key);
                fgetc(f_data);
                // Value là cipher code của key dịch phải 1 đơn vị
                for (int i = 0; i < strlen(buff_key); ++i)
                {
                    if (buff_key[i] == 'z')
                    {
                        buff_value[i] = 'a';
                    }
                    else
                    {
                        buff_value[i] = (char)(buff_key[i] + 1);
                    }
                }
                btins(dict, buff_key, buff_value, BUFF_SIZE);
            }
        }
        fclose(f_data);
    }
    // MENU
    do
    {
        // Xóa màn hình mỗi lần chạy xong 1 chức năng và in ra giao diện menu này
        system("clear");
        printf("--------------------------------------------------\n");
        printf("-------------------DICTIONARY APP-----------------\n");
        printf("--------------------------------------------------\n");
        printf("==================================================\n");
        printf("| 1.Add | 2.Search | 3.Delete | 4.Print | 5.Exit |\n");
        printf("==================================================\n");
        printf("-> Enter your choice: ");
        __fpurge(stdin);
        scanf("%d", &n);
        switch (n)
        {
        // Thêm từ vào từ điển
        case 1:
            printf("Key: ");
            __fpurge(stdin);
            fgets(buff_key, BUFF_SIZE, stdin);
            if (buff_key[strlen(buff_key) - 1] == '\n')
            {
                buff_key[strlen(buff_key) - 1] = '\0';
            }
            printf("Value: ");
            __fpurge(stdin);
            fgets(buff_value, BUFF_SIZE, stdin);
            if (buff_value[strlen(buff_key) - 1] == '\n')
            {
                buff_value[strlen(buff_key) - 1] = '\0';
            }
            btins(dict, buff_key, buff_value, BUFF_SIZE);
            break;
        // Tìm kiếm có tab complete hỗ trợ
        case 2:
            buff_key = readline("Search: ");
            if (buff_key[strlen(buff_key) - 1] == ' ')
            {
                buff_key[strlen(buff_key) - 1] = '\0';
            }
            if (btsel(dict, buff_key, buff_value, BUFF_SIZE, &size) == 0)
            {
                printf("Value of %s is %s\n", buff_key, buff_value);
            }
            else
            {
                printf("Value of %s not found!!!\n", buff_key);
            }
            break;
        // Xóa một từ khỏi từ điển
        case 3:
            break;
        // In ra danh sách các từ bắt đầu với giá trị từ buff_key
        case 4:
            printf("Start with: ");
            __fpurge(stdin);
            fgets(buff_key, BUFF_SIZE, stdin);
            if (buff_key[strlen(buff_key) - 1] == '\n')
            {
                buff_key[strlen(buff_key) - 1] = '\0';
            }
            // i dùng để đếm index cho việc in ra, len chứa độ dài xâu buff_key để so sánh trong strncmp
            int i = 0;
            int len = strlen(buff_key);
            strcpy(buff_search, buff_key);
            // Đưa vị trí con trỏ trong dict về vị trí gần text nhất
            btsel(dict, buff_key, buff_value, BUFF_SIZE, &size);
            printf("==================================================\n|%-14s| %-15s| %-15s|\n==================================================\n", "Index", "Key", "Value");
            // Dịch chuyển con trỏ sang vị trí tiếp theo trong cây
            while (btseln(dict, buff_key, buff_value, BUFF_SIZE, &size) == 0)
            {
                // Nếu khớp len kí tự thì in ra, dừng khi không khớp nữa
                if (strncmp(buff_search, buff_key, len) == 0)
                {
                    i++;
                    printf("|No.%-11d| %-15s| %-15s|\n--------------------------------------------------\n", i, buff_key, buff_value);
                }
                else
                {
                    break;
                }
            }
            break;
        // Chào tạm biệt trước khi thoát
        case 5:
            printf("-> Good bye!!!\n");
            break;
        // Thông báo nhập n không hợp lệ
        default:
            printf("Invalid choice!!!\n");
            break;
        }
        // Dừng màn hình trong trường hợp tìm kiếm và in ra
        if (n != 1 && n != 3 && n != 5)
        {
            __fpurge(stdin);
            printf("Press enter to continue!!!");
            getchar();
        }
    } while (n != 5);
    system("rm dict_db");
    free(buff_key);
    free(buff_value);
    free(buff_search);
    return 0;
}

char **suggest_completion(const char *text, int start, int end)
{
    // Khi không có từ gợi ý sẽ không fallback về gợi ý đường dẫn file
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, suggest_generator);
}

char *suggest_generator(const char *text, int state)
{
    // len lưu lại độ dài của text, các buff lưu key và value
    static int len = 0;
    char buffvalue[BUFF_SIZE], buffkey[BUFF_SIZE];
    if (strlen(text) == 0)
    {
        return NULL;
    }
    // Khi hàm này được gọi lần đầu state sẽ bằng 0
    if (!state)
    {
        memset(buffvalue, 0, BUFF_SIZE);
        strcpy(buffkey, text);
        len = strlen(text);
        // Đưa vị trí con trỏ trong dict về vị trí gần text nhất
        btsel(dict, buffkey, buffvalue, BUFF_SIZE, &size);
    }
    // Dịch chuyển con trỏ sang vị trí tiếp theo trong cây
    while (btseln(dict, buffkey, buffvalue, BUFF_SIZE, &size) == 0)
    {
        // Trả về nếu khớp len kí tự, nếu không trả về NULL kết thúc hàm complete
        if (strncmp(buffkey, text, len) == 0)
        {
            return strdup(buffkey);
        }
        else
        {
            return NULL;
        }
    }
    return NULL;
}
