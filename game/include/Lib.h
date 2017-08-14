#ifndef LIB_H_INCLUDED
#define LIB_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <unordered_map>
#include <queue>
#include <string>
#include <vector>
#include <stack>
#include <forward_list>
#include <windows.h>
#include <chrono>
#include <functional>

#include <irrlicht.h>
#include "../../tinyxml/tinyxml.h"

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#define M_PI_2  1.57079632679489661923f
#define M_PI_4  0.785398163397448309616f
#define M_1_PI  0.318309886183790671538f
#define M_2_PI  0.636619772367581343076f
#endif
#define M_PI_8  0.39269908169f

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Game;
class Character;
class Animation;

class Profiler;

class GUIWindow;
class GUIWindowMainMenu;
class GUIItem;
class GUIInput;
class GUIButton;

#include "Game.h"
#include "Character.h"

#include "Tools.h"

#include "GUIItem.h"
#include "GUIInput.h"
#include "GUIButton.h"
#include "GUIWindow.h"
#include "GUIWindowMainMenu.h"

#endif // LIB_H_INCLUDED
