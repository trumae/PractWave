#include <stdlib.h>
#include <Wt/WServer>
#include <Wt/Auth/AuthService>
#include <Wt/Auth/GoogleService>
#include <Wt/WLoadingIndicator>
#include <Wt/WOverlayLoadingIndicator>

#include "CabureUserManager.h"
#include "CabureWidgetPrincipal.h"
#include "CabureApplication.h"

using namespace Wt;
using namespace Wt::Dbo;

WApplication *createApplication(const WEnvironment& env) {
    CabureApplication *app = new CabureApplication(env);
    app->setTitle("PractWave");
    //app->setCssTheme("Polished");
    app->messageResourceBundle().use(app->appRoot() + "www/apps/Loja/loja");

    //inclue Meta
    app->addMetaHeader("viewport", "width=device-width, initial-scale=1.0");
    app->addMetaHeader("description", "");
    app->addMetaHeader("author","trumae@gmail.com");

    //inclue CSS
    app->useStyleSheet("css/practwave.css");
    app->useStyleSheet("css/modern.css");
    app->useStyleSheet("css/modern-responsive.css");

    //carrega Javascripts
    app->require("google_conversion.js");
    app->require("http://www.googleadservices.com/pagead/conversion.js");
    app->require("http://www.google-analytics.com/ga.js");
    app->require("http://ajax.googleapis.com/ajax/libs/jquery/1.8.3/jquery.min.js");
    app->require("javascript/tile-slider.js");
    app->require("javascript/practwave.js");


    //loadinIndicator
    WApplication::instance()->styleSheet().addRule("body", "margin: 0px");
    WLoadingIndicator *loading = new Wt::WOverlayLoadingIndicator("loading");
    loading->setMessage("Carregando");
    app->setLoadingIndicator(loading);

    //cria Widget Principal
    app->principal_ = new CabureWidgetPrincipal(app->root());

    return app;
}

void init() {
    srand(time(0));
}

int main(int argc, char **argv) {
    init();

    try {
        Wt::WServer server(argv[0]);

        server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
        server.addEntryPoint(Wt::Application, createApplication);

        if (Wt::Auth::FacebookService::configured()) {
            facebookService = new Wt::Auth::FacebookService(authServiceFacebook);
        }

        if (Wt::Auth::GoogleService::configured()) {
            googleService = new Wt::Auth::GoogleService(authServiceGoogle);
        }

        if(server.start()) {
            Wt::WServer::waitForShutdown();
            server.stop();
        }
    } catch(Wt::WServer::Exception& e) {
        std::cerr << e.what() << std::endl;
    } catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}



