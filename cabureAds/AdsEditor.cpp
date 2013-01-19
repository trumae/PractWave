#include <Wt/WText>
#include <Wt/WTemplate>
#include <Wt/WPushButton>
#include <Wt/WCheckBox>
#include <Wt/WBreak>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WMessageBox>
#include <Wt/WDialog>
#include <Wt/WLineEdit>
#include <cppdb/frontend.h>
#include "AdsEditor.h"
#include "AdsAnuncio.h"
#include "AdsApplication.h"

using namespace Wt;

AdsEditor::AdsEditor() {
  renderUI();
}

AdsEditor::AdsEditor(WContainerWidget *parent):
  WContainerWidget(parent) {
  renderUI();
}

void AdsEditor::renderUI() {
  AdsApplication *app = AdsApplication::adsApplication();
  cppdb::session &db = app->db_;
  
  clear();
  WPushButton *btn = new WPushButton("Criar Anuncio", this);
  btn->clicked().connect(this, &AdsEditor::novoAnuncio);
  WPushButton *update = new WPushButton("Atualiza", this);
  update->clicked().connect(this, &AdsEditor::renderUI);
  new WBreak(this);
  new WBreak(this);
  new WText("<h3>Lista de Anuncios</h3>",this);
  
  Wt::WContainerWidget *w = new Wt::WContainerWidget(this);
  w->resize(600, WLength::Auto);
  WVBoxLayout *layout = new Wt::WVBoxLayout();
  cppdb::result res = db <<
    "select id, titulo, url, imagem, texto, ativo "
    " from anuncio";
  while(res.next()) {
    WContainerWidget *cont = new WContainerWidget();
    AdsAnuncio *anuncio = new AdsAnuncio(cont);
    
    res >>  anuncio->id >> anuncio->titulo_ >> anuncio->link_ >>
      anuncio->imagem_ >> anuncio->texto_ >> anuncio->ativo_;    
    
    WPushButton *tituloBtn = new WPushButton("Titulo",cont);
    tituloBtn->clicked().connect(anuncio, &AdsAnuncio::editarTitulo);
    
    WPushButton *urlBtn = new WPushButton("URL",cont);
    urlBtn->clicked().connect(anuncio, &AdsAnuncio::editarURL);
    
    WPushButton *imagemBtn = new WPushButton("Imagem",cont);
    imagemBtn->clicked().connect(anuncio, &AdsAnuncio::editarImagem);
    
    WPushButton *textoBtn = new WPushButton("Texto",cont);
    textoBtn->clicked().connect(anuncio, &AdsAnuncio::editarTexto);
    
    WCheckBox *ativoCheck = new Wt::WCheckBox("Ativo",cont);
    if(anuncio->ativo_ == 1)
      ativoCheck->setChecked(true);
    else 
      ativoCheck->setChecked(false);
    ativoCheck->changed().connect(anuncio, &AdsAnuncio::changeAtiva);

    WPushButton *deletaBtn = new WPushButton("Deleta",cont);
    deletaBtn->clicked().connect(anuncio, &AdsAnuncio::deletaAnuncio);

    anuncio->renderUI();
    layout->addWidget(cont);
  }
  w->setLayout(layout);
}

void AdsEditor::novoAnuncio() {
  AdsApplication *app = AdsApplication::adsApplication();
  cppdb::session &db = app->db_;
    
  Wt::WDialog dialog("novo Anuncio");
  Wt::WPushButton *ok = new WPushButton("Ok");
  ok->setStyleClass("btn btn-primary");
  ok->clicked().connect(&dialog, &Wt::WDialog::accept);
  Wt::WPushButton *cancel = new WPushButton("Cancela");
  cancel->setStyleClass("btn");
  cancel->clicked().connect(&dialog, &Wt::WDialog::reject);

  WLineEdit *lTitulo = new WLineEdit();
  WLineEdit *lURL = new WLineEdit();
  WLineEdit *lTexto = new WLineEdit();

  WTemplate *t = new WTemplate(dialog.contents());
  t->setTemplateText("<form>"
		     " <fieldset>"
		     "  <legend>Dados do anuncio</legend>"
		     "  <table border=0>"
		     "   <tr><td>Titulo</td> <td>${titulo}</td></tr>"
		     "   <tr><td>URL</td>    <td>${url}</td></tr>"
		     "   <tr><td>Texto</td>  <td>${texto}</td></tr>"
		     "  </table>"
		     "  </fieldset>"
		     "  ${ok}${cancel}"			       
		     "</form>"
		     , XHTMLUnsafeText);
  t->bindWidget("titulo", lTitulo);
  t->bindWidget("url", lURL);
  t->bindWidget("texto", lTexto);
  t->bindWidget("ok", ok);
  t->bindWidget("cancel", cancel);
  if (dialog.exec() == Wt::WDialog::Accepted) {
    db << "insert into anuncio (id, titulo, url, texto, imagem) values "
      "(NULL, ?,?,?, '/img/cabure.png')" 
       << lTitulo->text().narrow() << lURL->text().narrow()
       << lTexto->text().narrow() << cppdb::exec;
  }
  renderUI();
}



