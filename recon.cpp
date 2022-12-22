#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>
using namespace std;

bool makeRequest(CURL* curl, string url);
size_t writeData(void* buffer, size_t size, size_t nmemb, void* userp);

int main(int argc, char* argv[]) {
    // Initialize variables to store the URL and file name
    string url;
    string filename;
    int requestPerMinute;

    // Parse the command-line options
    int opt;
    while ((opt = getopt(argc, argv, "u:w:r:")) != -1) {
        switch (opt) {
            case 'u':
                url = optarg;
                break;
            case 'w':
                filename = optarg;
                break;
            case 'r':
                requestPerMinute = stoi(optarg);
                break;

            default:
                cerr << "Usage: " << argv[0] << " -u <URL> -w <filename>" << std::endl;
                return 1;
        }
    }

    // Check if the URL and file name were set
    if (url.empty() || filename.empty()) {
        cerr << "Usage: " << argv[0] << " -u <URL> -w <filename>" << std::endl;
        return 1;
    }

	// Open wordlist
    ifstream file(filename);
    if (!file.is_open()){
        cerr << "Error: Unable to open file '" << filename << "'" << endl;
        return 1;
    }

    CURL* curl = curl_easy_init();

    string line;
    int counter = 1;
    time_t start = clock();
    while (getline(file,line)){
        string fullUrl = url + line;

        makeRequest(curl, fullUrl);

		// Delay if -r flag was set
        if(requestPerMinute){
            while(static_cast<double>((clock() - start))/CLOCKS_PER_SEC < counter * 60.0/requestPerMinute);
            counter ++;
        }
    }

    curl_easy_cleanup(curl);

    return 0;
}

// Make a request and print the HTTP status code
bool makeRequest(CURL* curl, string url){

    long statusCode;
    bool result = false;
    if (curl){
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        CURLcode res = curl_easy_perform(curl);

		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
		result = true;

		cout << url << " ==> " << statusCode << endl;

    }

    return result;
}

// This to change default action of cURL and disable the output of the response
size_t writeData(void* buffer, size_t size, size_t nmemb, void* userp){
	return size * nmemb;
}
