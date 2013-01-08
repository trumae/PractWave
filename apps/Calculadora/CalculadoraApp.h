#ifndef __CALCULADORA_APP__
#define __CALCULADORA_APP__

#include <Wt/WContainerWidget>
#include <string>

#include "../App.h"

class CalculadoraApp : public App {
 protected:
  const int CALCULADORA = 1;

  virtual std::string getTitulo();
  virtual void init();
  Wt::WWidget *calculadora();
 public:
  CalculadoraApp(Wt::WContainerWidget *parent);
};

class TileCalculadora : public Wt::WContainerWidget {
  public:
    TileCalculadora();
};

#endif
