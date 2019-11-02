#ifndef COMMON_H
#define COMMON_H

#include <Angel.h>
#include <vector>
using std::vector;

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

extern vector<point4> globalpoints;
extern vector<color4> globalcolors;
extern vector<color4> globalnormals;

#endif