#include <stdlib.h>
#include <stdio.h>
#include "Loader.h"
#include "Texture.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "YPS.h"

/* 摄像机 */
Camera camera;

/* 鼠标参数 */
bool leftMouseDown = false;
bool rightMouseDown = false;
glm::vec2 mouseCurrent = glm::vec2(0);
glm::vec2 mousePrevious = glm::vec2(0);
glm::vec2 mouseDelta = glm::vec2(0);

/* 绘制坐标轴 */
void doDrawAxis(float size);

/* 粒子系统 */
YPS ps;

/* 用于限制FPS的时间参数 */
int timenow = 0, timeprev = 0;

/* 纹理 */
unsigned int texture[7];

/* 是否打开地面灯光、是否绘制坐标轴、是否渲染背景 */
bool enableLight0 = true, drawAxis = false, renderScene = true;

/* 用于绘制场景（glCallList(regHex[i])） */
GLuint regHex[3] = { 0, 0 };

void init() {
    /* 光源设置 */
    glEnable(GL_LIGHTING);

    GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // 环境光
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 漫反射
    GLfloat specularLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };// 镜面光

    GLfloat lightPos0[] = { 0.0f, 0.0f, 0.0f, 1.0f };    // 光源位置
    GLfloat lightPos1[] = { 10.0f, 10.0f, 10.0f, 1.0f }; // 光源位置

    glEnable(GL_LIGHTING);                            // 启用光照
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);   // 设置环境光源
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);   // 设置漫反射光源
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight); // 设置镜面光源
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);     // 设置灯光位置
    glEnable(GL_LIGHT0);                              // 打开光源

    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);   // 设置环境光源
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);   // 设置漫反射光源
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight); // 设置镜面光源
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);     // 设置灯光位置
    glEnable(GL_LIGHT1);                              // 打开光源

    glEnable(GL_COLOR_MATERIAL);                      // 启用材质的颜色跟踪

    /* 材料设置 */
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientLight);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseLight);


    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 指定混合函数
    glEnable(GL_BLEND);

    /* 加载纹理 */
    loadTexture(texture[0], "Data/pool.jpg");           // 水池(?)纹理
    loadTexture(texture[1], "Data/ground.jpg");              // 地面纹理
    loadTexture(texture[2], "Data/bench.jpg");          // 长椅纹理
    loadTexture(texture[3], "Data/particle.jpg");               // 粒子纹理
    loadTexture(texture[4], "Data/img_test.png");         // 备用粒子纹理
    loadTexture(texture[5], "Data/Particle-Texture.png"); // 备用粒子纹理
    loadTexture(texture[6], "Data/particle.png");               // 备用粒子纹理
    glEnable(GL_TEXTURE_2D);

    /* 加载模型 */
    loadObj("Data/tube.obj", regHex[0]); // 水池(?)模型
    loadObj("Data/box.obj", regHex[1]);  // 地面模型
    loadObj("Data/bench.obj", regHex[2]);// 长椅模型
    ps.load("Data/particle.obj");        // 粒子模型
}

void doRenderScene() {
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_GEN_S); // 禁用自动生成纹理S坐标
    glDisable(GL_TEXTURE_GEN_T); // 禁用自动生成纹理T坐标

    /* 绘制水池 */
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    /* 
     * 下面这段代码是获取当前发射的粒子的颜色用于绘制水池，
     * 但事实上渲染效果并不好，所以注释掉。
     */
    //glm::vec4 color = ps->getCurrentColor();
    //if (color != glm::vec4(0)) {
    //    glColor4f(color.r, color.g, color.b, 1.0);
    //}
    //else {
    //    glColor4ub(255, 255, 255, 255);
    //}
    glColor4f(0.8, 0.8, 0.8, 1.0);
    glCallList(regHex[0]);

    /* 绘制地面 */
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(0.0, -0.001, 0.0);
    glCallList(regHex[1]);
    glPopMatrix();

    /* 绘制长椅 */
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glColor4f(0.3, 0.3, 0.3, 1.0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.8);
    glScalef(0.5, 0.5, 0.5);
    glCallList(regHex[2]);
    glScalef(2, 2, 2);
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glPushMatrix();
    camera.setViewTransform(); // 根据摄像机设置坐标变换
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[3]); // 绑定粒子纹理
    ps.render(); // 渲染粒子系统
    if (renderScene) doRenderScene();
    glBindTexture(GL_TEXTURE_2D, 0);
    if (drawAxis) doDrawAxis(1);
    glPopMatrix();
    glutSwapBuffers();
}

