#ifndef ERROR_H
#define ERROR_H

#include "color.h"
#include <iostream>
#include <GL/glew.h>
#include <cassert>
#include <string>

void _glCheckError(const char *file, int line);

#define glCheckError() _glCheckError(__FILE__, __LINE__);
#endif