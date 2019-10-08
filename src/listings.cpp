#include "listings.h"

listings::listings(){}

listings::~listings(){}


int listings::getListingURLs(std::string listingURL){

    CURL *image;
    FILE *fp = nullptr;
    
    const char *url = listingURL.c_str();

    image = curl_easy_init();
    if (image)
    {
        // Open file 
        if ((fp = fopen("listingJson.txt", "wb")) == NULL){
            std::cout << "File cannot be opened!\n";
            return 1;   
        } 

        curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(image, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(image, CURLOPT_URL, url);
        curl_easy_setopt(image, CURLOPT_FOLLOWLOCATION, 1);

        // Grab image 
        if (curl_easy_perform(image)){
            std::cout << "Cannot grab the listing URL Json!\n";
            return 1;
        }
    }
    // Clean up the resources 
    curl_easy_cleanup(image);
    // Close the file 
    fclose(fp);

    return 0;
}


std::string listings::_myJson(std::string json, std::string key){

    int cnt = 0;
    int jsonLen = json.length();
    std::string k = "";
    std::string data = "";

    for (int i = 0; i < jsonLen; i++){

        if (json[i] == key[cnt]){
            k += json[i];
            if (k == key){
                int idx = i +4;
                while (true){
                    data += json[idx];
                    idx++;
                    if (json[idx +1] == ',' || json[idx] == '}'){
                        return data;
                    }else if (idx > jsonLen){
                        break;
                    }
                }
            }
            cnt++;
        }
    }
    return data;
}


std::vector<std::string> listings::getListingJsonFromURLs(){

    std::vector<std::string> Json;
    bool run = true;
    int idx = 0;

    std::ifstream myfile("listingJson.txt");
    if (myfile.is_open()){
        std::string line = "";
        
        while (getline(myfile, line))
        {
            while(run)
            {
                int s = line.find("LargeImageUrl");
                //std::cout << "s: " << s << std::endl;
                if (s < 0)
                {
                    run = false;
                }
                else
                {
                    Json.push_back(_myJson(line, "LargeImageUrl"));
                    //std::cout << "\nJson[" << idx << "]: " << Json[idx] << "\n" << std::endl;
                    idx++;
                    line = line.erase(0, s+5);
                    //std::cout << "line: " << line << "\n" << std::endl;
                }
            }

            run = true;
        }

        myfile.close();

    }
    else 
    {
        std::cout << "Unable to open file\n";
    }

    return Json;
}


std::vector<std::string> listings::removeduplicateURLs(std::vector<std::string> urls){

    for (size_t i = 0; i < urls.size(); ++i)
    {
        std::string tempStr = urls[i];
        for (size_t j = 0; j < urls.size(); ++j)
        {
            if (tempStr == urls[j]){
                urls.erase (urls.begin()+j);
            }
        }
    }

    return urls;
}


int listings::getListingImages(std::string path, std::string url){

    
    CURL *image;
    FILE *fp = nullptr;

    image = curl_easy_init();
    if (image)
    {
        // Open file 
        if ((fp = fopen(path.c_str(), "wb")) == NULL){
            std::cout << "File cannot be opened!\n";
            return 1;   
        } 

        curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(image, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(image, CURLOPT_URL, url.c_str());
        curl_easy_setopt(image, CURLOPT_FOLLOWLOCATION, 1);

        // Grab image 
        if (curl_easy_perform(image)){
            std::cout << "Cannot grab the image!\n";
            return 1;
        }
    }
    else
    {
        std::cout << "Failed to instansiate curl!!\n";
        return 1;
    }
    // Clean up the resources 
    curl_easy_cleanup(image);
    // Close the file 
    fclose(fp);

    return 0;
}
