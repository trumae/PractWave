CC = g++  
CFLAGS = --std=c++0x -Wall -ggdb -O0
LFLAGS = -lwt -lwtext -lwthttp -lboost_signals -lboost_system -lcppdb -lcppdb_sqlite3
DEPS = logic/meusdados.h logic/Contabilidade.h logic/Clientes.h logic/Timeline.h logic/ContasBancarias.h logic/Moeda.h logic/Caixa.h logic/Fornecedores.h logic/Notificacao.h utils/pugixml/pugixml.hpp apps/Painel/PainelApp.h apps/Timeline/TimelineApp.h apps/Cliente/ClienteApp.h apps/App.h widgets/ContaWidget/ContaWidget.h widgets/ContaWidget/ItemContaWidget.h widgets/ContaCliente/ContaCliente.h widgets/AjusteContaWidget/AjusteContaWidget.h
# utils/pugixml/pugiconfig.hpp cabureAds/AdsWidget.h cabureAds/AdsWidgetBuilder.h cabureAds/AdsAnuncio.h 
OBJS = apps/Cliente/ClienteApp.o apps/Cliente/TileCliente.o widgets/ContaWidget/ContaWidget.o widgets/ContaWidget/ItemContaWidget.o widgets/ContaCliente/ContaCliente.o widgets/AjusteContaWidget/AjusteContaWidget.o caburewt.o CabureApplication.o CabureWidgetPrincipal.o CabureUserManager.o logic/meusdados.o logic/Contabilidade.o logic/Clientes.o logic/Timeline.o logic/ContasBancarias.o logic/Moeda.o logic/Caixa.o logic/Fornecedores.o logic/Notificacao.o apps/Painel/PainelApp.o apps/Painel/TilePainel.o apps/Timeline/TimelineApp.o apps/Timeline/TileTimeline.o
# utils/pugixml/pugixml.o cabureAds/AdsWidget.o cabureAds/AdsWidgetBuilder.o cabureAds/AdsAnuncio.o  

all: caburewt 

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

caburewt: $(OBJS) 
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LFLAGS)
	ctags *.cpp $(DEPS)
	@echo "========= SUCESSO ============"

doxygen: Doxyfile
	doxygen

clean:
	rm -f caburewt $(OBJS) *~ *.aux *.log *.tex *.dvi
	rm -rf html latex

ctags:
	ctags *.cpp $(DEPS)  


