#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define A -50   //Диапозон значений ключей
#define B 50    //[A; B]
//Пример: A + rand() % ((B + 1) - A);
#define N1 59 //Размер основной хэш-таблицы
#define N2 59 //Размер хэш-таблицы с положительными ключами
#define N3 59 //Размер хэш-таблицы с отрицательными ключами

struct zap {
    int key;
    int info;
    struct zap* next;
};
int zapSize = sizeof(struct zap);

int hashFun(int);
void printHashTable(struct zap**);
int add(int, int, struct zap**);
int addRand(int num, struct zap**);
void rm(int, struct zap**);
void move(int, struct zap**, struct zap**);

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<MAIN
int main() {
    srand(time(NULL));

    struct zap *mainHashTable[N1] = {NULL},
                *posHashTable[N2] = {NULL},
                *negHashTable[N3] = {NULL};

    addRand(50, mainHashTable);
    printHashTable(mainHashTable);

    for (int i = A; i <= B; ++i) {
        if (i < 0)
            move(i, mainHashTable, negHashTable);
        else if (i > 0)
            move(i, mainHashTable, posHashTable);
    }

    puts("Основная таблица:");
    printHashTable(mainHashTable);

    puts("Таблица с отрицательными ключами:");
    printHashTable(negHashTable);

    puts("Таблица с положительными ключами:");
    printHashTable(posHashTable);

    system("pause");
    return 0;
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

int hashFun(int key) {
    int n = N1;
    double a = 0.618033;

    return (int) fabs(n*modf(key*a, NULL)) + (key < 0?1:0);
}
void printHashTable(struct zap **table) {
    for (int i = 0; i < N1; ++i) {
        struct zap **n = &table[i];

        printf("[%d]\t", i);

        while (*n != NULL) {
            printf("{\"%d\": %d} -> ", (*n)->key, (*n)->info );
            n = &(*n)->next;
        }

        printf("NULL\n");
    }
    fflush(stdin);
    getchar();
}
int add(int key, int info, struct zap **table) {
    int i = hashFun(key);
    struct zap **n = &table[i];

    while (*n != NULL) {
        n = &(*n)->next;
    }

    *n = (struct zap *) malloc(zapSize);
    if (*n) {
        (*n)->key = key;
        (*n)->info = info;
        (*n)->next = NULL;
        return 1;
    } else {
        puts("Ошибка: пара не добавлоена!");
        system("pause");
        return 0;
    }
}
int addRand(int num, struct zap **table) {
    for (int j = 0; j < num; ++j) {
        int key = A + rand() % ((B + 1) - A);
        int info = A + rand() % ((B + 1) - A);

        add(key, info, table);
    }
}
void rm(int key, struct zap **table) {
    struct zap **n = &table[hashFun(key)], *e = NULL;

    while (*n != NULL) {
        if ((*n)->key == key) {
            e = (*n)->next;
            free(*n);
            *n = e;
        } else {
            n = &(*n)->next;
        }
    }
}
void move(int key, struct zap** table1, struct zap** table2) {
    struct zap **n = &table1[hashFun(key)];

    while (*n != NULL) {
        if ((*n)->key == key)
            add(key, (*n)->info, table2);

        n = &(*n)->next;
    }
    rm(key, table1);
}