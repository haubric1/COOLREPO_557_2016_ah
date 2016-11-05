//
//  main.cpp
//  Texture template for Homework 4
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//



/*---------------------------------------------------------------------------------
Note,

- The code gives you some hints what to do. I added some comments
- The shader code is stored in two files. HW4_shader.vs/.fs
  You must edit this shader programs in order to get the shader code to work with textures. 

In addition:
- The class (file) ShaderFileUtils provides functions to load shader code from a file. 
- The class (file) ImgLoader provides a function to load a bitmap .bmp image from a file. 

-----------------------------------------------------------------------------------*/

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"
#include "ShaderFileUtils.h"
#include "ImgLoader.h"



// this line tells the compiler to use the namespace std.
// Each object, command without a namespace is assumed to be part of std. 
using namespace std;


/// Camera control matrices
glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix; // Store the view matrix
glm::mat4 modelMatrix; // Store the model matrix




// The handle to the window object
GLFWwindow*         window;


// Define some of the global variables we're using for this sample
GLuint program;




// USE THESE vertex array objects to define your objects
unsigned int vaoID[2];

unsigned int vboID[4];




/**
This function checks your shader code
@param shader - the id of the shader
@param shader_type - type = GL_FRAGMENT_SHADER or GL_VERTEX_SHADER
*/
bool CheckShaderLocal(GLuint shader, GLenum shader_type)
{
	GLint compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLint info_len = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);

		if (info_len)
		{
			char* buf = (char*)malloc(info_len);
			if (buf) {
				glGetShaderInfoLog(shader, info_len, NULL, buf);
				cout << "Could not compile shader shader_type " << shader_type << ":\n" << buf << endl;
				free(buf);
			}
			glDeleteShader(shader);
			shader = 0;
		}
		return false;
	}
	return true;

}




/*!
This function creates a single square
 */
void createSquare(void)
{
	float* vertices = new float[30];  // Vertices for our square
	float* normals = new float[18]; // normals for our vertices

	vertices[0] = -1; vertices[1] = -1; vertices[2] = 0.5; // Bottom left corner
	vertices[3] = 0.0; vertices[4] = 0.0; //Texture Vertices
	normals[0] = 1.0; normals[1] = 0.0; normals[2] = 0.0; // Bottom left corner

	vertices[5] = -1; vertices[6] = 1; vertices[7] = 0.5; // Top left corner
	vertices[8] = 0.0; vertices[9] = 1.0; //Texture Vertices
	normals[3] = 0.0; normals[4] = 1.0; normals[5] = 0.0; // Top left corner

	vertices[10] = 1; vertices[11] = 1; vertices[12] = 0.5; // Top Right corner
	vertices[13] = 1.0; vertices[14] = 1.0; //Texture Vertices
	normals[6] = 0.0; normals[7] = 0.0; normals[8] = 1.0; // Top Right corner

	vertices[15] = 1; vertices[16] = -1; vertices[17] = 0.5; // Bottom right corner
	vertices[18] = 1.0; vertices[19] = 0.0; //Texture Vertices
	normals[9] = 0.0; normals[10] = 0.0; normals[11] = 0.0; // Bottom right corner

	vertices[20] = -1; vertices[21] = -1; vertices[22] = 0.5; // Bottom left corner
	vertices[23] = 0.0; vertices[24] = 0.0; //Texture Vertices
	normals[12] = 1.0; normals[13] = 0.0; normals[14] = 0.0; // Bottom left corner

	vertices[25] = 1; vertices[26] = 1; vertices[27] = 0.5; // Top Right corner
	vertices[28] = 1.0; vertices[29] = 1.0; //Texture Vertices
	normals[15] = 0.0; normals[16] = 0.0; normals[17] = 1.0; // Top Right corner


	/*---------------------------------------------------------------------------------
	  TODO:
	  - Add texture coordinates to the vertices above
	  - Assign the texture coordinates to a vertex buffer objects as discussed in class.
	    Note, they ned to be inside the same vbo in which the vertices are defined. 
	-----------------------------------------------------------------------------------*/



	glGenVertexArrays(2, &vaoID[0]); // Create our Vertex Array Object
	glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object so we can use it


	glGenBuffers(2, vboID); // Generate our Vertex Buffer Object

							// vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glEnableVertexAttribArray(0); //Enable the array for the vertices
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0); // Set up our vertex attributes pointer

	glEnableVertexAttribArray(1); // Enable the array for the texture vertices
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));


								  //Color
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), normals, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(1); // Enable the second vertex attribute array

	glBindVertexArray(0); // Disable our Vertex Buffer Object


	delete[] vertices; // Delete our vertices from memory
	delete[] normals; // And do the same to our colors.


	/*---------------------------------------------------------------------------------
	TODO:
	- Load the texture
	- Create a texture object
	- Set the correct texture unit active
	- Set all texture parameters as requried
	- Load the texture to your graphics card
	- Create a texture uniform variable
	-----------------------------------------------------------------------------------*/

	//file names and paths for the three textures we're using
	string texPath1 = "C:\\Users\\Arlo\\Desktop\\Stuff\\IAState Class Materials\\557\\Projects\\Homework_4\\p1\\tex_1.bmp";
	string texPath2 = "C:\\Users\\Arlo\\Desktop\\Stuff\\IAState Class Materials\\557\\Projects\\Homework_4\\p1\\tex_2.bmp";
	string texPath3 = "C:\\Users\\Arlo\\Desktop\\Stuff\\IAState Class Materials\\557\\Projects\\Homework_4\\p1\\tex_3.bmp";

	//FIRST TEXTURE! ****************************************
	unsigned char* data1 = ImgLoader::Load(texPath1); //Loading the first image and getting a pointer.

	//getting the location of our sampler2D
	int texLoc = glGetUniformLocation(program, "tex1");

	//Allocate the memory and associate the texture with a variable
	glActiveTexture(GL_TEXTURE0);
	
	GLuint _texture0;
	glGenTextures(0, &_texture0);

	//setting it as the active texture
	glBindTexture(GL_TEXTURE_2D, _texture0);
	glUniform1i(texLoc, 0);

	//Setting our texture units up
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//setting our texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);

	//Load the texture to the graphics hardware
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_BGR, GL_UNSIGNED_BYTE, data1);

	//SECOND TEXTURE! ****************************************
	unsigned char* data2 = ImgLoader::Load(texPath2); //Ditto with the second.

	//getting our uniform location
	texLoc = glGetUniformLocation(program, "tex2");
	
	//Allocate the memory and associate the texture with a variable
	glActiveTexture(GL_TEXTURE1);
	
	GLuint _texture1;
	glGenTextures(1, &_texture1);

	//setting it as the active texture
	glBindTexture(GL_TEXTURE_2D, _texture1);
	glUniform1i(texLoc, 1);

	//Setting our texture units up
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//setting our texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);

	//Load the texture to the graphics hardware
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_BGR, GL_UNSIGNED_BYTE, data2);

	//THIRD TEXTURE! ****************************************
	unsigned char* data3 = ImgLoader::Load(texPath3); //Ditto with the third.
	
	//getting our uniform location
	texLoc = glGetUniformLocation(program, "tex3");
	
	//Allocate the memory and associate the texture with a variable
	glActiveTexture(GL_TEXTURE2);
	
	GLuint _texture2;
	glGenTextures(2, &_texture2);

	//setting it as the active texture
	glBindTexture(GL_TEXTURE_2D, _texture2);
	glUniform1i(texLoc, 2);

	//Setting our texture units up
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//setting our texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);

	//Load the texture to the graphics hardware
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_BGR, GL_UNSIGNED_BYTE, data3);

}







