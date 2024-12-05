#ifndef QRIS_API_H
#define QRIS_API_H

#include <string>
#include <map>

class QRISAPI {
public:
    QRISAPI(const std::string& apiUrl, const std::string& apiKey);
    std::string generateQRIS(const std::map<std::string, std::string>& params);
    void printQRCode(const std::string& qrData);

private:
    std::string apiUrl;
    std::string apiKey;
    std::string httpPost(const std::string& endpoint, const std::string& data);
};

#endif
