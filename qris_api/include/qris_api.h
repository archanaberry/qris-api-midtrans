#ifndef QRIS_API_H
#define QRIS_API_H

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <jansson.h>

// Endpoint Midtrans (sandbox)
#define MIDTRANS_API_URL "https://api.sandbox.midtrans.com/v2/charge"

// Fungsi untuk mengirim request ke Midtrans dan mengembalikan respon
char *request_midtrans(const char *json_data);

// Fungsi untuk membuat transaksi QRIS dengan parameter merchant_id, amount, dan acquirer
char *generate_qris(const char *merchant_id, const char *amount, const char *acquirer);

#endif
