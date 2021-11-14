#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\soil\SOIL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>

#include "ShaderLoader.h";
//#include "CGameObject.h"

using glm::vec3;
using std::vector;

#define PRIM_RESTART 0xffffff

GLuint renderProg, computeProg;
glm::ivec3 nParticles;
GLuint totalParticles;

float vTime, deltaT, speed, angle;
GLuint particlesVao;
GLuint bhVao, bhBuf;  // black hole VAO and buffer
glm::vec4 bh1, bh2;

GLfloat currentTime;

glm::mat4 projection, model, view;
int width = 1280, height = 720;
bool m_animate = true;

void compileAndLinkShader();
void initBuffers();
void setMatrices();
void animate(bool value) { m_animate = value; }
bool animating() { return m_animate; }

void initParams()
{
    nParticles.x = 256; nParticles.y =128 ; nParticles.z =128 ;

    vTime = 0.0f; deltaT = 0.0f;  speed = 35.0f; angle = 0.0f;
    bh1.x = 7; bh1.y = 0; bh1.z = 0; bh1.w = 1;
    bh2.x = -7; bh2.y = 0; bh2.z = 0; bh2.w = 1;
    
    totalParticles = nParticles.x * nParticles.y * nParticles.z;
}
//complete
void init()
{
    initParams();
    compileAndLinkShader();
    initBuffers();

    //glClearColor(1, 1, 1, 1);
    //camera or MVP
   projection = glm::perspective(glm::radians(50.0f), (float)width / height, 1.0f, 100.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
//complete
void initBuffers()
{
    // Initial positions of the particles
    vector<GLfloat> initPos;
    vector<GLfloat> initVel(totalParticles * 4, 0.0f);
    glm::vec4 p(0.0f, 0.0f, 0.0f, 1.0f);
    GLfloat dx = 2.0f / (nParticles.x - 1),
        dy = 2.0f / (nParticles.y - 1),
        dz = 2.0f / (nParticles.z - 1);
    // We want to center the particles at (0,0,0)
    glm::mat4 transf = glm::translate(glm::mat4(1.0f), glm::vec3(-1, -1, -1));
    for (int i = 0; i < nParticles.x; i++) {
        for (int j = 0; j < nParticles.y; j++) {
            for (int k = 0; k < nParticles.z; k++) {
                p.x = dx * i;
                p.y = dy * j;
                p.z = dz * k;
                p.w = 1.0f;
                p = transf * p;
                initPos.push_back(p.x);
                initPos.push_back(p.y);
                initPos.push_back(p.z);
                initPos.push_back(p.w);
            }
        }
    }

    // We need buffers for position , and velocity.
    GLuint bufs[2];
    glGenBuffers(2, bufs);
    GLuint posBuf = bufs[0];
    GLuint velBuf = bufs[1];

    GLuint bufSize = totalParticles * 4 * sizeof(GLfloat);

    // The buffers for positions
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posBuf);
    glBufferData(GL_SHADER_STORAGE_BUFFER, bufSize, &initPos[0], GL_DYNAMIC_DRAW);

    // Velocities
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velBuf);
    glBufferData(GL_SHADER_STORAGE_BUFFER, bufSize, &initVel[0], GL_DYNAMIC_COPY);

    // Set up the VAO
    glGenVertexArrays(1, &particlesVao);
    glBindVertexArray(particlesVao);

    glBindBuffer(GL_ARRAY_BUFFER, posBuf);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Set up a buffer and a VAO for drawing the attractors (the "black holes")
    glGenBuffers(1, &bhBuf);
    glBindBuffer(GL_ARRAY_BUFFER, bhBuf);
    GLfloat data[] = { bh1.x, bh1.y, bh1.z, bh1.w, bh2.x, bh2.y, bh2.z, bh2.w };
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), data, GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &bhVao);
    glBindVertexArray(bhVao);

    glBindBuffer(GL_ARRAY_BUFFER, bhBuf);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