void doDrawAxis(float size)
{
    glDisable(GL_TEXTURE_2D);
    glDepthFunc(GL_ALWAYS);     // to avoid visual artifacts with grid lines
    // draw axis
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(size, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, size);
    glEnd();
    glLineWidth(1);

    // draw arrows(actually big square dots)
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(1, 0, 0);
    glVertex3f(size, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(0, size, 0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, size);
    glEnd();
    glPointSize(1);

    // restore default settings
    glDepthFunc(GL_LEQUAL);
}

void reshape(int w, int h)
{
    // set viewport to be the entire window
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
    else
        glOrtho(-1.0*(GLfloat)w / (GLfloat)h, 1.0*(GLfloat)w / (GLfloat)h, -1.0, 1.0, -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        leftMouseDown = (state == GLUT_DOWN);
    }
    else if (button == GLUT_RIGHT_BUTTON)
    {
        rightMouseDown = (state == GLUT_DOWN);
    }
    mousePrevious = glm::vec2(x, y);
}

void motion(int x, int y)
{
    mouseCurrent = glm::vec2(x, y);
    mouseDelta = (mouseCurrent - mousePrevious) / 10.0f;

    if (leftMouseDown) {
        /* 更新视角 */
        camera.addRotate(mouseDelta.y, mouseDelta.x);
    }
    else if (rightMouseDown) {
        /* 拖动喷泉水柱 */
        ps.pull(mouseDelta.x);
    }
    mousePrevious = mouseCurrent;
}

void idle() {
    // 限制fps
    timenow = glutGet(GLUT_ELAPSED_TIME);
    while (timenow - timeprev < 16) {
        timenow = glutGet(GLUT_ELAPSED_TIME);
    }
    float deltaT = (timenow - timeprev) / 1000.0;
    timeprev = timenow;
    ps.update(deltaT);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27:  // ESC, 退出
        exit(0);
        break;
    case 'p': // 暂停
        glutIdleFunc(NULL);
        break;
    case 'z': // 继续
        glutIdleFunc(idle);
        break;
    case 'x': // 旋转发射
        ps.spin();
        break;
    case 'c': // 改变光照
        if (enableLight0) {
            glDisable(GL_LIGHT0);
            enableLight0 = false;
        }
        else {
            glEnable(GL_LIGHT0);
            enableLight0 = true;
        }
        break;
    case 'v': // 改变粒子模型（使用glut提供的实体对象时）
        ps.changeShape();
        break;
    case 'k': // 增加发射路数
        ps.addWays();
        break;
    case 'j': // 减少发射路数
        ps.cutWays();
        break;
    case 'a': // 设置是否显示坐标系
        drawAxis = !drawAxis;
        break;
    case 's': // 设置是否渲染背景
        renderScene = !renderScene;
        break;
    case 'd': // 增大粒子尺寸
        ps.changeSize(0.005);
        break;
    case 'f': // 减小粒子尺寸
        ps.changeSize(-0.005);
        break;
    case 'u': // 设置是否使用外部obj文件替换粒子三维模型
        ps.setLoadModel();
        break;
    default:
        break;
    }
}

int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1080, 720);
    glutInitWindowPosition(30, 30);
    glutCreateWindow("喷泉模拟");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}