/*!
 This code renders the plane
 */
void renderSquare(void)
{

	glUseProgram(program);

	glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object

	glDrawArrays(GL_TRIANGLES, 0, 6); // Draw our square

	glBindVertexArray(0); // Unbind our Vertex Array Object

	glUseProgram(0);

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*!
 This function creates the two models
 */
void setupScene(void) {
    
	createSquare();
    
}




int main(int argc, const char * argv[])
{
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
    
    
    // Init the glew api
    initGlew();
    
	// Prepares some defaults
	CoordSystemRenderer* coordinate_system_renderer = new CoordSystemRenderer(10.0);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// The Shader Program starts here
    
    // Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
    // Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
	static const string vertex_code = ShaderFileUtils::LoadFromFile("C:\\Users\\Arlo\\Desktop\\Stuff\\IAState Class Materials\\557\\Projects\\Homework_4\\p1\\hw4_shader.vs");
    static const char * vs_source = vertex_code.c_str();
    
    // Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
    static const string fragment_code = ShaderFileUtils::LoadFromFile("C:\\Users\\Arlo\\Desktop\\Stuff\\IAState Class Materials\\557\\Projects\\Homework_4\\p1\\hw4_shader.fs");
    static const char * fs_source = fragment_code.c_str();
    
    // This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
    program = glCreateProgram();
    
    // We create a shader with our fragment shader source code and compile it.
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
	bool ret = CheckShaderLocal(fs, GL_VERTEX_SHADER);
	if (!ret) { cout << "Problems compiling GL_VERTEX_SHADER" << endl; }
    
    // We create a shader with our vertex shader source code and compile it.
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
	ret = CheckShaderLocal(vs, GL_VERTEX_SHADER);
	if (!ret) { cout << "Problems compiling GL_VERTEX_SHADER" << endl; }
    
    // We'll attach our two compiled shaders to the OpenGL program.
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    glLinkProgram(program);
    
    // We'll specify that we want to use this program that we've attached the shaders to.
    glUseProgram(program);
    
    //// The Shader Program ends here
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    

    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    
    projectionMatrix = glm::perspective(1.1f, (float)800 / (float)600, 0.1f, 100.f);
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
    viewMatrix = glm::lookAt(glm::vec3(1.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    int projectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
    int viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
    int modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader
    
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader

    glBindAttribLocation(program, 0, "in_Position");
	glBindAttribLocation(program, 1, "in_TexCoord");
    glBindAttribLocation(program, 2, "in_Normal");
    

    
    // this creates the scene
    setupScene();
    
    int i=0;

    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
        // this draws the coordinate system
		coordinate_system_renderer->draw();
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This generate the object
        // Enable the shader program
        glUseProgram(program);
        
        // this changes the camera location
        glm::mat4 rotated_view = viewMatrix * GetRotationMatrix();
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
        
        // This moves the model 
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader

		// This line renders your Ppolygon model
		renderSquare();
        
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
	// delete the coordinate system object
	delete coordinate_system_renderer;

    // Program clean up when the window gets closed.
    glDeleteVertexArrays(2, vaoID);
    glDeleteProgram(program);
}

