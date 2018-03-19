#pragma once
#include "ParticleSystem.h"
#include "Loader.h"

/* 
 * YPS
 * Yet another Particle System.
 */

/* glut�ṩ��ʵ����� */
enum Shape {
     CONE,
     CUBE,
     SPHERE,
     TORUS
};

class YPS : public ParticleSystem {
public:
    YPS();
    void load(const char *file); // ����objģ��
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
    std::vector<Particle> particles;       // ϵͳ�е�����
    int particleCnt; // ��ǰ������Ŀ

    unsigned int ways; // ��Ȫ����·��

    float angle; // �û��϶�ˮ���ĽǶ�

    /* ����ʵ����ת�������ӵı��� */
    int timeCnt; // ��¼ʱ�䣬���ڼ�����ת�Ƕ�
    float angularV; // ��ת���ٶ�
    bool spinning; // �Ƿ�������ת��������

    /* ����ʵ��������ɫ����ı��� */
    bool rAdd;
    bool gAdd;
    bool bAdd;
    glm::vec4 currentColor; 

    float size; // ���ӳߴ�

    bool loadModel; // �Ƿ����ģ��
    float modelSize; // ģ�ͳߴ�
    GLuint regHex; // ���ڻ���ģ��

    Shape shape; // ����������״��ʹ��glut�ṩ��ʵ�����ʱ��
protected:
    int                   maxParticles;    // ���������Ŀ
    glm::vec3             origin;          // ���ӷ���Դ
    glm::vec3             force;           // �����˶�����
 };