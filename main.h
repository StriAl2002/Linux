#ifndef MAIN_H
#define MAIN_H

#include "network.h"

// Интервал проверки доступности компьютеров
#define CHECK_INTERVAL 5

// Структура для передачи аргументов в поток мониторинга
typedef struct {
    Computer *computers;
    int computerCount;
} MonitorArgs;

#endif // MAIN_H