#include "network.h"
#ifndef TREE_UTILS_H
#define TREE_UTILS_H

// Структура для узла бинарного дерева
typedef struct TreeNode {
    int value;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Глобальные переменные
extern int *values; // Массив значений для расчёта
extern int size;    // Количество элементов

// Функции для работы с деревом
TreeNode* createNode(int value);
TreeNode* insert(TreeNode *root, int value);
TreeNode* balanceTree(int *values, int start, int end);
void readFile(const char *filename);
void distributeTasks(int *values, int size, Computer *computers, int computerCount);

#endif // TREE_UTILS_H