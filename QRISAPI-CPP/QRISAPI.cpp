#include "QRISAPI.h"
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <qrencode.h>

QRISAPI::QRISAPI(const std::string& apiUrl, const std::string& apiKey)
    : apiUrl(apiUrl), apiKey(apiKey) {}

std::string QRISAPI::httpPost(const std::string& endpoint, const std::string& data) {
    CURL* curl = curl_easy_init();
    if (!curl) throw std::runtime_error("Failed to initialize cURL");

    std::string response;
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, (apiUrl + endpoint).c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void* contents, size_t size, size_t nmemb, void* userp) -> size_t {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        throw std::runtime_error("cURL error: " + std::string(curl_easy_strerror(res)));
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    return response;
}

std::string QRISAPI::generateQRIS(const std::map<std::string, std::string>& params) {
    std::ostringstream jsonData;
    jsonData << "{";
    for (auto it = params.begin(); it != params.end(); ++it) {
        if (it != params.begin()) jsonData << ",";
        jsonData << "\"" << it->first << "\":\"" << it->second << "\"";
    }
    jsonData << "}";

    return httpPost("/generate", jsonData.str());
}

void QRISAPI::printQRCode(const std::string& qrData) {
    QRcode* qr = QRcode_encodeString(qrData.c_str(), 0, QR_ECLEVEL_Q, QR_MODE_8, 1);
    if (!qr) throw std::runtime_error("Failed to encode QR data");

    for (int y = 0; y < qr->width; y++) {
        for (int x = 0; x < qr->width; x++) {
            std::cout << (qr->data[y * qr->width + x] & 1 ? "██" : "  ");
        }
        std::cout << std::endl;
    }
    QRcode_free(qr);
}
