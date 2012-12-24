#include <cppdb/frontend.h>
#include <vector>
#include "AdsWidgetBuilder.h"
#include "AdsAnuncio.h"

AdsWidget *AdsWidgetBuilder::createAdsWidget() {
    AdsWidget *w = new AdsWidget();
    static cppdb::session db;
    if(!db.is_open())
        db.open("sqlite3:db=ads.db");

    cppdb::result res = db << 
	    "select titulo, url, imagem, texto "
	    " from anuncio order by random() limit 6";
    while(res.next()){
       AdsAnuncio *anuncio = new AdsAnuncio(w);
       res >>  anuncio->titulo_ >> anuncio->link_ >>
	       anuncio->imagem_ >> anuncio->texto_;
       anuncio->renderUI();
    }
    return w;
}

