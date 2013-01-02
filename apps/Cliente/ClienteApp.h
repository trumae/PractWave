#ifndef __CLIENTE_APP__
#define __CLIENTE_APP__

#include <Wt/WContainerWidget>
#include <Wt/WStackedWidget>
#include "../App.h"
#include "../../logic/Clientes.h"

class ClienteApp : public App {
private:
	Wt::WStackedWidget *stack;
protected:
    virtual WWidget* getConteudo();
    virtual std::string getTitulo();

    void processInicial(std::string inicial);
    void processCliente(int id, int idconta, std::string inicial);

	void showDados();
	void showConta();
public:
    ClienteApp(Wt::WContainerWidget *parent);
};

class TileCliente : public Wt::WContainerWidget {
public:
    TileCliente();
};

#endif
