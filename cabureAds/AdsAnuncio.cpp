#include <Wt/WText>
#include <Wt/WBorder>
#include <Wt/WCssDecorationStyle>
#include <Wt/WAnchor>
#include <Wt/WContainerWidget>
#include <Wt/WBreak>
#include <Wt/WImage>
#include <Wt/WDialog>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WTextArea>
#include <Wt/WTemplate>
#include <Wt/WHBoxLayout>
#include <Wt/WMessageBox>
#include <stdlib.h>

#include "AdsAnuncio.h"
#include "AdsApplication.h"

using namespace Wt;

static std::vector<std::string> cores = {
  "bg-color-blueDark",
  "bg-color-greenDark",
  "bg-color-pinkDark",
  "bg-color-orangeDark",  
  "bg-color-purple"
};

AdsAnuncio::AdsAnuncio(WContainerWidget *parent) : WContainerWidget(parent) {
}

void AdsAnuncio::renderUI() {
    clear();
    //imagem
    WImage *img = new WImage(imagem_);
    img->setMinimumSize(WLength(90), WLength(70));
    img->setMaximumSize(WLength(90), WLength(70));
    img->resize(WLength(90), WLength(70));
    img->addStyleClass("place-right");
    img->setInline(true);

    //texto
    WText *texto = new WText(texto_);

    //layout
    WTemplate *container = new WTemplate(this);
    container->setTemplateText(std::string("<a href='") + link_  + "' target='_blank'>" 
			       "<div class='tile double " 
			       + cores[rand() % cores.size()] + 
			       "'>"
			       "   <div class='tile-content'>"
			       "     ${imagem}"
			       "     <h4>" + titulo_  + "</h4>"
			       "     <p>${texto}</p>"
			       "   </div>"
			       "   <div class='brand'>"
			       "     <span class='name'>@Publicidade</span> "
			       "   </div>"
			       "</div></a>", Wt::XHTMLUnsafeText);
    container->bindWidget("imagem", img);
    container->bindWidget("texto", texto);
}

void AdsAnuncio::changeAtiva(){
  AdsApplication *app = AdsApplication::adsApplication();
  cppdb::session &db = app->db_;

  cppdb::result res = db <<
    "select ativo from anuncio where id = ?" << id;
  while(res.next()){
    res >> ativo_;
    if(ativo_ == 1) 
      db << "update anuncio set ativo = 0 where id = ?" << id << cppdb::exec;
    else
      db << "update anuncio set ativo = 1 where id = ?" << id << cppdb::exec;
  }
}

void AdsAnuncio::deletaAnuncio(){
  AdsApplication *app = AdsApplication::adsApplication();
  cppdb::session &db = app->db_;
  
  StandardButton
    result = WMessageBox::show("Confirma", "Deseja, realmente, deletar este anuncio? Voce deve atulizar a lista manualmente.",
			       Ok | Cancel);
  if(result == Ok) {
    db << "delete from anuncio where id = ?" << id << cppdb::exec;
  }
}

void AdsAnuncio::editarTitulo(){
    AdsApplication *app = AdsApplication::adsApplication();
    cppdb::session &db = app->db_;
    
    Wt::WDialog dialog("Editar Titulo");
    Wt::WPushButton *ok = new WPushButton("Ok");
    ok->setStyleClass("btn btn-primary");
    ok->clicked().connect(&dialog, &Wt::WDialog::accept);
    Wt::WPushButton *cancel = new WPushButton("Cancela");
    cancel->setStyleClass("btn");
    cancel->clicked().connect(&dialog, &Wt::WDialog::reject);

    WLineEdit *lTitulo = new WLineEdit(titulo_);

    WTemplate *t = new WTemplate(dialog.contents());
    t->setTemplateText("<form>"
		       " <fieldset>"
		       "  <legend>Titulo</legend>"
                       "  <table border=0>"
                       "   <tr><td>Titulo</td> <td>${titulo}</td></tr>"
                       "  </table>"
		       "  </fieldset>"
		       "  ${ok}${cancel}"			       
                       "</form>"
                       , XHTMLUnsafeText);
    t->bindWidget("titulo", lTitulo);
    t->bindWidget("ok", ok);
    t->bindWidget("cancel", cancel);
    if (dialog.exec() == Wt::WDialog::Accepted) {
       db << "update anuncio set titulo = ? where id = ?"
	  << lTitulo->text().narrow() << id << cppdb::exec;
       titulo_ = lTitulo->text().narrow();
    }
    renderUI();
}

