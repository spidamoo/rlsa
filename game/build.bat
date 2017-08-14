
del -f main.o GUIInput.o GUIButton.o GUIItem.o GUIWindow.o GUIWindowLogin.o Character.o Game.o Tools.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o manual-bin\rlsa.exe
mingw32-g++ -std=c++11 -c -I../Eigen -Iinclude -I../irrlicht-1.8.4/include  main.cpp src/GUIInput.cpp src/GUIButton.cpp src/GUIItem.cpp src/GUIWindow.cpp src/GUIWindowMainMenu.cpp src/Character.cpp src/Tools.cpp src/Game.cpp ../tinyxml/tinystr.cpp ../tinyxml/tinyxml.cpp ../tinyxml/tinyxmlerror.cpp ../tinyxml/tinyxmlparser.cpp
mingw32-g++ main.o GUIInput.o GUIButton.o GUIItem.o GUIWindow.o GUIWindowMainMenu.o Character.o Tools.o Game.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o -o manual-bin/rlsa.exe -lWS2_32 ../irrlicht-1.8.4/lib/Win32-gcc/libIrrlicht.a
