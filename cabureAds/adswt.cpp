#include <stdlib.h>
#include <Wt/WServer>
#include <Wt/WText>

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
  app->useStyleSheet("css/bootstrap.min.css"); 
  app->useStyleSheet("css/bootstrap-responsive.css"); 
  app->useStyleSheet("css/media-fluid.css"); 
  app->useStyleSheet("css/cabure.css"); 

  //carrega Javascripts
  app->require("lib/jquery-1.7.2.min.js");
  app->require("lib/bootstrap.min.js");

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



