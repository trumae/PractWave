#ifndef __ADS__EDITOR__H__
#define __ADS__EDITOR__H__

#include <Wt/WContainerWidget>

class AdsEditor: public Wt::WContainerWidget {
public:
    AdsEditor();
    AdsEditor(Wt::WContainerWidget *parent);
    void renderUI();
private:
    void novoAnuncio();
};

#endif

