
# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Werror -pthread -D_POSIX_C_SOURCE=200112L 
LDFLAGS =

# Имена исходных файлов и исполнимых файлов
SRC = main.c network.c tree.c
OBJ = $(SRC:.c=.o)
EXEC = program

# Поддерживаемые цели
.PHONY: all clean build help usage

# Сборка по умолчанию
all: build

# Сборка проекта
build: $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Компиляция каждого исходного файла
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка временных файлов и исполнимого файла
clean:
	rm -f $(OBJ) $(EXEC)

# Показываем подсказку по использованию
help:
	@echo "Справка по Makefile:"
	@echo "  make build    - Сборка проекта"
	@echo "  make clean    - Очистка временных файлов"
	@echo "  make help     - Показать это сообщение"
	@echo "  make usage    - Показать информацию по использованию исполнимого файла"

# Информация по использованию исполнимого файла
usage:
	@echo "Использование: ./program <имя_файла>"
	@echo "  <имя_файла> - Путь к файлу с данными для балансировки дерева"
