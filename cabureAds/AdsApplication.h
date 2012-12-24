#ifndef __ADS__APPLICATION__H__
#define __ADS__APPLICATION__H__

#include <Wt/WApplication>
#include <string>
#include <cppdb/frontend.h>

class AdsApplication : public Wt::WApplication {
public:
    AdsApplication(const Wt::WEnvironment& env);
    ~AdsApplication();

    static AdsApplication* adsApplication() {
        return (AdsApplication *) WApplication::instance();
    };

    cppdb::session db_;
};


#endif
