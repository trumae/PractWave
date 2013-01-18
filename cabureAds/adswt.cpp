#include <stdlib.h>
#include <Wt/WServer>
#include <Wt/WText>
#include <Wt/WLoadingIndicator>
#include <Wt/WOverlayLoadingIndicator>

#include "AdsApplication.h"
#include "AdsManage.h"

using namespace Wt;
using namespace Wt::Dbo;

WApplication *createApplication(const WEnvironment& env){
  WApplication *app = new AdsApplication(env);
  app->setTitle("PractWave - Ads Manage");

  //inclue Meta 
  app->addMetaHeader("viewport", "width=device-width, initial-scale=1.0");
  app->addMetaHeader("description", "");
  app->addMetaHeader("author","trumae@gmail.com");

  //inclue CSS
  app->useStyleSheet("css/practwave.css");
  app->useStyleSheet("css/modern.css");
  app->useStyleSheet("css/modern-responsive.css");
  
  //loadinIndicator
  WApplication::instance()->styleSheet().addRule("body", "margin: 0px");
  WLoadingIndicator *loading = new Wt::WOverlayLoadingIndicator("loading");
  loading->setMessage("Carregando");
  app->setLoadingIndicator(loading);

  //cria Widget Principal
  new AdsManage(app->root());

  return app;
}

void init(){
  srand(time(0));
}

int main(int argc, char **argv){
  init();
  
  try{
    Wt::WServer server(argv[0]);
    
    server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
    server.addEntryPoint(Wt::Application, createApplication);
        
    if(server.start()){
      Wt::WServer::waitForShutdown();
      server.stop();
    }
  } catch(Wt::WServer::Exception& e){
    std::cerr << e.what() << std::endl;
  } catch(std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}