void AdsAnuncio::editarURL(){
    AdsApplication *app = AdsApplication::adsApplication();
    cppdb::session &db = app->db_;
    
    Wt::WDialog dialog("Editar URL");
    Wt::WPushButton *ok = new WPushButton("Ok");
    ok->setStyleClass("btn btn-primary");
    ok->clicked().connect(&dialog, &Wt::WDialog::accept);
    Wt::WPushButton *cancel = new WPushButton("Cancela");
    cancel->setStyleClass("btn");
    cancel->clicked().connect(&dialog, &Wt::WDialog::reject);

    WLineEdit *lURL = new WLineEdit(link_);

    WTemplate *t = new WTemplate(dialog.contents());
    t->setTemplateText("<form>"
		       " <fieldset>"
		       "  <legend>URL</legend>"
                       "  <table border=0>"
                       "   <tr><td>URL</td> <td>${url}</td></tr>"
                       "  </table>"
		       "  </fieldset>"
		       "  ${ok}${cancel}"			       
                       "</form>"
                       , XHTMLUnsafeText);
    t->bindWidget("url", lURL);
    t->bindWidget("ok", ok);
    t->bindWidget("cancel", cancel);
    if (dialog.exec() == Wt::WDialog::Accepted) {
       db << "update anuncio set url = ? where id = ?"
	  << lURL->text().narrow() << id << cppdb::exec;
       link_ = lURL->text().narrow();
    }
    renderUI();
}

void AdsAnuncio::editarImagem(){
    AdsApplication *app = AdsApplication::adsApplication();
    cppdb::session &db = app->db_;
    
    Wt::WDialog dialog("Editar Nome da imagem");
    Wt::WPushButton *ok = new WPushButton("Ok");
    ok->setStyleClass("btn btn-primary");
    ok->clicked().connect(&dialog, &Wt::WDialog::accept);
    Wt::WPushButton *cancel = new WPushButton("Cancela");
    cancel->setStyleClass("btn");
    cancel->clicked().connect(&dialog, &Wt::WDialog::reject);

    WLineEdit *lImagem = new WLineEdit(imagem_);

    WTemplate *t = new WTemplate(dialog.contents());
    t->setTemplateText("<form>"
		       " <fieldset>"
		       "  <legend>Texto</legend>"
                       "  <table border=0>"
                       "   <tr><td>Texto</td> <td>${imagem}</td></tr>"
                       "  </table>"
		       "  </fieldset>"
		       "  ${ok}${cancel}"			       
                       "</form>"
                       , XHTMLUnsafeText);
    t->bindWidget("imagem", lImagem);
    t->bindWidget("ok", ok);
    t->bindWidget("cancel", cancel);
    if (dialog.exec() == Wt::WDialog::Accepted) {
       db << "update anuncio set imagem = ? where id = ?"
	  << lImagem->text().narrow() << id << cppdb::exec;
       imagem_ = lImagem->text().narrow();
    }
    renderUI();
}

void AdsAnuncio::editarTexto(){
    AdsApplication *app = AdsApplication::adsApplication();
    cppdb::session &db = app->db_;
    
    Wt::WDialog dialog("Editar Texto");
    Wt::WPushButton *ok = new WPushButton("Ok");
    ok->setStyleClass("btn btn-primary");
    ok->clicked().connect(&dialog, &Wt::WDialog::accept);
    Wt::WPushButton *cancel = new WPushButton("Cancela");
    cancel->setStyleClass("btn");
    cancel->clicked().connect(&dialog, &Wt::WDialog::reject);

    WTextArea *lTexto = new WTextArea(texto_);

    WTemplate *t = new WTemplate(dialog.contents());
    t->setTemplateText("<form>"
		       " <fieldset>"
		       "  <legend>Texto</legend>"
                       "  <table border=0>"
                       "   <tr><td>Texto</td> <td>${texto}</td></tr>"
                       "  </table>"
		       "  </fieldset>"
		       "  ${ok}${cancel}"			       
                       "</form>"
                       , XHTMLUnsafeText);
    t->bindWidget("texto", lTexto);
    t->bindWidget("ok", ok);
    t->bindWidget("cancel", cancel);
    if (dialog.exec() == Wt::WDialog::Accepted) {
       db << "update anuncio set texto = ? where id = ?"
	  << lTexto->text().narrow() << id << cppdb::exec;
       texto_ = lTexto->text().narrow();
    }
    renderUI();
}


