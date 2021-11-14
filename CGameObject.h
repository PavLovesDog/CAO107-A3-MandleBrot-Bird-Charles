#pragma once
#include "glm/gtc/matrix_transform.hpp"
#include "shaderloader.h"
#include "Dependencies/soil/SOIL.h"
#include "glm/gtc/type_ptr.hpp"

using glm::mat4;
class CGameObject {

public:
	CGameObject() 
	{
	};
	 ~CGameObject() 
	{
		//TODO:
		/*if (vertices != NULL)
			delete vertices;*/
	};
	virtual bool init();
	  void update();
	  void update(mat4 transform);
	  void render();
	mat4 getTransform() 
	{
		return transform;
	};

	void setTransform(mat4 pTransform) 
	{
		this->transform = pTransform;
	};

	void setVertices(GLfloat* pVerticies)
	{
		GLuint total = 32; //= sizeof(pVerticies);//* sizeof(GLfloat);

		vertices = new GLfloat[total];

		GLfloat* p = pVerticies;
		/*for (int i = 0; i < total; i++)
		{
			std::cout << *p <<"	";
			p++;

		}*/

		memcpy(vertices, pVerticies, total*sizeof(GLfloat));

		p = vertices;
		std::cout << "\n printing vertices\n";
		for (GLuint i = 0; i < total; i++)
		{
			//std::cout << *p << "	";
			p++;

		}
		int i = 0;
		/*for (GLfloat* p = pVerticies; p != NULL; p++,i++)
		{
			vertices[i] = *pVerticies;
		}*/
	}
private:
	ShaderLoader shaderLoader;
	mat4 transform;
	GLuint program;
	GLuint vao, vbo, ebo;
	GLuint texture, texture2;

	GLfloat* vertices;
	
	//GLuint indices[]; assumption that always using quads

};
