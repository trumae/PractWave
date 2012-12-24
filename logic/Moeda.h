#ifndef __CABURE__MOEDA__
#define __CABURE__MOEDA__

#include <string>
#include <Wt/WValidator>
#include <Wt/WRegExpValidator>

using namespace std;

class Moeda {
public:
    Moeda(string s);
    Moeda(long long s);

    long long valInt();
    string valStr();
    void setVal(long long val) {
        val_ = val;
    };


    static Wt::WValidator* newWValidator() {
        return new Wt::WRegExpValidator(
                   "[0-9]{1,3}\\.?([0-9]{3}\\.?)*(\\,[0-9]{2})?");
    }
private:
    long long val_;
};

#endif


