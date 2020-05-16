#include <iostream>
#include <string>
#include "curl/curlver.h"
#include "curl/curl.h"


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


int main() {
    std::cout << "hello world" << std::endl;
    std::cout << "LIBCURL VERSION: " << LIBCURL_VERSION << std::endl;
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if (curl) {
        std::cout << "Testing cURL with https protocol\n";
        curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "testing=1");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    std::cout << "Contents of CURL: " << readBuffer << std::endl;

    return 0;
}
