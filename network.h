#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <netinet/in.h>

// Структура для хранения информации о компьютере
typedef struct {
    char ip[INET_ADDRSTRLEN];
    int cores;
    char isOnline;
} Computer;

// Функции для работы с сетью
int isReachable(const char *ip);
int discoverComputers(Computer *computers, int maxCount);
int getCoreCount(const char *ip);
int sendTask(const char *ip, int *data, int dataSize);

// Структура для передачи аргументов в поток мониторинга
typedef struct {
    Computer *computers;
    int computerCount;
    int checkInterval; // Интервал проверки доступности (в секундах)
} MonitorArgs;

#endif // NETWORK_UTILS_H