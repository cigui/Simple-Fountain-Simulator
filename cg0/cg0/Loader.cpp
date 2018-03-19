#include "Loader.h"

static float minY, maxY;

ObjLoader::ObjLoader(const char *file) {
    init(file);
}

int ObjLoader::faceN() {
    return f1.size() / 3;
}

// 获取Loader中存储的顶点位置/纹理坐标/法向量数据
float ObjLoader::getData(int vector, int index, int axis) {
    index -= 1;
    if (vector == 0) {
        if (axis == 0) {
            return v[index].x;
        }
        else if (axis == 1) {
            return v[index].y;
        }
        else {
            return v[index].z;
        }
    }
    else if (vector == 1) {
        if (axis == 0) {
            return vn[index].x;
        }
        else if (axis == 1) {
            return vn[index].y;
        }
        else {
            return vn[index].z;
        }
    }
    else if (vector == 2) {
        if (axis == 0) {
            return vt[index].x;
        }
        else if (axis == 1) {
            return vt[index].y;
        }
    }
}

// 获取Loader中某个面的某个点的顶点位置/纹理坐标/法向量的索引
unsigned int ObjLoader::getIndex(int vector, int i) {
    if (vector == 0) {
        return f1[i];
    }
    else if (vector == 1) {
        return f3[i];
    }
    else if (vector == 2) {
        return f2[i];
    }
}

void ObjLoader::init(const char *file) {
    std::ifstream in(file);
    char buf[300];
    if (!in) {
        printf("Loader Error: Unable to open file '%s' !\n", file);
        exit(0);
    }
    // 逐行分析文件
    while (in.getline(buf, 300)) {
        std::string str(buf);
        std::stringstream ss(str);
        std::string header;
        ss >> header;
        // 顶点位置数据
        if (header == "v") {
            myVector3 tmp;
            ss >> tmp.x;
            ss >> tmp.y;
            ss >> tmp.z;
            if (firstV) {
                firstV = false;
                minY = tmp.y;
                maxY = tmp.y;
            }
            if (tmp.y < minY) {
                minY = tmp.y;
            }
            if (tmp.y > maxY) {
                maxY = tmp.y;
            }
            v.push_back(tmp);
        }
        // 顶点纹理坐标数据
        else if (header == "vt") {
            myVector2 tmp;
            ss >> tmp.x;
            ss >> tmp.y;
            vt.push_back(tmp);
        }
        // 顶点法向量数据
        else if (header == "vn") {
            myVector3 tmp;
            ss >> tmp.x;
            ss >> tmp.y;
            ss >> tmp.z;
            vn.push_back(tmp);
        }
        // 面的顶点索引（要求模型中的面为三角形）
        else if (header == "f") {
            std::string p1;
            std::string p2;
            std::string p3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            ss >> p1;
            ss >> p2;
            ss >> p3;
            sscanf_s(p1.c_str(), "%d/%d/%d", &vertexIndex[0], &uvIndex[0], &normalIndex[0]);
            sscanf_s(p2.c_str(), "%d/%d/%d", &vertexIndex[1], &uvIndex[1], &normalIndex[1]);
            sscanf_s(p3.c_str(), "%d/%d/%d", &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            f1.push_back(vertexIndex[0]);
            f1.push_back(vertexIndex[1]);
            f1.push_back(vertexIndex[2]);
            f2.push_back(uvIndex[0]);
            f2.push_back(uvIndex[1]);
            f2.push_back(uvIndex[2]);
            f3.push_back(normalIndex[0]);
            f3.push_back(normalIndex[1]);
            f3.push_back(normalIndex[2]);
        }
    }
}

// 加载模型，并且注册一个callList到regHex上供后续渲染使用
float loadObj(const char *file, GLuint &regHex) {
    ObjLoader obj(file);
    int fn = obj.faceN();
    regHex = glGenLists(1);
    glNewList(regHex, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < fn; i++) {
        float x, y, z;
        for (int k = 0; k < 3; k++) {
            x = obj.getData(1, obj.getIndex(1, i * 3 + k), 0);
            y = obj.getData(1, obj.getIndex(1, i * 3 + k), 1);
            z = obj.getData(1, obj.getIndex(1, i * 3 + k), 2);
            glNormal3f(x, y, z);

            x = obj.getData(2, obj.getIndex(2, i * 3 + k), 0);
            y = obj.getData(2, obj.getIndex(2, i * 3 + k), 1);
            glTexCoord2f(x, y);

            x = obj.getData(0, obj.getIndex(0, i * 3 + k), 0);
            y = obj.getData(0, obj.getIndex(0, i * 3 + k), 1);
            z = obj.getData(0, obj.getIndex(0, i * 3 + k), 2);
            glVertex3f(x, y, z);
        }
    }
    glEnd();
    glEndList();
    return (maxY - minY); // 返回模型顶点中最大y值与最小y值的差表示模型尺寸
}