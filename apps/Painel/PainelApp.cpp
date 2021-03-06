#include <Wt/WTemplate>
#include <Wt/WPushButton>
#include <Wt/WAnchor>
#include <Wt/WImage>
#include <Wt/WText>
#include <boost/lexical_cast.hpp>
#include <algorithm>

#include "../../core/CabureApplication.h"
#include "../../logic/Clientes.h"
#include "../../logic/Fornecedores.h"
#include "../../logic/Contabilidade.h"
#include "../../logic/Moeda.h"
#include "PainelApp.h"

using namespace Wt;
using namespace std;

PainelApp::PainelApp(WContainerWidget *parent)
    : App(parent) {
  PAINEL = 1;
  
  CabureApplication *app = CabureApplication::cabureApplication();
  
  app->clientes_->getTodosClientes(clientes_);
  sort(clientes_.begin(), clientes_.end(),
       [](const Cliente &a, const Cliente &b) {
	 CabureApplication *app = CabureApplication::cabureApplication();
	 Contabilidade *contabilidade = app->contabilidade_;
	 return contabilidade->getSaldoContaFolha(a.idconta) >
	   contabilidade->getSaldoContaFolha(b.idconta);
       });
  
  app->fornecedores_->getTodosFornecedores(fornecedores_);
  sort(fornecedores_.begin(), fornecedores_.end(),
       [](const Fornecedor &a, const Fornecedor &b) {
	 
	 CabureApplication *app = CabureApplication::cabureApplication();
	 Contabilidade *contabilidade = app->contabilidade_;
	 return contabilidade->getSaldoContaFolha(a.idconta) >
	   contabilidade->getSaldoContaFolha(b.idconta);
       });
  
  init();
}

void PainelApp::init(){
  adicionaEstado(PAINEL, boost::bind(&PainelApp::Painel, this));

  adicionaTransicao(PAINEL, START, "back", 
		    boost::bind(&PainelApp::fazNada, this, nullptr),
		    boost::bind(&PainelApp::guardOk, this));  
  setEstado(PAINEL);
}

std::string PainelApp::getTitulo() {
    return tr("tile-dashboard").toUTF8();
}

Wt::WWidget *PainelApp::Painel() {
    CabureApplication *app = CabureApplication::cabureApplication();
    Contabilidade *contabilidade = app->contabilidade_;
    std::string tplInformacoes(
        std::string() + 
        "        <div class='grid'>"
        "          <div class='row'>"
        "             <div class='span12'>"
        "         <div class='tile double bg-color-blue'>"
        "           <div class='tile-content'>"
        "              <h2>" + tr("total-receivables").toUTF8() + "</h2>"
        "              <h2>R$ ${totalReceber}</h2>"
        "              <div class='brand'>"
        "                <span class='name'>" + tr("registered-customers").toUTF8() + "</span>"
        "                <span class='badge'>${numClientes}</span>"
        "              </div>"
        "           </div>"
        "         </div>"
        "         <div class='tile double bg-color-red'>"
        "           <div class='tile-content'>"
        "              <h2>" + tr("total-to-pay").toUTF8() + "</h2>"
        "              <h2>R$ ${totalPagar}</h2>"
        "              <div class='brand'>"
        "                <span class='name'>" + tr("registered-suppliers").toUTF8() + "</span>"
        "                <span class='badge'>${numFornecedores}</span>"
        "              </div>"
        "           </div>"
        "         </div>"
        "         </div>"
    );
    WTemplate *w;

    long long totalReceber = 0;
    long long totalPagar = 0;

    for(Cliente cli: clientes_) {
        totalReceber += contabilidade->getSaldoContaFolha(cli.idconta);
    }
    for(Fornecedor forn: fornecedores_) {
        totalPagar += contabilidade->getSaldoContaFolha(forn.idconta);
    }

    Moeda mtotalReceber(totalReceber);
    Moeda mtotalPagar(totalPagar);
    string textoNumClientes = boost::lexical_cast<string>(clientes_.size());
    string textoNumFornecedores = boost::lexical_cast<string>(fornecedores_.size());

    if(clientes_.size() == 0 && fornecedores_.size() == 0) {
        w = new WTemplate();
        w->setTemplateText(
            tplInformacoes
            , XHTMLUnsafeText);
        w->bindString("totalReceber", mtotalReceber.valStr());
        w->bindString("totalPagar", mtotalPagar.valStr());
        w->bindString("numClientes", textoNumClientes);
        w->bindString("numFornecedores", textoNumFornecedores);
    } else {
        WImage *chartDeve;
        WImage *chartDevo;
        bool complementarTplDeve = false;
        bool complementarTplDevo = false;

        w = new WTemplate();
        std::string tplPainel = tplInformacoes;
        tplPainel += "<div class=\"grid\">";
        if(clientes_.size() > 0)
            complementarTplDeve = contabilidade->getSaldoContaFolha(clientes_[0].idconta) != 0;
        if(fornecedores_.size() > 0)
            complementarTplDevo = contabilidade->getSaldoContaFolha(fornecedores_[0].idconta) != 0;
        if(complementarTplDeve) {
            tplPainel +=
                "<div class='row'>  <div class='span8'> ${chartDeve} </div> </div>";
            chartDeve = new WImage(getUrlGrafico(QUEMMEDEVE));
        }
        if(complementarTplDevo) {
            tplPainel +=
                "<div class='row'> <div class='span8'>  ${chartDevo} </div> </div>";
            chartDevo = new WImage(getUrlGrafico(PRAQUEMDEVO));
        }
        tplPainel += "</div>";
        w->setTemplateText(
            tplPainel
            , XHTMLUnsafeText);

        w->bindString("totalReceber", mtotalReceber.valStr());
        w->bindString("totalPagar", mtotalPagar.valStr());
        w->bindString("numClientes", textoNumClientes);
        w->bindString("numFornecedores", textoNumFornecedores);
        if(complementarTplDeve)  w->bindWidget("chartDeve", chartDeve);
        if(complementarTplDevo)  w->bindWidget("chartDevo", chartDevo);
    }
    return w;
}

