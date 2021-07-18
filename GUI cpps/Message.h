
#ifndef MAIN_CPP_MESSAGE_H
#define MAIN_CPP_MESSAGE_H
#include <functional>
#include <vector>
#include <map>
#include <string>
#include <cstdint>
#include <opencv2/opencv.hpp>

using namespace std;
class Message {
    typedef uint8_t index;
    typedef function<void(void)> ftype;
    vector<ftype> func_v;
    map<string,index> func_mp;
    function<cv::Mat()> passf;
public:
    cv::Mat image,*ptr;
    Message();
    void registerFunc(const string &s,ftype f);
    void callFunc(const string &s);
    void announceView();
    void registerPassF(function<cv::Mat()> &&f);
    void passImage() {
        image=passf();
        ptr=&image;
    }
};


#endif //MAIN_CPP_MESSAGE_H
