#pragma once

#include "CommonHeader.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

typedef struct Vector3 {
    float x;
    float y;
    float z;
} myVector3;

typedef struct Vector2 {
    float x;
    float y;
} myVector2;

class ObjLoader {
public:
    ObjLoader(const char *file);
    void init(const char *file);
    int faceN();
    float getData(int vector, int index, int axis);
    unsigned int getIndex(int vector, int i);
private:
    std::vector<myVector3> v; // 顶点
    std::vector<myVector3> vn;// 顶点法向量
    std::vector<myVector2> vt;// 顶点纹理坐标
    std::vector<unsigned int> f1;// 面的顶点
    std::vector<unsigned int> f2;// 面的顶点的纹理坐标
    std::vector<unsigned int> f3;// 面的顶点的法向量
    bool firstV = true;
};

float loadObj(const char *file, GLuint &regHex);