std::string PainelApp::getUrlGrafico(TipoGrafico tipoGrafico) {
    CabureApplication *app = CabureApplication::cabureApplication();
    Contabilidade *contabilidade = app->contabilidade_;
    int sizecli = clientes_.size();
    int sizeforn = fornecedores_.size();
    int sum = 0;
    std::string url = "http://chart.apis.google.com/chart?chs=480x360&cht=p&chds=a";

    switch(tipoGrafico) {
    case QUEMMEDEVE:
        url += "&chco=224499";
        if(sizecli == 0) {
            url += "&chtt=" + tr("without-data").toUTF8();
            return url;
        }
        if(sizecli == 1) {
            url += "&chd=t:" + boost::lexical_cast<string>(contabilidade->getSaldoContaFolha(clientes_[0].idconta));
            url += "&chl=" + clientes_[0].nome;
        }
        if(sizecli == 2) {
            url += "&chd=t:" + boost::lexical_cast<string>(contabilidade->getSaldoContaFolha(clientes_[0].idconta))
                   + std::string(",")
                   + boost::lexical_cast<string>(contabilidade->getSaldoContaFolha(clientes_[1].idconta));
            url += "&chl=" + clientes_[0].nome + "|" + clientes_[1].nome;
        }
        if(sizecli == 3) {
            url += "&chd=t:" + boost::lexical_cast<string>(contabilidade->getSaldoContaFolha(clientes_[0].idconta))
                   + std::string(",")
                   + boost::lexical_cast<string>(contabilidade->getSaldoContaFolha(clientes_[1].idconta))
                   + std::string(",")
                   + boost::lexical_cast<string>(contabilidade->getSaldoContaFolha(clientes_[2].idconta));
            url += "&chl=" + clientes_[0].nome + "|" + clientes_[1].nome + "|" + clientes_[2].nome;
        }
        if(sizecli > 3) {
            for(unsigned int i = 3; i < clientes_.size(); i++) {
                sum += contabilidade->getSaldoContaFolha(clientes_[i].idconta);
            }
            url += "&chd=t:" + boost::lexical_cast<string>(contabilidade->getSaldoContaFolha(clientes_[0].idconta))
                   + std::string(",")
                   + boost::lexical_cast<string>(contabilidade->getSaldoContaFolha(clientes_[1].idconta))
                   + std::string(",")
                   + boost::lexical_cast<string>(contabilidade->getSaldoContaFolha(clientes_[2].idconta))
                   + std::string(",")
                   + boost::lexical_cast<string>(sum) ;
            url += "&chl=" + clientes_[0].nome + "|" + clientes_[1].nome + "|" + clientes_[2].nome + "|Outros";
        }
        url += "&chtt=" + tr("receivables").toUTF8();
        break;
    case PRAQUEMDEVO:
        url += "&chco=AA0033";
        if(sizeforn == 0) {
            url += "&chtt=" + tr("without-data").toUTF8();
            return url;
        }
        if(sizeforn == 1) {
            url += "&chd=t:" + boost::lexical_cast<string>(contabilidade->getSaldoContaFolha(fornecedores_[0].idconta));
            url += "&chl=" + fornecedores_[0].nome;
        }
        if(sizeforn == 2) {
            url += "&chd=t:" + boost::lexical_cast<string>(contabilidade->getSaldoContaFolha(fornecedores_[0].idconta))
                   + std::string(",")
                   + boost::lexical_cast<string>(contabilidade->getSaldoContaFolha(fornecedores_[1].idconta));
            url += "&chl=" + fornecedores_[0].nome + "|" + fornecedores_[1].nome;
        }
        if(sizeforn == 3) {
            url += "&chd=t:" + boost::lexical_cast<string>(contabilidade->getSaldoContaFolha(fornecedores_[0].idconta))
                   + std::string(",")
                   + boost::lexical_cast<string>(contabilidade->getSaldoContaFolha(fornecedores_[1].idconta))
                   + std::string(",")
                   + boost::lexical_cast<string>(contabilidade->getSaldoContaFolha(fornecedores_[2].idconta));
            url += "&chl=" + fornecedores_[0].nome + "|" + fornecedores_[1].nome + "|" + fornecedores_[2].nome;
        }
        if(sizeforn > 3) {
            for(unsigned int i = 3; i < fornecedores_.size(); i++) {
                sum += contabilidade->getSaldoContaFolha(fornecedores_[i].idconta);
            }
            url += "&chd=t:" + boost::lexical_cast<string>(contabilidade->getSaldoContaFolha(fornecedores_[0].idconta))
                   + std::string(",")
                   + boost::lexical_cast<string>(contabilidade->getSaldoContaFolha(fornecedores_[1].idconta))
                   + std::string(",")
                   + boost::lexical_cast<string>(contabilidade->getSaldoContaFolha(fornecedores_[2].idconta))
                   + std::string(",")
                   + boost::lexical_cast<string>(sum) ;
            url += "&chl=" + fornecedores_[0].nome + "|" + fornecedores_[1].nome + "|" + fornecedores_[2].nome + "|Outros";
        }
        url += "&chtt=" + tr("to-pay").toUTF8();
        break;
    }
    return url;
}

