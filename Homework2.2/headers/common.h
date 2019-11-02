#ifndef COMMON_H
#define COMMON_H

#include <Angel.h>
#include <vector>
using std::vector;
typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

extern vector<point4> oldglobalpoints;
extern vector<color4> oldglobalcolors;
extern vector<color4> oldglobalnormals;

#endif