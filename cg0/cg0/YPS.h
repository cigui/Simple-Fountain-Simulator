#pragma once
#include "ParticleSystem.h"
#include "Loader.h"

/* 
 * YPS
 * Yet another Particle System.
 */

/* glut提供的实体对象 */
enum Shape {
     CONE,
     CUBE,
     SPHERE,
     TORUS
};

class YPS : public ParticleSystem {
public:
    YPS();
    void load(const char *file); // 加载obj模型
    virtual void update(float deltaT);
    virtual void render();
    virtual void emit(int numParticles);
    void pull(float dx);
    void spin();
    void addWays();
    void cutWays();
    void changeSize(float ds);
    void setLoadModel();
    void changeShape();
    glm::vec4 getCurrentColor();
private:
    std::vector<Particle> particles;       // 系统中的粒子
    int particleCnt; // 当前粒子数目

    unsigned int ways; // 喷泉发射路数

    float angle; // 用户拖动水柱的角度

    /* 用于实现旋转发射粒子的变量 */
    int timeCnt; // 记录时间，用于计算旋转角度
    float angularV; // 旋转角速度
    bool spinning; // 是否正在旋转发射粒子

    /* 用于实现粒子颜色渐变的变量 */
    bool rAdd;
    bool gAdd;
    bool bAdd;
    glm::vec4 currentColor; 

    float size; // 粒子尺寸

    bool loadModel; // 是否加载模型
    float modelSize; // 模型尺寸
    GLuint regHex; // 用于绘制模型

    Shape shape; // 控制粒子形状（使用glut提供的实体对象时）
protected:
    int                   maxParticles;    // 最大粒子数目
    glm::vec3             origin;          // 粒子发射源
    glm::vec3             force;           // 粒子运动受力
 };