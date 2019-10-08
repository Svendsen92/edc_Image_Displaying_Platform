#include <iostream> 
#include <unistd.h>
#include <vector>
#include <memory>
#include <thread>
#include <signal.h>
#include <stdio.h> 
#include "opencv2/highgui/highgui.hpp"

#include <listings.h>


struct thread_args {
	std::vector<std::string> caseNo = {"93004108", "98401577", "64201529", "60203328"};

  	// locks for the different threads
  	pthread_mutex_t getImages_mutex = PTHREAD_MUTEX_INITIALIZER;
  	pthread_cond_t  getImages_cond = PTHREAD_COND_INITIALIZER;
};

std::string GetCurrentWorkingDir() {

  char buff[FILENAME_MAX];
  getcwd(buff, FILENAME_MAX);
  std::string current_working_dir(buff);

  return current_working_dir;
}

void init(void* a){

	std::cout << "init()" << std::endl;

	thread_args *b;
	b = (thread_args*)a;

	listings list;

	std::string dirPath = GetCurrentWorkingDir();
	
	size_t listing_ID = 0;

	for (size_t i = 0; i < b->caseNo.size(); ++i)
	{
	
		std::string listingURL = "https://www.edc.dk/alle-boliger/?sagsnr=" + b->caseNo[listing_ID];

	    if(list.getListingURLs(listingURL)){
	    	std::cout << "Unable to aquire image from URL!\n";
	    }
	    else
	    {

		    std::vector<std::string> url;
		    url = list.getListingJsonFromURLs();

		    url = list.removeduplicateURLs(url);
		    
		    for (size_t i = 0; i < url.size(); ++i)
		    {
		    	std::string URL = "https:" + url[i];
				std::string path = dirPath + "/images/Image_" + std::to_string(listing_ID) + "-" + std::to_string(i) + ".jpg";

				list.getListingImages(path, URL);
			}

			listing_ID++;

			if (listing_ID > b->caseNo.size())
			{
				listing_ID = 0;
			}
		}
	}
}

void* getImages(void* a) {

	std::cout << "getImages()" << std::endl;

	thread_args *b;
	b = (thread_args*)a;

	listings list;

	std::string dirPath = GetCurrentWorkingDir();

	size_t listing_ID = 0;

	while(true){

		pthread_cond_wait(&b->getImages_cond, &b->getImages_mutex);

		std::string listingURL = "https://www.edc.dk/alle-boliger/?sagsnr=" + b->caseNo[listing_ID];

	    if(list.getListingURLs(listingURL)){
	    	std::cout << "Unable to aquire image from URL!\n";
	    }
	    else
	    {
		    std::vector<std::string> url;
		    url = list.getListingJsonFromURLs();

		    url = list.removeduplicateURLs(url);
		    
		    for (size_t i = 0; i < url.size(); ++i)
		    {
		    	std::string URL = "https:" + url[i];
				std::string path = dirPath + "/images/Image_" + std::to_string(listing_ID) + "-" + std::to_string(i) + ".jpg";

				list.getListingImages(path, URL);
			}
		}

		listing_ID++;

		if (listing_ID >= b->caseNo.size())
		{
			listing_ID = 0;
		}
	}
}

void* displayImages(void* a) {

	std::cout << "displayImages()" << std::endl;

	thread_args *b;
	b = (thread_args*)a;

	cv::Mat dispImg;
	std::string dirPath = GetCurrentWorkingDir();
	std::string imgPath = dirPath + "/images/Image_";
	std::string nextListing;
	size_t listing_ID = 0, imageNum = 0;
	size_t dispTime = 2000;

	while(true){

		nextListing = std::to_string(listing_ID) + "-" + std::to_string(imageNum) + ".jpg";
		dispImg = cv::imread(imgPath + nextListing);

		if(!dispImg.data)
	    {
	        //std::cout << "Could not open or find the image!\n";
	        std::cout << "displayImages() >> listing_ID: " << listing_ID << std::endl;
			listing_ID++;
			imageNum = 0;

			if (listing_ID >= b->caseNo.size())
			{
				listing_ID = 0;
			}

			pthread_cond_signal(&b->getImages_cond);
	    }
	    else
	    {
	    	imageNum++;

	    	namedWindow("Display window", cv::WINDOW_NORMAL);
	    	cv::resizeWindow("Display window", 400, 400);
		    imshow("Display window", dispImg);

		    cv::waitKey(dispTime); 
	    }
	}
}



int main(){

	thread_args *args = new thread_args;
	
	init((void*) args);

  	// Starts the threads
  	std::thread thread1(getImages, args);
  	sleep(2);
	std::thread thread2(displayImages, args);
	sleep(2);

	// Holds the main thread until the thread has finished
	thread1.join();
  	thread2.join();
	

    return 0;
}

