include_directories(${PRACTWAVE2_SOURCE_DIR}/widgets/ContaWidget)
link_directories(${PRACTWAVVE2_SOURCE_DIR}/widgets/ContaWidget)
include_directories(${PRACTWAVE2_SOURCE_DIR}/widgets/AjusteContaWidget)
link_directories(${PRACTWAVVE2_SOURCE_DIR}/widgets/AjusteContaWidget)
include_directories(${PRACTWAVE2_SOURCE_DIR}/widgets/ContaCliente)
link_directories(${PRACTWAVVE2_SOURCE_DIR}/widgets/ContaCliente)
include_directories(${PRACTWAVE2_SOURCE_DIR}/widgets/ContaFornecedor)
link_directories(${PRACTWAVVE2_SOURCE_DIR}/widgets/ContaFornecedor)
include_directories(${PRACTWAVE2_SOURCE_DIR}/widgets/ContaWidget)
link_directories(${PRACTWAVVE2_SOURCE_DIR}/widgets/ContaWidget)

include_directories(${PRACTWAVE2_SOURCE_DIR}/logic)
link_directories(${PRACTWAVVE2_SOURCE_DIR}/logic)

include_directories(${PRACTWAVE2_SOURCE_DIR}/apps/Caixa)
link_directories(${PRACTWAVVE2_SOURCE_DIR}/apps/Caixa)
include_directories(${PRACTWAVE2_SOURCE_DIR}/apps/Calculadora)
link_directories(${PRACTWAVVE2_SOURCE_DIR}/apps/Calculadora)
include_directories(${PRACTWAVE2_SOURCE_DIR}/apps/Cliente)
link_directories(${PRACTWAVVE2_SOURCE_DIR}/apps/Cliente)
include_directories(${PRACTWAVE2_SOURCE_DIR}/apps/ContaBancaria)
link_directories(${PRACTWAVVE2_SOURCE_DIR}/apps/ContaBancaria)
include_directories(${PRACTWAVE2_SOURCE_DIR}/apps/Fornecedor)
link_directories(${PRACTWAVVE2_SOURCE_DIR}/apps/Fornecedor)
include_directories(${PRACTWAVE2_SOURCE_DIR}/apps/Painel)
link_directories(${PRACTWAVVE2_SOURCE_DIR}/apps/Painel)
include_directories(${PRACTWAVE2_SOURCE_DIR}/apps/Timeline)
link_directories(${PRACTWAVVE2_SOURCE_DIR}/apps/Timeline)


add_executable (practwave2 CabureApplication.cpp  CabureUserManager.cpp  CabureWidgetPrincipal.cpp  caburewt.cpp)
#geral
target_link_libraries(practwave2 m)
target_link_libraries(practwave2 wt)
target_link_libraries(practwave2 wtext)
target_link_libraries(practwave2 wthttp)
target_link_libraries(practwave2 boost_signals)
target_link_libraries(practwave2 boost_system)
target_link_libraries(practwave2 cppdb)
target_link_libraries(practwave2 cppdb_sqlite3)

target_link_libraries(practwave2 Logic)

#widgets
target_link_libraries(practwave2 WidgetContaWidget)
target_link_libraries(practwave2 WidgetAjustaContaWidget)
target_link_libraries(practwave2 WidgetContaCliente)
target_link_libraries(practwave2 WidgetContaFornecedor)

#aplicativos core
target_link_libraries(practwave2 PainelApp)
target_link_libraries(practwave2 TimelineApp)
target_link_libraries(practwave2 ClienteApp WidgetContaWidget WidgetContaCliente WidgetAjustaContaWidget)
target_link_libraries(practwave2 FornecedorApp  WidgetContaWidget WidgetContaFornecedor WidgetAjustaContaWidget)
target_link_libraries(practwave2 ContaBancariaApp)
target_link_libraries(practwave2 CaixaApp)

# aplicativos
target_link_libraries(practwave2 CalculadoraApp)

