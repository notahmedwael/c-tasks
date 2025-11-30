#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
using namespace std;

int main(){
    vector<string> insults = {
        "I'm not good enough\n",
        "I don't deserve anything\n",
        "I'm a fool\n",
        "I'm a disgrace\n",
        "I suck at everything\n",
        "I'm stupid\n"
    };
    while(1 > 0){
        for(auto &i: insults){
            cout << i;
            this_thread::sleep_for(chrono::seconds(1));
        }
    }

    return 0;
}