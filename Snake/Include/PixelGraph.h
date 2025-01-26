#pragma once

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <map>
#include <unordered_map>
#include <chrono>
#include <vector>
#include <list>
#include <conio.h>

namespace Key 
{
    const int ESCAPE = 27;
    const int ARROW_UP = 72;
    const int ARROW_DOWN = 80;
    const int ARROW_RIGHT = 77;
    const int ARROW_LEFT = 75;
    const int ENTER = 13;
    const int SHIFT = 16;
    const int CTRL = 17;
    const int TAB = 9;
    const int CAPSLOCK = 20;
    const int SPACE = 32;
    const int DELETE = 46;
}


const float PI = 3.1415926535f;
const float DEG_TO_RAD = PI / 180.0f;
const float RAD_TO_DEG = 180.0f / PI;

const int MAX_INT = 2147483647;
const int MIN_INT = -2147483647;

#include "nlohmann/json.hpp"
#include "Timer.h"
#include "Vector2.h"
#include "Font.h"
#include "Box.h"
#include "Color.h"
#include "Screen.h"
#include "Transform.h"
#include "Shape.h"
#include "Text.h"
#include "Polygon.h"
#include "Rectangle.h"
#include "Elpise.h"
