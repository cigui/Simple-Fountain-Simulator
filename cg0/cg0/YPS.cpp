#include "YPS.h"

YPS::YPS() {
    this->maxParticles = 2000;
    this->origin = glm::vec3(0.0, 0.0, 0.0);
    this->force = glm::vec3(0, -0.98, 0);
    this->angle = 0;
    this->spinning = false;
    this->angularV = 0;
    this->ways = 5;
    this->rAdd = true;
    this->gAdd = true;
    this->bAdd = true;
    this->loadModel = false;
    this->currentColor = glm::vec4(0.22, 0.77, 0.73, 0.5); // 最初的颜色，#39c5bb
    this->size = SIZE_BASE * 0.8;
    this->modelSize = 0.0;
    this->shape = CONE;
}

// 加载obj模型
void YPS::load(const char *file) {
    if (!strcmp(file, "")) {
        loadModel = false;
    }
    else {
        modelSize = loadObj(file, regHex);
        loadModel = true;
    }
}

glm::vec4 YPS::getCurrentColor() {
    return currentColor;
}

// 更新粒子状态
void YPS::update(float deltaT) {
    timeCnt++;
    float r = randUN;
    std::vector<Particle>::iterator it;
    for (it = particles.begin(); it != particles.end();) {
        Particle &p = *it;
        p.life -= deltaT;
        if (p.life > 0 && p.postition.y >= 0) {
            p.postition += p.velocity * deltaT;
            p.velocity += force * deltaT;
            p.color.a = (p.life / LIFE_BASE); // 将粒子alpha值设为其寿命
            it++;
        }
        else {
            // 消除粒子
            it = particles.erase(it);
            particleCnt--;
        }
    }
    if (particleCnt < maxParticles - ways) {
        emit(ways); // 发射新粒子
        particleCnt += ways;
    }

    // 改变当前粒子颜色
    r = currentColor.r;
    float g = currentColor.g;
    float b = currentColor.b;
    if (rAdd) {
        currentColor.r += randUN / 10.0;
        currentColor.r = (currentColor.r > 1) ? 1 : currentColor.r;
        rAdd = (currentColor.r > 0.9) ? false : true;
    }
    else {
        currentColor.r -= randUN / 10.0;
        currentColor.r = (currentColor.r < 0) ? 0 : currentColor.r;
        rAdd = (currentColor.r < 0.1) ? true : false;
    }
    if (gAdd) {
        currentColor.g += randUN / 10.0;
        currentColor.g = (currentColor.g > 1) ? 1 : currentColor.g;
        gAdd = (currentColor.g > 0.9) ? false : true;
    }
    else {
        currentColor.g -= randUN / 10.0;
        currentColor.g = (currentColor.g < 0) ? 0 : currentColor.g;
        gAdd = (currentColor.g < 0.1) ? true : false;
    }
    if (bAdd) {
        currentColor.b += randUN / 10.0;
        currentColor.b = (currentColor.b > 1) ? 1 : currentColor.b;
        bAdd = (currentColor.b > 0.9) ? false : true;
    }
    else {
        currentColor.b -= randUN / 10.0;
        currentColor.b = (currentColor.b < 0) ? 0 : currentColor.b;
        bAdd = (currentColor.b < 0.1) ? true : false;
    }
    /*if (colorAdd) {
        if (r > 0.9 || g > 0.9 || b > 0.9) {
            colorAdd = false;
        }
        else {
            currentColor += glm::vec4(randUN / 10.0, randUN / 10.0, randUN / 10.0, 0);
            currentColor.x = (currentColor.x > 1) ? 1 : currentColor.x;
            currentColor.y = (currentColor.x > 1) ? 1 : currentColor.y;
            currentColor.z = (currentColor.x > 1) ? 1 : currentColor.z;
        }
    }
    else {
        if (r < 0.1 || g < 0.1 || b < 0.1) {
            colorAdd = true;
        }
        else {
            currentColor -= glm::vec4(randUN / 10.0, randUN / 10.0, randUN / 10.0, 0);
            currentColor.x = (currentColor.x < 0) ? 0 : currentColor.x;
            currentColor.y = (currentColor.x < 0) ? 0 : currentColor.y;
            currentColor.z = (currentColor.x < 0) ? 0 : currentColor.z;
        }
    }*/
    //if (timeCnt == 125) {
    //    timeCnt = 0;
    //    //currentColor = glm::vec4(randUN, randUN, randUN, randUN);
    //}
}

// 发射新粒子
void YPS::emit(int num) {
    float r = randUN;
    for (int i = 0; i < num; i++) {
        double angle = 2 * PI / num * i - this->angle / 0.3;
        if (spinning) { // 旋转发射
            angle -= angularV * timeCnt;
        }
        Particle p;
        p.color = currentColor;
        p.life = LIFE_BASE + LIFE_VAR * r;
        p.postition = origin;
        p.velocity = glm::vec3(0.3 * cos(angle), 1.0, 0.3 * sin(angle));
        p.size = size;
        particles.push_back(p);
    }
    if (spinning) angularV += 0.001; // 旋转发射的角速度匀速增加
}

// 渲染粒子系统
void YPS::render() {
    std::vector<Particle>::iterator it;
    float x, y, z, r, g, b, a, size;
    for (it = particles.begin(); it != particles.end(); it++) {
        Particle &p = *it;
        // 粒子位置属性
        x = p.postition.x;
        y = p.postition.y;
        z = p.postition.z;
        // 粒子颜色属性
        r = p.color.r;
        g = p.color.g;
        b = p.color.b;
        a = p.color.a;
        size = p.size;
        glColor4f(r, g, b, a);
        glPushMatrix();
        glTranslatef(x, y, z);
        if (loadModel) {
            glDisable(GL_TEXTURE_GEN_S); // 禁用自动生成纹理S坐标
            glDisable(GL_TEXTURE_GEN_T); // 禁用自动生成纹理T坐标
            // 加载的模型可能太大，需要根据粒子尺寸进行缩放
            float multiple = modelSize / size;
            glScalef(1 / multiple, 1 / multiple, 1 / multiple);
            glCallList(regHex);
            glScalef(multiple, multiple, multiple);
        }
        else {
            glEnable(GL_TEXTURE_GEN_S); // 自动生成纹理S坐标
            glEnable(GL_TEXTURE_GEN_T); // 自动生成纹理T坐标
            // 使用glut提供的实体对象绘制粒子
            switch (shape) {
                case CONE:
                    glutSolidCone(size, size, 10, 10);
                    break;
                case CUBE:
                    glutSolidCube(size);
                    break;
                case SPHERE:
                    glutSolidSphere(size, 10, 10);
                    break;
                case TORUS:
                    glutSolidTorus(size - 0.001, size + 0.001, 10, 10);
                    break;
            }
        }
        glPopMatrix();
    }
}

void YPS::pull(float dx) {
    angle += dx * 0.01;
    while (angle > 0.3 * 2 * PI) {
        angle -= 0.3 * 2 * PI;
    }
}

void YPS::spin() {
    spinning = !spinning;
    angularV = 0;
}

void YPS::addWays() {
    if (ways < 10) ways++;
}

void YPS::cutWays() {
    if (ways > 0) ways--;
}

void YPS::changeSize(float ds) {
    float result = size + ds;
    if (result > SIZE_BASE * 0.1 && result < SIZE_BASE) size = result;
}

void YPS::setLoadModel() {
    loadModel = !loadModel;
}

void YPS::changeShape() {
    shape = (Shape)((shape + 1) % 4);
}