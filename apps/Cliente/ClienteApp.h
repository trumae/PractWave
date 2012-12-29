#ifndef __CLIENTE_APP__
#define __CLIENTE_APP__

#include <Wt/WContainerWidget>
#include "../../logic/Clientes.h"

class ClienteApp : public Wt::WContainerWidget {
 public:
  ClienteApp(Wt::WContainerWidget *parent);
  void viewHome();
  void viewZero(){};
};

class TileCliente : public Wt::WContainerWidget {
  public:
    TileCliente();
};

#endif
