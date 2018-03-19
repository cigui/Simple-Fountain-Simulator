#include <stdlib.h>
#include <stdio.h>
#include "Loader.h"
#include "Texture.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "YPS.h"

/* ����� */
Camera camera;

/* ������ */
bool leftMouseDown = false;
bool rightMouseDown = false;
glm::vec2 mouseCurrent = glm::vec2(0);
glm::vec2 mousePrevious = glm::vec2(0);
glm::vec2 mouseDelta = glm::vec2(0);

/* ���������� */
void doDrawAxis(float size);

/* ����ϵͳ */
YPS ps;

/* ��������FPS��ʱ����� */
int timenow = 0, timeprev = 0;

/* ���� */
unsigned int texture[7];

/* �Ƿ�򿪵���ƹ⡢�Ƿ���������ᡢ�Ƿ���Ⱦ���� */
bool enableLight0 = true, drawAxis = false, renderScene = true;

/* ���ڻ��Ƴ�����glCallList(regHex[i])�� */
GLuint regHex[3] = { 0, 0 };

void init() {
    /* ��Դ���� */
    glEnable(GL_LIGHTING);

    GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // ������
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // ������
    GLfloat specularLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };// �����

    GLfloat lightPos0[] = { 0.0f, 0.0f, 0.0f, 1.0f };    // ��Դλ��
    GLfloat lightPos1[] = { 10.0f, 10.0f, 10.0f, 1.0f }; // ��Դλ��

    glEnable(GL_LIGHTING);                            // ���ù���
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);   // ���û�����Դ
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);   // �����������Դ
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight); // ���þ����Դ
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);     // ���õƹ�λ��
    glEnable(GL_LIGHT0);                              // �򿪹�Դ

    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);   // ���û�����Դ
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);   // �����������Դ
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight); // ���þ����Դ
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);     // ���õƹ�λ��
    glEnable(GL_LIGHT1);                              // �򿪹�Դ

    glEnable(GL_COLOR_MATERIAL);                      // ���ò��ʵ���ɫ����

    /* �������� */
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientLight);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseLight);


    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // ָ����Ϻ���
    glEnable(GL_BLEND);

    /* �������� */
    loadTexture(texture[0], "Data/pool.jpg");           // ˮ��(?)����
    loadTexture(texture[1], "Data/ground.jpg");              // ��������
    loadTexture(texture[2], "Data/bench.jpg");          // ��������
    loadTexture(texture[3], "Data/particle.jpg");               // ��������
    loadTexture(texture[4], "Data/img_test.png");         // ������������
    loadTexture(texture[5], "Data/Particle-Texture.png"); // ������������
    loadTexture(texture[6], "Data/particle.png");               // ������������
    glEnable(GL_TEXTURE_2D);

    /* ����ģ�� */
    loadObj("Data/tube.obj", regHex[0]); // ˮ��(?)ģ��
    loadObj("Data/box.obj", regHex[1]);  // ����ģ��
    loadObj("Data/bench.obj", regHex[2]);// ����ģ��
    ps.load("Data/particle.obj");        // ����ģ��
}

void doRenderScene() {
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_GEN_S); // �����Զ���������S����
    glDisable(GL_TEXTURE_GEN_T); // �����Զ���������T����

    /* ����ˮ�� */
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    /* 
     * ������δ����ǻ�ȡ��ǰ��������ӵ���ɫ���ڻ���ˮ�أ�
     * ����ʵ����ȾЧ�������ã�����ע�͵���
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

    /* ���Ƶ��� */
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(0.0, -0.001, 0.0);
    glCallList(regHex[1]);
    glPopMatrix();

    /* ���Ƴ��� */
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
    camera.setViewTransform(); // �����������������任
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[3]); // ����������
    ps.render(); // ��Ⱦ����ϵͳ
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
        /* �����ӽ� */
        camera.addRotate(mouseDelta.y, mouseDelta.x);
    }
    else if (rightMouseDown) {
        /* �϶���Ȫˮ�� */
        ps.pull(mouseDelta.x);
    }
    mousePrevious = mouseCurrent;
}

void idle() {
    // ����fps
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
    case 27:  // ESC, �˳�
        exit(0);
        break;
    case 'p': // ��ͣ
        glutIdleFunc(NULL);
        break;
    case 'z': // ����
        glutIdleFunc(idle);
        break;
    case 'x': // ��ת����
        ps.spin();
        break;
    case 'c': // �ı����
        if (enableLight0) {
            glDisable(GL_LIGHT0);
            enableLight0 = false;
        }
        else {
            glEnable(GL_LIGHT0);
            enableLight0 = true;
        }
        break;
    case 'v': // �ı�����ģ�ͣ�ʹ��glut�ṩ��ʵ�����ʱ��
        ps.changeShape();
        break;
    case 'k': // ���ӷ���·��
        ps.addWays();
        break;
    case 'j': // ���ٷ���·��
        ps.cutWays();
        break;
    case 'a': // �����Ƿ���ʾ����ϵ
        drawAxis = !drawAxis;
        break;
    case 's': // �����Ƿ���Ⱦ����
        renderScene = !renderScene;
        break;
    case 'd': // �������ӳߴ�
        ps.changeSize(0.005);
        break;
    case 'f': // ��С���ӳߴ�
        ps.changeSize(-0.005);
        break;
    case 'u': // �����Ƿ�ʹ���ⲿobj�ļ��滻������άģ��
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
    glutCreateWindow("��Ȫģ��");

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

