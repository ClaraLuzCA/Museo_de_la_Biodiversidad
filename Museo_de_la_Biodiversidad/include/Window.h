#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLfloat getCamello_MovX() { return Camello_MovX; }
	GLfloat getCamello_MovY() { return Camello_MovY; }
	GLfloat getCamello_MovZ() { return Camello_MovZ; }
	GLfloat getCamello_MovCuerpo() { return Camello_MovCuerpo; }
	GLfloat getCamello_MovRodillaD() { return Camello_MovRodillaD; }
	GLfloat getCamello_MovRodillaT() { return Camello_MovRodillaT; }
	GLfloat getCamello_MovPiernaD() { return Camello_MovPiernaD; }
	GLfloat getCamello_MovPiernaT() { return Camello_MovPiernaT; }
	GLfloat getCamello_MovCuello() { return Camello_MovCuello; }
	GLfloat getCamello_MovCabeza() { return Camello_MovCabeza; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat Camello_MovX;
	GLfloat Camello_MovY;
	GLfloat Camello_MovZ;
	GLfloat Camello_MovCuerpo;
	GLfloat Camello_MovRodillaD;
	GLfloat Camello_MovRodillaT;
	GLfloat Camello_MovPiernaD;
	GLfloat Camello_MovPiernaT;
	GLfloat Camello_MovCuello;
	GLfloat Camello_MovCabeza;

	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

