#ifndef __ADS__MANAGE__H__
#define __ADS__MANAGE__H__

#include <Wt/WContainerWidget>
using namespace Wt;

class AdsManage : public Wt::WContainerWidget {
public:
   AdsManage(Wt::WContainerWidget *parent);   
private:
   void criaDB();
};

#endif
