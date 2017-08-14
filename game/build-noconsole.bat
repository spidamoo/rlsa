mingw32-g++.exe -std=c++11 -c -I../Eigen -I../rnslib/include -I../rnslib/windows_include -Iinclude -I../hge181/include  main.cpp src/Exception.cpp src/GUIInput.cpp src/GUIItem.cpp src/GUIWindow.cpp src/GUIWindowLogin.cpp src/VisualCharacter.cpp src/VisualEffect.cpp src/VisualGame.cpp src/VisualTools.cpp src/services/alert.cpp ../hge181/src/helpers/hgeanim.cpp ../hge181/src/helpers/hgefont.cpp ../hge181/src/helpers/hgerect.cpp ../hge181/src/helpers/hgesprite.cpp ../tinyxml/tinystr.cpp ../tinyxml/tinyxml.cpp ../tinyxml/tinyxmlerror.cpp ../tinyxml/tinyxmlparser.cpp
mingw32-g++.exe main.o Exception.o GUIInput.o GUIItem.o GUIWindow.o GUIWindowLogin.o VisualCharacter.o VisualEffect.o VisualGame.o VisualTools.o alert.o hgeanim.o hgefont.o hgerect.o hgesprite.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o -o manual-bin/rns.exe -lWS2_32 -mwindows ..\hge181\lib\gcc\libhge.a ../rnslib/librns.a