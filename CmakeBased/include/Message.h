//
// Created by user on 2021/7/15.
//

#ifndef MAIN_CPP_MESSAGE_H
#define MAIN_CPP_MESSAGE_H
#include <functional>
#include <cstdint>
#include <map>
#include <string>

using namespace std;
class Message {
    typedef uint8_t index;
    typedef function<void(void)> ftype;
    vector<ftype> func_v;
    map<string,index> func_mp;
public:

    void registerFunc(const string &s,ftype f);
    void callFunc(const string &s); //TODO:是否需要参数，int还是void?
    void announceView();
    void mainLoop();
};


#endif //MAIN_CPP_MESSAGE_H
