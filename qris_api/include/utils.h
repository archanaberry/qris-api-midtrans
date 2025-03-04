#ifndef UTILS_H
#define UTILS_H

// Fungsi untuk dekripsi file menggunakan XOR dengan key sederhana
// Mengembalikan string hasil dekripsi (harus di-free oleh pemanggil)
char *decrypt_file(const char *filename, const char *xor_key);

// Fungsi untuk parsing file konfigurasi (format: key = "value")
// Mengembalikan nilai untuk key yang dicari (harus di-free oleh pemanggil)
char *parse_config_value(const char *content, const char *key);

// Fungsi untuk parsing file .env (format: KEY = value)
// Mengembalikan nilai untuk key yang dicari (harus di-free oleh pemanggil)
char *parse_env_value(const char *content, const char *key);

#endif
