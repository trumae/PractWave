#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "Moeda.h"

using namespace std;

Moeda::Moeda(string in) {
    string aux;
    string s = in;
    size_t posvirgula = s.find_last_of(",");
    if(posvirgula != s.size() - 3)
       s += "00";
   
	if(s.size() < 3)
	   s += "00";

	string::iterator it;
    for(it = s.begin(); it < s.end(); it++) {
        if(*it != '.' &&
                *it != ',')
            aux.push_back(*it);
    }
	stringstream ss;
	ss << aux;
	ss >> val_;
}

Moeda::Moeda(long long i) {
    val_ = i;
}

long long Moeda::valInt() {
    return val_;
}

string Moeda::valStr() {
    string ret;	
    ret = boost::lexical_cast<string>(val_);
	if(ret.size() == 1) 
		ret = "00" + ret;
	if(ret.size() == 2) 
		ret = "0" + ret;
	ret.insert(ret.size() - 2, ",");
	for(int i = ret.size() - 6; i > 0; i -= 3){
		ret.insert(i,".");
	}
	if(ret[0] == '-' &&
	   ret[1] == '.')
		ret.erase(1,1);
    return ret;
}



