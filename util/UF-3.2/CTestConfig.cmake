set(CTEST_PROJECT_NAME "UF")
#set(CTEST_NIGHTLY_START_TIME "00:00:00 EST")
set(CTEST_NIGHTLY_START_TIME "12:00:00 UTC")

if( NOT DEFINED CTEST_DROP_METHOD )
  set(CTEST_DROP_METHOD "http")
endif( NOT DEFINED CTEST_DROP_METHOD )
  
if( CTEST_DROP_METHOD STREQUAL "http" )
  set(CTEST_DROP_METHOD "http")
  set(CTEST_DROP_SITE "cdash.acfr.usyd.edu.au")
  set(CTEST_DROP_LOCATION "/submit.php?project=UF")
  #  set(CTEST_DROP_LOCATION "/CDash/submit.php?project=UF")
  set(CTEST_DROP_SITE_CDASH TRUE)
endif( CTEST_DROP_METHOD STREQUAL "http" )
  
#set(CTEST_DROP_METHOD "http")
#set(CTEST_DROP_SITE "bamboo.acfr.usyd.edu.au")
#set(CTEST_DROP_LOCATION "/CDash/submit.php?project=UF")
#set(CTEST_DROP_SITE_CDASH TRUE)