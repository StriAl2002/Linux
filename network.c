#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <errno.h>
#include <sys/sysinfo.h>
#include <pthread.h>

#define PORT 5000  // Порт для связи

// Структура для хранения информации о компьютере
typedef struct {
    char ip[INET_ADDRSTRLEN];
    int cores;
} Computer;

// Функция для проверки доступности компьютера (ping)
int isReachable(const char *ip) {
    char command[128];
    snprintf(command, sizeof(command), "ping -c 1 -W 1 %s > /dev/null 2>&1", ip);
    return (system(command) == 0);
}

// Получение доступных компьютеров в локальной сети
int discoverComputers(Computer *computers, int maxCount) {
    struct ifaddrs *ifaddr, *ifa;
    char baseIP[INET_ADDRSTRLEN];
    int count = 0;

    // Получение базового IP адреса
    if (getifaddrs(&ifaddr) == -1) {
        perror("Ошибка получения сетевого интерфейса");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
            inet_ntop(AF_INET, &(sa->sin_addr), baseIP, INET_ADDRSTRLEN);

            // Проверяем, является ли это локальной сетью
            if (strncmp(baseIP, "192.168.", 8) == 0 || strncmp(baseIP, "10.", 3) == 0) {
                break;
            }
        }
    }

    freeifaddrs(ifaddr);

    // Пинг всех возможных адресов в подсети
    char ip[INET_ADDRSTRLEN];
    for (int i = 1; i < 255; i++) {
        snprintf(ip, sizeof(ip), "%s.%d", baseIP, i);
        if (isReachable(ip)) {
            snprintf(computers[count].ip, sizeof(computers[count].ip), "%s", ip);
            count++;
            if (count >= maxCount) {
                break;
            }
        }
    }

    return count;
}

// Запрос количества ядер на удалённой машине
int getCoreCount(const char *ip) {
    // Для примера предполагаем, что на всех машинах одинаковое число ядер
    return sysconf(_SC_NPROCESSORS_ONLN);
}

// Отправка задачи на удалённый компьютер
int sendTask(const char *ip, int *data, int dataSize) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Ошибка создания сокета");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Не удалось подключиться к удалённому компьютеру");
        close(sock);
        return -1;
    }

    // Отправка данных
    if (send(sock, data, dataSize * sizeof(int), 0) < 0) {
        perror("Ошибка отправки данных");
        close(sock);
        return -1;
    }

    close(sock);
    return 0;
}