//complete
void update()
{
    currentTime = glutGet(GLUT_ELAPSED_TIME);
   	currentTime = currentTime / 1000;
    if (vTime == 0.0f) {
        deltaT = 0.0f;
    }
    else {
        deltaT = currentTime - vTime;
    }
    vTime = currentTime;
    if (animating()) {
        angle += speed * deltaT;
        if (angle > 360.0f) angle -= 360.0f;
    }
    glutPostRedisplay();
}

void render()
{
    
    // Rotate the attractors ("black holes")
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 0, 1));
    glm::vec3 att1 = glm::vec3(rot * bh1);
    glm::vec3 att2 = glm::vec3(rot * bh2);

    // Execute the compute shader
    glUseProgram(computeProg);
   
    /*GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME);
    currentTime = currentTime / 10000;
    GLint currentTimeLocation = glGetUniformLocation(computeProg, "DeltaT");
    glUniform1f(currentTimeLocation, currentTime);*/

    glDispatchCompute(totalParticles / 1024, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    

    // Draw the scene
    glUseProgram(renderProg);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    view = glm::lookAt(glm::vec3(2, 0, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    model = glm::mat4(1.0f);
    setMatrices();

    // Draw the particles
    glPointSize(2.0f);
    GLint colorUniLocation = glGetUniformLocation(renderProg, "Color");
    glm::vec4 color = glm::vec4(1, 1, 1, 0.2f);
    glUniform4fv(colorUniLocation, 1, glm::value_ptr(color));    

    glBindVertexArray(particlesVao);
    glDrawArrays(GL_POINTS, 0, totalParticles);
    glBindVertexArray(0);

    // Draw the attractors
    glPointSize(5.0f);
    GLfloat data[] = { att1.x, att1.y, att1.z, 1.0f, att2.x, att2.y, att2.z, 1.0f };
    glBindBuffer(GL_ARRAY_BUFFER, bhBuf);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(GLfloat), data);
    color = glm::vec4(1.0f, 0, 0, 1.0f);
    glUniform4fv(colorUniLocation, 1, glm::value_ptr(color));
    
    glBindVertexArray(bhVao);
    glDrawArrays(GL_POINTS, 0, 2);
    glBindVertexArray(0);

    glutSwapBuffers();

}

void setMatrices() {
    glUseProgram(renderProg);
    glm::mat4 mv = view * model;
    glm::mat3 norm = glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]));

    GLint MVPmatLocation = glGetUniformLocation(renderProg, "ModelViewMatrix");
    GLint NormalMatLocation = glGetUniformLocation(renderProg, "NormalMatrix");
    GLint MVP = glGetUniformLocation(renderProg, "MVP");

    glUniformMatrix4fv(MVPmatLocation, 1, false, &mv[0][0]);
    //glUniformMatrix4fv(MVPmatLocation, 1, false, glm::value_ptr(mv));
    glUniformMatrix3fv(NormalMatLocation, 1, false, &norm[0][0]);
    glUniformMatrix4fv(MVP, 1, false, glm::value_ptr(projection*mv));

    /*renderProg.setUniform("ModelViewMatrix", mv);
    renderProg.setUniform("NormalMatrix", norm);
    renderProg.setUniform("MVP", projection * mv);*/
}


void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
}

void compileAndLinkShader()
{
    ShaderLoader shaderLoader;
    renderProg = shaderLoader.CreateProgram("ParticlesVertexShader.vs",
        "ParticlesFragment.fs");
    computeProg = shaderLoader.CreateProgram("ParticlesCompShader.cs");

    /*try {
        renderProg.compileShader("shader/particles.vs");
        renderProg.compileShader("shader/particles.fs");
        renderProg.link();

        computeProg.compileShader("shader/particles.cs");
        computeProg.link();
    }
    catch (GLSLProgramException& e) {
        std::cout << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }*/
}


int main(int argc, char **argv){

	
	// init glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(75, 75);
	glutInitWindowSize(width, height);
	glutCreateWindow("Compute Shader | Particles Example");

	//init GLEW
	glewInit();
	
	//init Game objects
	init();

	//clear      R  G  B
	glClearColor(0, 0, 0, 1.0); // 1RGBA

	// register callbacks
	glutDisplayFunc(render);

	glutIdleFunc(update);

	glutMainLoop();

	return 0;
}