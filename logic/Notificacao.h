#ifndef __CABURE__NOTIFICACAO__
#define __CABURE__NOTIFICACAO__

#include <cppdb/frontend.h>
#include <string>
#include <vector>

class Notificacao {
  public:
     Notificacao(std::string to, std::string script);
     ~Notificacao();
     void send();
  private:
     std::string to_;
     std::string script_;
};


#endif


