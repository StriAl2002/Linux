#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

// Функция для создания нового узла
TreeNode* createNode(int value) {
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) {
        perror("Ошибка выделения памяти для нового узла");
        exit(EXIT_FAILURE);
    }
    node->value = value;
    node->left = node->right = NULL;
    return node;
}

// Функция для вставки значения в бинарное дерево
TreeNode* insert(TreeNode *root, int value) {
    if (root == NULL) {
        return createNode(value);
    }

    if (value < root->value) {
        root->left = insert(root->left, value);
    } else {
        root->right = insert(root->right, value);
    }

    return root;
}

// Функция для балансировки бинарного дерева
TreeNode* balanceTree(int *values, int start, int end) {
    if (start > end) return NULL;

    int mid = (start + end) / 2;
    TreeNode *root = createNode(values[mid]);

    root->left = balanceTree(values, start, mid - 1);
    root->right = balanceTree(values, mid + 1, end);

    return root;
}

// Функция для чтения данных из файла
void readFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    // Считаем количество чисел в файле
    int value;
    while (fscanf(file, "%d", &value) == 1) {
        size++;
        values = realloc(values, size * sizeof(int));
        if (values == NULL) {
            perror("Ошибка выделения памяти для данных");
            exit(EXIT_FAILURE);
        }
        values[size - 1] = value;
    }

    fclose(file);
}

// Функция для распределения задач между машинами
void distributeTasks(int *values, int size, Computer *computers, int computerCount) {
    int offset = 0;
    int coresSum = 0;

    // Считаем общее количество ядер
    for (int i = 0; i < computerCount; i++) {
        coresSum += computers[i].cores;
    }

    for (int i = 0; i < computerCount; i++) {
        int taskSize = (computers[i].cores * size) / coresSum;
        if (taskSize > 0) {
            if (sendTask(computers[i].ip, &values[offset], taskSize) != 0) {
                printf("Ошибка отправки задачи на компьютер %s\n", computers[i].ip);
            }
            offset += taskSize;
        }
    }
}