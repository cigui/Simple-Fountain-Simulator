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
    std::vector<myVector3> v; // ����
    std::vector<myVector3> vn;// ���㷨����
    std::vector<myVector2> vt;// ������������
    std::vector<unsigned int> f1;// ��Ķ���
    std::vector<unsigned int> f2;// ��Ķ������������
    std::vector<unsigned int> f3;// ��Ķ���ķ�����
    bool firstV = true;
};

float loadObj(const char *file, GLuint &regHex);
