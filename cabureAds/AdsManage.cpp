#include <Wt/WTabWidget>
#include <Wt/WText>
#include <cppdb/frontend.h>

#include "AdsManage.h"
#include "AdsEditor.h"
#include "AdsAnuncio.h"
#include "AdsTeste.h"
#include "AdsApplication.h"

AdsManage::AdsManage(Wt::WContainerWidget *parent) :
    Wt::WContainerWidget(parent) {
    criaDB();
    Wt::WTabWidget *abas = new Wt::WTabWidget(this);
    abas->addTab(new AdsEditor(), "Editor");
    abas->addTab(new AdsTeste(), "Teste");
    abas->addTab(new Wt::WText("Relatorios"), "Relatorios");
}

void AdsManage::criaDB(){
    AdsApplication *app = AdsApplication::adsApplication(); 
    cppdb::session &db = app->db_;
    
    try{
      db << "create table if not exists anuncio ( "
	"   id integer  not null primary key autoincrement, "
	"   titulo text not null, "
	"   url text not null, "
	"   imagem text, "
	"   texto text not null, "
	"   prio int not null default 10, "
	"   ate datetime default(date('now', '+1 month')), "
	"   grupo text) "  << cppdb::exec;	  
      
      db << "alter table anuncio add column ativo integer not null default '1'" << cppdb::exec;
    }catch(...) {
      std::cerr << "Excecao ao criar/modificar banco de dados" << std::endl;
    }
}

