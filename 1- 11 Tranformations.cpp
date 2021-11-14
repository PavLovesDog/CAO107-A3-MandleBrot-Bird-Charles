//
///*TODO:
//* Vertex Data:
//
//
//*/
//#include "Dependencies\glew\glew.h"
//#include "Dependencies\freeglut\freeglut.h"
//#include "Dependencies\soil\SOIL.h"
//
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtc/type_ptr.hpp"
//
//
//#include <iostream>
//#include <vector>
//#include <algorithm>
//#include <cstdlib>
//
//#include "ShaderLoader.h";
//#include "CGameObject.h"
//#include "FBRenderPass.h"
//
//GLuint program;
//GLuint compProgram;
//GLuint vao, vbo, ebo;
//GLuint texture, texture2;
//FBRenderPass mRenderPass;
//enum {
//	PARTICLE_GROUP_SIZE = 128,
//	PARTICLE_GROUP_COUNT = 8000,
//	PARTICLE_COUNT =
//	(PARTICLE_GROUP_SIZE *
//		PARTICLE_GROUP_COUNT),
//	MAX_ATTRACTORS = 64
//};
//static union {
//	struct {
//		GLuint position_buffer;
//		GLuint velocity_buffer;
//	}; GLuint buffers[2];
//};
//// TBOs texture buffer objects
//static union {
//	struct {
//		GLuint position_tbo;
//		GLuint velocity_tbo;
//	}; GLuint tbos[2];
//};
//std::vector<CGameObject*> gameObjectsList;
////glm::mat4 transform;
////glm::mat4 trans2;
//
//void initCompShaderStuff()
//{
//	     //Generate two buffers, bind themand initialize their data //stores one
//		//for positionand other for velocity
//	glGenBuffers(2, buffers);
//	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
//	glBufferData(GL_ARRAY_BUFFER, PARTICLE_COUNT *
//		sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);
//	// Map the position buffer and fill it with random vectors
//	glm::vec4* positions =
//		(glm::vec4*)glMapBufferRange(GL_ARRAY_BUFFER,
//			0, //offset
//			PARTICLE_COUNT * sizeof(glm::vec4), //length
//			GL_MAP_WRITE_BIT |
//			GL_MAP_INVALIDATE_BUFFER_BIT); //access
//	srand(10);//random seed
//	for (int i = 0; i < PARTICLE_COUNT; i++) {
//		positions[i] = glm::vec4(rand()%10,rand()% -10,rand()%10,
//			                     rand());
//	}
//	glUnmapBuffer(GL_ARRAY_BUFFER);
//
//	//setting up Texture
//	glGenTextures(2, tbos);
//	for (int i = 0; i < 2; i++) {
//		glBindTexture(GL_TEXTURE_BUFFER, tbos[i]);
//		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, buffers[i]);
//	}
//
//	//Create Attractor buffer which determine the movement of the particles.
//	//They are initialized in initand updated in render function.
//	GLuint attractor_buffer;
//	GLfloat attractor_masses[MAX_ATTRACTORS];
//	glGenBuffers(1, &attractor_buffer);
//	glBindBuffer(GL_UNIFORM_BUFFER, attractor_buffer);
//	glBufferData(GL_UNIFORM_BUFFER, 32 * sizeof(glm::vec4), NULL,
//		GL_STATIC_DRAW);
//	// masses for attractors are between 0.5 and 1.0
//	// positions are initilized to 0 , but are moved during the render loop
//	srand(500);
//	for (int i = 0; i < MAX_ATTRACTORS; i++) {
//		attractor_masses[i] = 0.5f + rand() * 0.5f;
//	}
//	// Now bind the buffer to the zeroth location
//	glBindBufferBase(GL_UNIFORM_BUFFER,// target buffer type
//		0,//index of the binding point
//		attractor_buffer);//name of target buffer
//}
//void init(){
//	initCompShaderStuff();
//	ShaderLoader shaderLoader;
//	program = shaderLoader.CreateProgram("ParticlesVertexShader.vs",
//		                                 "ParticlesFragment.fs");
//	compProgram = shaderLoader.CreateProgram("ParticlesCompShader.cs");
//
//#pragma region previouse code
//
//	//CGameObject *gameobjMain = new CGameObject();
//
//	//CGameObject *gobj2 = new CGameObject();
//
//	//CGameObject *gobj3 = new CGameObject();;
//
//	//
//	////CGameObject *mRenderPass = new FBRenderPass();
//
//	//GLfloat vertices[] = {
//
//	//	// Positions          // Colors           //texture coords 
//	//	0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,    // Top Right
//	//	0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,    // Bottom Right
//	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,    // Bottom Left
//	//	-0.5f, 0.5f, 0.0f,    1.0f, 1.0f, 0.0f,   0.0f, 1.0f     // Top Left 
//	//};
//	//gameobjMain->setVertices(vertices);
//	//gobj2->setVertices(vertices);
//	//gobj3->setVertices(vertices);
//
//
//	////gameObjectsList.push_back(mRenderPass);
//
//	//gameObjectsList.push_back(gameobjMain);
//	////2nd object
//	//gameObjectsList.push_back(gobj2);
//
//	//gameObjectsList.push_back(gobj3);
//
//	//mRenderPass.init();
//
//	//for (auto itr = gameObjectsList.begin(); itr != gameObjectsList.end(); itr++)
//	//	     (*itr)->init();
//
//	//mRenderPass.init();
//#pragma endregion
//	
//}
//
//void render(){
//
//#pragma region Old-Code
//	
//	////glClear(GL_COLOR_BUFFER_BIT);
// // //glBindFramebuffer(GL_FRAMEBUFFER, mRenderPass.fbo);
//	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	//
//	//for (auto itr = gameObjectsList.begin(); itr != gameObjectsList.end(); itr++)
//	//                   (*itr)->render();
//	//
//	////mRenderPass.render();
//#pragma endregion
//	glUseProgram(compProgram);
//	glBindImageTexture(0, // location id
//		velocity_tbo, // buffer id
//		0, // level
//		GL_FALSE, // layered
//		0, // layer
//		GL_READ_WRITE, //access type
//		GL_RGBA32F);
//	glBindImageTexture(1, // location id
//		position_tbo, // buffer id
//		0, // level
//		GL_FALSE, // layered
//		0, // layer
//		GL_READ_WRITE, //access type
//		GL_RGBA32F);
//
//	//Set delta time
//	GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME);
//	currentTime = currentTime / 1000;
//	GLint currentTimeLocation = glGetUniformLocation(program, "dt");
//	glUniform1f(currentTimeLocation, currentTime);
//
//	// Dispatch the compute shader
//	glDispatchCompute(PARTICLE_GROUP_COUNT/1000, 1, 1);
//	// Ensure that writes by the compute shader have completed
//	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
//	glPointSize(1.0f);
//	glUseProgram(program);
//	 //glBindVertexArray(position_buffer);
//	glDrawArrays(GL_POINTS, 0,PARTICLE_COUNT);
//	glutSwapBuffers();
//}
//
//void update(){
////	glm::mat4 vtransform;
////	vtransform = glm::scale(vtransform, glm::vec3(0.5f, 0.5f, 0.5f));	
////	vtransform = glm::translate(vtransform, glm::vec3(-0.9f, -0.3f, 0.0f));
////
////	glm::mat4 trans2;
////	trans2 = glm::scale(trans2, glm::vec3(1.0f, 1.0f, 1.0f));	
////	trans2 = glm::translate(trans2, glm::vec3(0.9f, 0.3f, 0.0f));
////
////	glm::mat4 trans3;
////	trans3 = glm::scale(trans3, glm::vec3(1.0f, 1.0f, 1.0f));
////	trans3 = glm::rotate(trans3, 90.0f, glm::vec3(0.3f, 0.0f, 1.0));
////	trans3 = glm::translate(trans3, glm::vec3(0.9f, -0.3f, 0.0f));
//////	transform = glm::rotate(transform, (GLfloat)currentTime / 10 * 50.0f, glm::vec3(0.0f, 0.0f, 1.0f));
////
////	//if(gameObjectsList[1])
////	gameObjectsList[0]->setTransform(vtransform);
////	gameObjectsList[1]->setTransform(trans2);
////	gameObjectsList[2]->setTransform(trans3);
////	//gameObjectsList[3]->setTransform(trans3);
////
////	
////	gameObjectsList[0]->update();
////	gameObjectsList[1]->update(gameObjectsList[1]->getTransform());
////	gameObjectsList[2]->update(gameObjectsList[2]->getTransform());
////	//gameObjectsList[3]->setTransform(trans3);
//
//	glutPostRedisplay();
//	
//	//mRenderPass.update();
//	/*for (auto itr = gameObjectsList.begin(); itr != gameObjectsList.end(); itr++)
//	               itr->update(itr->getTransform());*/
//	
//	
//}
//
//int main(int argc, char **argv){
//
//	
//	// init glut
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
//	glutInitWindowPosition(500, 300);
//	glutInitWindowSize(1280, 720);
//	glutCreateWindow("QUAD EBO");
//
//	//init GLEW
//	glewInit();
//	
//	//init Game objects
//	init();
//
//
//	//clear
//	glClearColor(1.0, 0.0, 0.0, 1.0);//clear red
//
//	// register callbacks
//	glutDisplayFunc(render);
//
//	glutIdleFunc(update);
//
//	glutMainLoop();
//
//	return 0;
//}
