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
	GLfloat getGorila_MovX() { return Gorila_MovX; }
	GLfloat getGorila_MovY() { return Gorila_MovY; }
	GLfloat getGorila_MovZ() { return Gorila_MovZ; }
	GLfloat getGorila_MovCuerpo() { return Gorila_MovCuerpo; }
	GLfloat getGorila_MovBrazo() { return Gorila_MovBrazo; }
	GLfloat getGorila_MovManoL() { return Gorila_MovManoL; }
	GLfloat getGorila_MovManoR() { return Gorila_MovManoR; }
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
	GLfloat Gorila_MovX;
	GLfloat Gorila_MovY;
	GLfloat Gorila_MovZ;
	GLfloat Gorila_MovCuerpo;
	GLfloat Gorila_MovBrazo;
	GLfloat Gorila_MovManoL;
	GLfloat Gorila_MovManoR;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

