add_executable (practwave3 CabureApplication.cpp  CabureUserManager.cpp  CabureWidgetPrincipal.cpp  caburewt.cpp)


TARGET_LINK_LIBRARIES(practwave3
	Wt::Wt
	Wt::HTTP
	core
	${Boost_LIBRARIES}
	m
)


#target_link_libraries(practwave2 cppdb)
#target_link_libraries(practwave2 cppdb_sqlite3)

##widgets
#target_link_libraries(practwave2 WidgetContaWidget)
#target_link_libraries(practwave2 WidgetAjustaContaWidget)
#target_link_libraries(practwave2 WidgetContaCliente)
#target_link_libraries(practwave2 WidgetContaFornecedor)

##Ads
#target_link_libraries(practwave2 Ads)

##aplicativos core
#target_link_libraries(practwave2 PainelApp)
#target_link_libraries(practwave2 TimelineApp)
#target_link_libraries(practwave2 ClienteApp WidgetContaWidget WidgetContaCliente WidgetAjustaContaWidget)
#target_link_libraries(practwave2 FornecedorApp  WidgetContaWidget WidgetContaFornecedor WidgetAjustaContaWidget)
#target_link_libraries(practwave2 ContaBancariaApp)
#target_link_libraries(practwave2 CaixaApp)

## aplicativos
#target_link_libraries(practwave2 CalculadoraApp)
#target_link_libraries(practwave2 LojaApp)

