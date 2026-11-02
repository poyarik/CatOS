#ifndef ATA_H
#define ATA_H

#include <stdint.h>
#include "io.h"

// Структура BPB для FAT32 (упакованная, чтобы компилятор не добавлял мусорные отступы)
typedef struct __attribute__((packed)) {
    uint8_t  jmp[3];
    uint8_t  oem[8];
    uint16_t bytes_per_sector;      // Обычно 512
    uint8_t  sectors_per_cluster;    // Размер кластера в секторах (например, 8)
    uint16_t reserved_sectors;      // Сколько секторов пропускаем в начале (например, 32)
    uint8_t  num_fats;              // Количество таблиц FAT (обычно 2)
    uint16_t root_entry_count;      // В FAT32 это всегда 0!
    uint16_t total_sectors_16;      // В FAT32 обычно 0, размер хранится ниже
    uint8_t  media_type;
    uint16_t fat_size_16;           // В FAT32 тоже 0
    uint16_t sectors_per_track;
    uint16_t num_heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_32;      // Полное число секторов на диске
    
    // Специфичные поля FAT32:
    uint32_t fat_size_32;           // Размер одной таблицы FAT в секторах!
    uint16_t ext_flags;
    uint16_t fs_version;
    uint32_t root_cluster;          // Кластер корневой директории (обычно 2)
    uint16_t fs_info;
    uint16_t backup_boot_sector;
    uint8_t  reserved[12];
    uint8_t  drive_number;
    uint8_t  reserved1;
    uint8_t  boot_signature;
    uint32_t volume_id;
    uint8_t  volume_label[11];
    uint8_t  file_system_type[8];   // Должно быть "FAT32
} fat32_bpb_t;

int read_sector(uint32_t lba, uint8_t *target_buffer);

#endif
