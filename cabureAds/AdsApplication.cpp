#include "AdsApplication.h"

using namespace Wt;

AdsApplication::AdsApplication (const WEnvironment& env)
    : WApplication(env){
  db_.open("sqlite3:db=ads.db");       
}

AdsApplication::~AdsApplication() {
    if(db_.is_open())
        db_.close();
}
