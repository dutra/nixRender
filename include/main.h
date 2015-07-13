#include "types.h"

#define WINDOW_WIDTH 1200u
#define WINDOW_HEIGHT 900u
#define FOV 65.0f
const uint32 FRAMES_COUNTER = 3000;
#define GLSL_VERSION 400

#define CHUNK_SIZE_X 1
#define CHUNK_SIZE_Y 1
#define CHUNK_SIZE_Z 1
#define CHUNK_VOLUME_SIZE CHUNK_SIZE_X*CHUNK_SIZE_Y*CHUNK_SIZE_Z

const float Rg = 6360.0;
const float Rt = 6420.0;
const float RL = 6421.0;

const int TRANSMITTANCE_W = 256;
const int TRANSMITTANCE_H = 64;

const int SKY_W = 64;
const int SKY_H = 16;

const int RES_R = 32;
const int RES_MU = 128;
const int RES_MU_S = 32;
const int RES_NU = 8;
