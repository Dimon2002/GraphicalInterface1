#pragma once

#include "resource.h"
#include <iostream>
#include "string"

#define CHANGE_VIEW_STATE 1000
#define M_PI acos(-1.0)

using namespace std;

inline void DrawLine(const HDC&, const POINT&, const POINT&, const COLORREF&);
inline void DrawLine(const HDC&, const POINT&, const POINT&, INT, const COLORREF&);
inline void DrawCross(const HDC&, const COLORREF&);
inline void DrawClock(const HDC&, const COLORREF&);
