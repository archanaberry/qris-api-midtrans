#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fungsi dekripsi XOR
char *decrypt_file(const char *filename, const char *xor_key) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Gagal membuka file %s\n", filename);
        return NULL;
    }

    // Dapatkan ukuran file
    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    rewind(fp);

    char *buffer = malloc(filesize + 1);
    if (!buffer) {
        fclose(fp);
        return NULL;
    }
    fread(buffer, 1, filesize, fp);
    buffer[filesize] = '\0';
    fclose(fp);

    size_t key_len = strlen(xor_key);
    // Dekripsi setiap byte menggunakan XOR (rotasi kunci)
    for (long i = 0; i < filesize; i++) {
        buffer[i] ^= xor_key[i % key_len];
    }
    return buffer;
}

// Fungsi untuk parsing nilai konfigurasi berdasarkan key (misal: apikey = "value")
char *parse_config_value(const char *content, const char *key) {
    char *found = strstr(content, key);
    if (!found) return NULL;

    // Cari tanda "=" setelah key
    found = strchr(found, '=');
    if (!found) return NULL;
    found++; // lewati '='

    // Hilangkan spasi di awal
    while (*found == ' ' || *found == '\t') found++;

    // Jika ada tanda petik, cari isinya
    if (*found == '\"') {
        found++; // lewati petik awal
        char *end = strchr(found, '\"');
        if (!end) return NULL;
        size_t len = end - found;
        char *value = malloc(len + 1);
        strncpy(value, found, len);
        value[len] = '\0';
        return value;
    } else {
        // Ambil hingga newline
        char *end = strchr(found, '\n');
        size_t len = end ? (size_t)(end - found) : strlen(found);
        char *value = malloc(len + 1);
        strncpy(value, found, len);
        value[len] = '\0';
        return value;
    }
}

// Fungsi parsing untuk file .env (format: KEY = value)
// Mengembalikan nilai (trim whitespace) dari baris yang mengandung key
char *parse_env_value(const char *content, const char *key) {
    return parse_config_value(content, key);
}
