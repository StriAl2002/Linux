#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <netinet/in.h>

// Структура для хранения информации о компьютере
typedef struct {
    char ip[INET_ADDRSTRLEN];
    int cores;
} Computer;

// Функции для работы с сетью
int isReachable(const char *ip);
int discoverComputers(Computer *computers, int maxCount);
int getCoreCount(const char *ip);
int sendTask(const char *ip, int *data, int dataSize);

#endif // NETWORK_UTILS_H