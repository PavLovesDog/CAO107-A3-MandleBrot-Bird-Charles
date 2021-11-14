#include "CGameObject.h"

bool CGameObject::init()
{

	program = shaderLoader.CreateProgram("Transform_Texture_QUAD.vs", "Transform_Texture_QUAD.fs");

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Set up vertex data (and buffer(s)) and attribute pointers
	//GLfloat vertices[] = {

	//	// Positions          // Colors           //texture coords 
	//	0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,    // Top Right
	//	0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,    // Bottom Right
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,    // Bottom Left
	//	-0.5f, 0.5f, 0.0f,    1.0f, 1.0f, 0.0f,   0.0f, 1.0f     // Top Left 
	//};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	//** VAO **
	// Generate vertex arrow object
	glGenVertexArrays(1, &vao);
	// Bind the Vertex Array Object to openGl context
	glBindVertexArray(vao);//otherwise glVertexAttribPointer

	//** VBO **
	// Then bind and set vertex buffer(s).
	// First paramter is how many buffers we have to create
	glGenBuffers(1, &vbo);

	int sz = sizeof(vertices);
	// bind VBO to binding point, here it is GL_ARRAY_BUFFER
	// there are other binding points
	glBindBuffer(GL_ARRAY_BUFFER, vbo);//bind to context
	glBufferData(GL_ARRAY_BUFFER,
		/*sizeof(vertices)*/ 32*sizeof(GLfloat),// GPU need to know how much memory needs to be allocated
		vertices,//copy data to GPU
		GL_STATIC_DRAW);// How to use the buffer - buffer is created and modified once

	//** EBO **
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(indices),
		indices,
		GL_STATIC_DRAW);


	// ** Attributes **

	//** Vertex Attribute **
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//** Color Attribute **
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//** TexCoord attribute **
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// It's always a good thing to unbind any buffer/array to prevent strange bugs
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//** Load and bind texture 1 
	//--------------------------
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);


	//** loadImage and create texture
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image("wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	// ** Load and Bind Texture 2
	//---------------------------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);


	//** loadImage and create texture
	// Load image, create texture and generate mipmaps
	unsigned char* image2 = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);


	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;

}

void CGameObject::update(mat4 pTransform)
{
	GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME);
	currentTime = currentTime / 1000;
	GLint currentTimeLocation = glGetUniformLocation(program, "currentTime");
	glUniform1f(currentTimeLocation, currentTime);

	this->transform = pTransform;

	GLuint transformLoc = glGetUniformLocation(program, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));	

}
void CGameObject::update()
{
	GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME);
	currentTime = currentTime / 1000;
	GLint currentTimeLocation = glGetUniformLocation(program, "currentTime");
	glUniform1f(currentTimeLocation, currentTime);

	//printf("currenttime, %f \n", currentTime);

	//transforms
	glm::mat4 vtransform;

	vtransform = glm::scale(vtransform, glm::vec3(0.5f, 0.5f, 0.5f));
	//transform = glm::rotate(transform, 90.0f, glm::vec3(0.0, 0.0, 1.0));
	vtransform = glm::translate(vtransform, glm::vec3(0.9f, -0.3f, 0.0f));

	vtransform = glm::rotate(vtransform, (GLfloat)currentTime / 10 * 50.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	this->transform = vtransform;

	GLuint transformLoc = glGetUniformLocation(program, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

}

void CGameObject::render()
{
	
	// Draw our first triangle
	//glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);

	//bind 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(program, "Texture"), 0);

	
	/*glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(program, "Texture2"), 1);*/

	glBindVertexArray(vao);
	
	update(getTransform());	

	// Draw Elements 	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	
	glBindVertexArray(0);

	
}