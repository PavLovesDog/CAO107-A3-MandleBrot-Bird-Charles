#pragma once
#include "shaderloader.h"
#include "glm/glm.hpp"
#include "Dependencies/soil/SOIL.h"

#include "CGameObject.h"
//Can inherit from GameObject class
class FBRenderPass //:public CGameObject
{
private:
	GLuint vao, vbo, ebo;
	glm::mat4 transform;
	const glm::uint mWIDTH = 1280, mHEIGHT = 720;
public:
	FBRenderPass() { };
	~FBRenderPass() {

	};
	GLint indices[6] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};
	glm::vec2 halfTexel = 1.0f / glm::vec2(mWIDTH, mHEIGHT) * 0.5f;

	float vertexData[36] = {
		-1, -1, 0, 1, halfTexel.x, halfTexel.y,
		1, 1, 0, 1, 1 - halfTexel.x, 1 - halfTexel.y,
		-1, 1, 0, 1, halfTexel.x, 1 - halfTexel.y,

		-1, -1, 0, 1, halfTexel.x, halfTexel.y,
		1, -1, 0, 1, 1 - halfTexel.x, halfTexel.y,
		1, 1, 0, 1, 1 - halfTexel.x, 1 - halfTexel.y,
	};

	 
	glm::uint fbo;
	glm::uint texID;
	glm::uint rbo;
	glm::uint progID;
	
	void update(mat4 transform) {
		//do nothing
	}
	void setupFB()
	{
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8,mWIDTH, mHEIGHT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,texID, 0);

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,mWIDTH, mHEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, rbo);

		/*GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);*/
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
	bool  init()    {
		
		ShaderLoader  shaderLoader;
		 progID = shaderLoader.CreateProgram("FB_PostProcess.vs",
			                                  "FB_PostProcess.fs");
		////this->CGameObject::init();
		//** VAO **
		 glGenVertexArrays(1, &vao);
		 glBindVertexArray(vao);

		 glGenBuffers(1, &vbo);
		 glBindBuffer(GL_ARRAY_BUFFER, vbo);

		 glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6,
			 vertexData, GL_STATIC_DRAW);

		 glEnableVertexAttribArray(0);
		 glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
			 sizeof(float) * 6, 0);

		 glEnableVertexAttribArray(1);
		 glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
			 sizeof(float) * 6, ((char*)0) + 16);

		 glBindVertexArray(0);
		 glBindBuffer(GL_ARRAY_BUFFER, 0);

		// It's always a good thing to unbind any buffer/array to prevent strange bugs
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		
		setupFB();
			return true;
	};
	void update () 
	{
		/*GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME);
		currentTime = currentTime / 1000;
		GLint currentTimeLocation = glGetUniformLocation(progID, "currentTime");
		glUniform1f(currentTimeLocation, currentTime);*/

		//printf("currenttime, %f \n", currentTime);

		//transforms
		glm::mat4 vtransform;

		vtransform = glm::scale(vtransform, glm::vec3(0.5f, 0.5f, 0.5f));
		//transform = glm::rotate(transform, 90.0f, glm::vec3(0.0, 0.0, 1.0));
		vtransform = glm::translate(vtransform, glm::vec3(0.9f, -0.3f, 0.0f));

		//vtransform = glm::rotate(vtransform, (GLfloat)/*currentTime */ 10 * 50.0f, 
		//	                         glm::vec3(0.0f, 0.0f, 1.0f));

		this->transform = vtransform;

		GLuint transformLoc = glGetUniformLocation(progID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	};
	void render()  {

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
		glClear( GL_DEPTH_BUFFER_BIT);

		//glViewport(0, 0, mWIDTH, mHEIGHT);
		//Draw object
		glUseProgram(progID);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(progID, "Target"), 0);
		glBindTexture(GL_TEXTURE_2D, texID);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		
	};
};
