#ifndef LISTINGS_H
#define LISTINGS_H

#include <iostream>
#include <curl/curl.h> 
#include <fstream>
#include <vector>

class listings{

    private:
        std::string _myJson(std::string json, std::string key);        

    public:
        listings();
        ~listings();
        int getListingURLs(std::string listingURL);
        std::vector<std::string> getListingJsonFromURLs();
        std::vector<std::string> removeduplicateURLs(std::vector<std::string> urls);
        int getListingImages(std::string path, std::string url);

};

#endif