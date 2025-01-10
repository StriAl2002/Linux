#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "network.h"
#include "tree.h"

#define CHECK_INTERVAL 5  // Интервал проверки доступности компьютеров

// Глобальные переменные
int *values;      // Массив значений для расчёта
int size = 0;     // Количество элементов
Computer *computers; // Список доступных компьютеров
int computerCount = 0; // Количество доступных компьютеров

// Функция для мониторинга доступности компьютеров
void *monitorComputers(void *arg) {
    while (1) {
        for (int i = 0; i < computerCount; i++) {
            if (!isReachable(computers[i].ip)) {
                printf("Соединение с компьютером %s потеряно. Перезапустите расчёт.\n", computers[i].ip);
                pthread_exit(NULL);  // Завершаем мониторинг
            }
        }
        sleep(CHECK_INTERVAL);
    }
}

// Главная функция
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Использование: %s <имя_файла>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Чтение данных из файла
    readFile(argv[1]);

    // Обнаружение доступных компьютеров в сети
    computers = (Computer *)malloc(256 * sizeof(Computer));
    computerCount = discoverComputers(computers, 256);

    if (computerCount == 0) {
        printf("Нет доступных компьютеров в сети.\n");
        return EXIT_FAILURE;
    }

    // Подсчёт доступных ядер на каждом компьютере
    for (int i = 0; i < computerCount; i++) {
        computers[i].cores = getCoreCount(computers[i].ip);
        printf("IP: %s, Ядер: %d\n", computers[i].ip, computers[i].cores);
    }

    // Запуск потока мониторинга доступности компьютеров
    pthread_t monitorThread;
    MonitorArgs args = {computers, computerCount};
    pthread_create(&monitorThread, NULL, monitorComputers, &args);

    // Распределение задач по компьютерам
    distributeTasks(values, size, computers, computerCount);

    // Завершаем мониторинг после выполнения задач
    pthread_cancel(monitorThread);
    pthread_join(monitorThread, NULL);

    // Очистка памяти
    free(computers);
    free(values);

    return 0;
}