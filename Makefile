CC = g++  
CFLAGS = --std=c++0x -Wall -ggdb -O0
LFLAGS = -lwt -lwtext -lwthttp -lboost_signals -lboost_system -lcppdb -lcppdb_sqlite3
DEPS = logic/meusdados.h logic/Contabilidade.h logic/Clientes.h logic/Timeline.h logic/ContasBancarias.h logic/Moeda.h logic/Caixa.h cabureAds/AdsWidget.h cabureAds/AdsWidgetBuilder.h cabureAds/AdsAnuncio.h logic/Fornecedores.h logic/Notificacao.h utils/pugixml/pugixml.hpp utils/pugixml/pugiconfig.hpp apps/Painel/Painel.h apps/Timeline/Timeline.h 
OBJS = caburewt.o CabureApplication.o CabureWidgetPrincipal.o CabureUserManager.o logic/meusdados.o logic/Contabilidade.o logic/Clientes.o logic/Timeline.o logic/ContasBancarias.o logic/Moeda.o logic/Caixa.o cabureAds/AdsWidget.o cabureAds/AdsWidgetBuilder.o cabureAds/AdsAnuncio.o logic/Fornecedores.o logic/Notificacao.o utils/pugixml/pugixml.o apps/Painel/Painel.o apps/Painel/TilePainel.o apps/Timeline/Timeline.o apps/Timeline/TileTimeline.o

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


