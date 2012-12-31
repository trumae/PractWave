#ifndef __CLIENTE_APP__
#define __CLIENTE_APP__

#include <Wt/WContainerWidget>
#include "../App.h"
#include "../../logic/Clientes.h"

class ClienteApp : public App {
protected:
    virtual WWidget* getConteudo();
    virtual std::string getTitulo();

    void processInicial(std::string inicial);
    void processCliente(int id, std::string inicial);
public:
    ClienteApp(Wt::WContainerWidget *parent);
};

class TileCliente : public Wt::WContainerWidget {
public:
    TileCliente();
};

#endif
