#define PORT 5000

#include <unistd.h>      // Для gethostname
#include <netdb.h>       // Для getaddrinfo
#include <sys/socket.h>  // Для сокетов
#include <arpa/inet.h>   // Для inet_ntoa
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"

#define MAX_COMPUTERS 10

struct Computer {
    char ip[INET_ADDRSTRLEN];
    int cores;
    int isOnline;
};

// Функция для получения всех доступных компьютеров
int discoverComputers(Computer *computers, int maxCount) {
    int count = 0;

    // Получаем имя хоста текущего компьютера
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        perror("Ошибка получения имени хоста");
        return count;
    }

    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_STREAM;

    // Получаем информацию о текущем хосте
    if (getaddrinfo(hostname, NULL, &hints, &res) != 0) {
        perror("Ошибка получения информации о хосте");
        return count;
    }

    // Получаем IP-адреса всех доступных сетевых интерфейсов
    for (p = res; p != NULL; p = p->ai_next) {
        struct sockaddr_in *addr = (struct sockaddr_in *)p->ai_addr;
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(addr->sin_addr), ip, sizeof(ip));

        // Исключаем локальный IP-адрес (127.0.0.1)
        if (strcmp(ip, "127.0.0.1") == 0 || strcmp(ip, "localhost") == 0) {
            continue;
        }

        // Добавляем компьютер в список
        if (count < maxCount) {
            strncpy(computers[count].ip, ip, INET_ADDRSTRLEN);
            computers[count].cores = 4;  // Примерное количество ядер
            computers[count].isOnline = 1;  // Компьютер доступен
            count++;
        }
    }

    freeaddrinfo(res);

    return count;
}

// Функция для проверки доступности компьютера (ping)
int isReachable(const char *ip) {
    char command[128];
    snprintf(command, sizeof(command), "ping -c 1 -W 1 %s > /dev/null 2>&1", ip);
    return (system(command) == 0);
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