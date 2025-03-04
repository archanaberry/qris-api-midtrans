#include "qris_api.h"
#include <string.h>

// Callback untuk menyimpan response dari cURL
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t real_size = size * nmemb;
    struct MemoryStruct {
        char *memory;
        size_t size;
    };
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + real_size + 1);
    if (!ptr) return 0;
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, real_size);
    mem->size += real_size;
    mem->memory[mem->size] = '\0';
    return real_size;
}

char *request_midtrans(const char *json_data) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    struct MemoryStruct {
        char *memory;
        size_t size;
    } response;
    response.memory = malloc(1);
    response.size = 0;

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Gagal inisialisasi cURL\n");
        return NULL;
    }

    // Header HTTP
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Accept: application/json");

    /* 
       Header Authorization akan diisi dengan API key yang didapat dari config (sudah didekripsi di main)
       Misalnya, format: "Authorization: Basic <base64_api_key>".
       Di sini kita asumsikan string API key sudah dalam format yang benar.
    */
    extern char *global_api_key;  // variabel global yang diisi dari config
    char auth_header[256];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Basic %s", global_api_key);
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl, CURLOPT_URL, MIDTRANS_API_URL);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "cURL error: %s\n", curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    return response.memory;
}

char *generate_qris(const char *merchant_id, const char *amount, const char *acquirer) {
    char json_data[1024];
    /* 
       Membangun JSON data untuk transaksi QRIS.
       Termasuk merchant_id, gross_amount, dan acquirer yang dipilih.
    */
    snprintf(json_data, sizeof(json_data),
        "{"
            "\"payment_type\": \"qris\","
            "\"transaction_details\": {"
                "\"order_id\": \"order-%s\","
                "\"gross_amount\": %s"
            "},"
            "\"merchant_id\": \"%s\","
            "\"qris\": {"
                "\"acquirer\": \"%s\""
            "}"
        "}",
        merchant_id, amount, merchant_id, acquirer);

    return request_midtrans(json_data);
}
