#include <cppdb/frontend.h>
#include <vector>
#include "AdsWidgetBuilder.h"
#include "AdsAnuncio.h"
#include "AdsWidget.h"

AdsWidget *AdsWidgetBuilder::createAdsWidget() {
  AdsWidget *w = new AdsWidget();
  
  for(int i = 0; i < 6; i++){
    AdsAnuncio *anuncio = createAdsAnuncio();
    w->addWidget(anuncio);
  } 
  return w;
}

AdsAnuncio *AdsWidgetBuilder::createAdsAnuncio(){
  AdsAnuncio *anuncio = new AdsAnuncio();
  static cppdb::session db;
  if(!db.is_open())
    db.open("sqlite3:db=ads2.db");
  
  
  cppdb::result res = db << 
    "select titulo, url, imagem, texto "
    " from anuncio where ativo = 1 order by random() limit 1";
  while(res.next()){
    res >>  anuncio->titulo_ >> anuncio->link_ >>
      anuncio->imagem_ >> anuncio->texto_;
    anuncio->renderUI();
  }
  return anuncio;
}

