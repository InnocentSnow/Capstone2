#ifndef _DRAW_UTIL_H

#include "util.h"
#include "score.h"

void setColor(Score::UserType type);
void drawSphere(const Position &p, float radius);
void drawCylinder(const Position &p1, const Position &p2, float r1, float r2);
void drawBody(float top, float bottom, float x1, float z1, float x2, float z2);

#endif