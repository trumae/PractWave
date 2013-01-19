#ifndef __ADS__ANUNCIO__H__
#define __ADS__ANUNCIO__H__

#include <Wt/WContainerWidget>
#include <string>

class AdsAnuncio : public Wt::WContainerWidget {
   public:
      AdsAnuncio(){}
      AdsAnuncio(WContainerWidget *parent);
      void renderUI();
      void editarTitulo();
      void editarURL();
      void editarImagem();
      void editarTexto();

      void changeAtiva();
      void deletaAnuncio();
      
      int id;
      std::string link_;
      std::string titulo_;
      std::string texto_;
      std::string imagem_;
      int ativo_;
};

#endif

