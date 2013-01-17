add_library(Ads AdsAnuncio AdsWidgetBuilder AdsWidget)

add_executable(adswt AdsApplication AdsEditor AdsManage AdsTeste adswt)
target_link_libraries(adswt m)
target_link_libraries(adswt wt)
target_link_libraries(adswt wtext)
target_link_libraries(adswt wthttp)
target_link_libraries(adswt boost_signals)
target_link_libraries(adswt boost_system)
target_link_libraries(adswt cppdb)
target_link_libraries(adswt cppdb_sqlite3)

target_link_libraries(adswt Ads)