# Имя исполняемого файла
TARGET = crc32_calculator

# Исходный файл
SRC = crc32_calculator.c

# Компилятор
CC = gcc

# Флаги компиляции
CFLAGS = -Wall -Wextra -Wpedantic -std=c11

# Цель по умолчанию
all: $(TARGET)

# Правило для создания исполняемого файла
$(TARGET): $(SRC)
    $(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Правило для очистки скомпилированных файлов
clean:
    rm -f $(TARGET)

# Правило для запуска программы (для удобства тестирования)
run: $(TARGET)
    ./$(TARGET) test_file.txt

.PHONY: all clean run