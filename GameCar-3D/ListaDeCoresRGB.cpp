#include "ListaDeCoresRGB.h"

//https://community.khronos.org/t/color-tables/22518/5


float Cores[][3] = {
    
    0.439216,0.858824,0.576471,
    0.62352,0.372549,0.623529,
    0.647059,0.164706,0.164706,
    0.372549,0.623529,0.623529,
    1,0.498039,0,
    0.258824,0.258824,0.435294,
    0.184314,0.309804,0.184314,
    0.309804,0.309804,0.184314,
    0.6,0.196078,0.8,
    0.419608,0.137255,0.556863,
    0.184314,0.309804,0.309804,
    0.184314,0.309804,0.309804,
    0.439216,0.576471,0.858824,
    0.556863,0.137255,0.137255,
    0.137255,0.556863,0.137255,
    0.8,0.498039,0.196078,
    0.858824,0.858824,0.439216,
    0.576471,0.858824,0.439216,
    0.309804,0.184314,0.184314,
    0.623529,0.623529,0.372549,
    0.74902,0.847059,0.847059,
    0.560784,0.560784,0.737255,
    0.196078,0.8,0.196078,
    0.556863,0.137255,0.419608,
    0.196078,0.8,0.6,
    0.196078,0.196078,0.8,
    0.419608,0.556863,0.137255,
    0.917647,0.917647,0.678431,
    0.576471,0.439216,0.858824,
    0.258824,0.435294,0.258824,
    0.498039,0,1,
    0.498039,1,0,
    0.439216,0.858824,0.858824,
    0.858824,0.439216,0.576471,
    0.184314,0.184314,0.309804,
    0.137255,0.137255,0.556863,
    0.137255,0.137255,0.556863,
    1,0.5,0,
    1,0.25,0,
    0.858824,0.439216,0.858824,
    0.560784,0.737255,0.560784,
    0.737255,0.560784,0.560784,
    0.917647,0.678431,0.917647,
    0.435294,0.258824,0.258824,
    0.137255,0.556863,0.419608,
    0.556863,0.419608,0.137255,
    0.196078,0.6,0.8,
    0,0.498039,1,
    0,1,0.498039,
    0.137255,0.419608,0.556863,
    0.858824,0.576471,0.439216,
    0.847059,0.74902,0.847059,
    0.678431,0.917647,0.917647,
    0.309804,0.184314,0.309804,
    0.8,0.196078,0.6,
    0.847059,0.847059,0.74902,
    0.6,0.8,0.196078,
    0.22,0.69,0.87,
    0.35,0.35,0.67,
    0.71,0.65,0.26,
    0.72,0.45,0.2,
    0.55,0.47,0.14,
    0.65,0.49,0.24,
    0.9,0.91,0.98,
    0.85,0.85,0.1,
    0.81,0.71,0.23,
    0.82,0.57,0.46,
    0.85,0.85,0.95,
    1,0.43,0.78,
    0.53,0.12,0.47,
    0.3,0.3,1,
    0.85,0.53,0.1,
    0.89,0.47,0.2,
    0.91,0.76,0.65,
    0.65,0.5,0.39,
    0.52,0.37,0.26,
    1,0.11,0.68,
    0.42,0.26,0.15,
    0.36,0.2,0.09,
    0.96,0.8,0.69,
    0.92,0.78,0.62,
    0,0,0.61,
    0.35,0.16,0.14,
    0.36,0.25,0.2,
    0.59,0.41,0.31,
    0.32,0.49,0.46,
    0.29,0.46,0.43,
    0.52,0.39,0.39,
    0.13,0.37,0.31,
    0.55,0.09,0.09,
    0.73,0.16,0.96,
    0.87,0.58,0.98,
    0.94,0.81,0.99,
    1,0,0,
    0,0,1,
    0,1,0,
    1,1,0,
    0,0,0,
    1,1,1,
    0.5,0.5,0.5
};

void defineCor(int c)
{
   // glColor3fv(Cores[c]);
    glColor3f(Cores[c][0], Cores[c][1], Cores[c][2]);
}

						
