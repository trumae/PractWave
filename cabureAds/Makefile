CC = g++
CFLAGS = -Wall -ggdb -O0
#CFLAGS = -O3 
LFLAGS = -lwt -lwtext -lwthttp -lboost_signals -lboost_system -lcppdb -lcppdb_sqlite3
DEPS = AdsManage.h AdsWidget.h AdsApplication.h AdsEditor.h AdsAnuncio.h AdsTeste.h AdsWidgetBuilder.h
OBJS = AdsManage.o AdsWidget.o AdsApplication.o adswt.o AdsEditor.o AdsAnuncio.o AdsTeste.o AdsWidgetBuilder.o

all: adswt 

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

adswt: $(OBJS) 
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LFLAGS)
	ctags *.cpp $(DEPS)
	@echo "========= SUCESSO ============"

doxygen: Doxyfile
	doxygen

clean:
	rm -f adswt $(OBJS) *~ *.aux *.log *.tex *.dvi
	rm -rf html latex

ctags:
	ctags *.cpp $(DEPS)  


