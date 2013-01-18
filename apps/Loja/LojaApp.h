#ifndef __LOJA_APP__
#define __LOJA_APP__

#include <Wt/WContainerWidget>
#include <string>

#include "../App.h"

class LojaApp : public App {
 protected:
  int LOJA;

  virtual std::string getTitulo();
  virtual void init();
  Wt::WWidget *loja();
 public:
  LojaApp(Wt::WContainerWidget *parent);
};

class TileLoja : public Wt::WContainerWidget {
  public:
    TileLoja();
};

#endif
