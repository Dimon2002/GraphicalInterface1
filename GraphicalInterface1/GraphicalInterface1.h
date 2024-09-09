#pragma once

#include "resource.h"
#include <iostream>
#include "string"

#define CHANGE_VIEW_STATE 1000
#define M_PI acos(-1.0)

using namespace std;

inline void DrawLine(HDC, POINT, POINT);
inline void DrawLine(HDC, POINT, POINT, INT);
inline void DrawCross(HDC);
inline void DrawClock(HDC);