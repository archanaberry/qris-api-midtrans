#include "QRISAPI-CPP/QRISAPI.h"
#include <iostream>
#include <map>

int main() {
    try {
        // Ganti URL dan API Key dengan milik Anda
        QRISAPI qris("https://api.qris.com/v1", "your-api-key-here");

        std::map<std::string, std::string> params = {
            {"merchant_id", "123456"},
            {"amount", "10000"},
            {"currency", "IDR"},
            {"description", "Uji coba pembayaran QRIS"}
        };

        std::string qrData = qris.generateQRIS(params);
        std::cout << "QRIS Data: " << qrData << std::endl;

        std::cout << "Generated QR Code:" << std::endl;
        qris.printQRCode(qrData);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
