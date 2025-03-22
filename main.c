#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

// Таблица предвычисленных значений для CRC32
static uint32_t crc32_table[256];

// Функция для инициализации таблицы CRC32
void init_crc32_table() {
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t crc = i;
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
        crc32_table[i] = crc;
    }
}

// Функция для обновления CRC32
uint32_t update_crc32(uint32_t crc, const uint8_t *data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        crc = (crc >> 8) ^ crc32_table[(crc ^ data[i]) & 0xFF];
    }
    return crc;
}

// Основная функция программы
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <путь к файлу>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *file_path = argv[1];
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("Ошибка открытия файла");
        return EXIT_FAILURE;
    }

    // Инициализация таблицы CRC32
    init_crc32_table();

    uint32_t crc = 0xFFFFFFFF; // Начальное значение CRC32
    uint8_t buffer[4096];      // Буфер для чтения данных
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        crc = update_crc32(crc, buffer, (size_t)bytes_read);
    }

    if (bytes_read == -1) {
        perror("Ошибка чтения файла");
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);

    crc ^= 0xFFFFFFFF; // Финальное инвертирование битов

    printf("CRC32: %08X\n", crc);
    return EXIT_SUCCESS;
}