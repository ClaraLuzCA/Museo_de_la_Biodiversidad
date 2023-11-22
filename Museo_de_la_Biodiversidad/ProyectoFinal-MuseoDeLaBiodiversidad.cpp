/*
Semestre 2024-1
Proyecto Final Museo de la Biodiversidad
*/

//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;
//variables animación
GLfloat earthRotationAngle = 0.0f;

//Variables para KeyFrames
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

float mov_alas = 0.0f;
float movMariposa=0.0f;
float mov_Patas = 0.0f;
float movRana=0.0f;

float movOffset;
float angulovaria = 0.0;
float rotM = 0.0f;
float rotR = 0.0f;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture Domo_Exterior;
Texture Domo_Interior;
Texture Tierra_Texture;
Texture MariposaTexture;
Texture GorilaTexture;
//Tigre
Texture TigreTexture;
//Vegetación Segunda Escena
Texture PlantaTaro;
Texture ArbolTexture;
Texture JungleTexture;

Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;
Model Dado_M;
Model Domos;
Model Tierra;

Model Lampara;
Model Helecho;
Model Monstera;
//Tigre
Model Tigre;
//Vegetación
Model Taro;
Model ArbolS;
Model JunglePlant;

// Gorila
Model CabezaGorila;
Model CuerpoGorila;
Model AnteBrazoRGorila;
Model AnteBrazoLGorila;
Model BrazoRGorila;
Model BrazoLGorila;
Model CaderaGorila;
Model MusloRGorila;
Model MusloLGorila;
Model PiernaRGorila;
Model PiernaLGorila;
Model PieRGorila;
Model PieLGorila;
Model Pivote;

//Camello
Model CabezaCamello;
Model CuerpoCamello;
Model RodillaDLCamello;
Model RodillaDRCamello;
Model RodillaTLCamello;
Model RodillaTRCamello;
Model PiernaDLCamello;
Model PiernaDRCamello;
Model PiernaTLCamello;
Model PiernaTRCamello;
Model CuelloCamello;

//CUERPO MARIPOSA
Model Cuerpo_Mariposa;
Model ala1;
Model ala2;
Model Mariposa;

//RANA
Model Rana;
Model Rana_p1;
Model Rana_p2;
Model Rana_p3;
Model Rana_p4;


Model Armadillo;

glm::mat4 tmpRotGorila = glm::mat4(1.0f);
glm::mat4 tmpCaderaGorila = glm::mat4(1.0f);
glm::mat4 tmpCuerpoGorila = glm::mat4(1.0f);
glm::mat4 tmpAnteBrazoLGorila = glm::mat4(1.0f);
glm::mat4 tmpAnteBrazoRGorila = glm::mat4(1.0f);
glm::mat4 tmpMusloLGorila = glm::mat4(1.0f);
glm::mat4 tmpMusloRGorila = glm::mat4(1.0f);
glm::mat4 tmpPiernaLGorila = glm::mat4(1.0f);
glm::mat4 tmpPiernaRGorila = glm::mat4(1.0f);
//Camello
//glm::mat4 tmpRotCamello = glm::mat4(1.0f);
glm::mat4 tmpCuerpoCamello = glm::mat4(1.0f);
glm::mat4 tmpRodillaTCamello = glm::mat4(1.0f);
glm::mat4 tmpRodillaDCamello = glm::mat4(1.0f);
glm::mat4 tmpCuelloCamello = glm::mat4(1.0f);

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";
// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//función para teclado de keyframes 
void inputKeyframes(bool* keys);

//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, -1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		1.0f, 0.0f, -1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		1.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		1.0f, 0.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);
	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);
	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);
	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);
}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

bool animacion = false;

//NEW// Keyframes

float	Gorila_MovX = 13.8000006,
		Gorila_MovY = 1.5f,
		Gorila_MovZ = 10.800004f,
		Gorila_MovCadera = 0.0f,
		Gorila_MovCuerpo = 0.0f,
		Gorila_MovBrazo = 0.0f,
		Gorila_MovManoL = 0.0f,
		Gorila_MovManoR = 0.0f;

float	Camello_MovX = 0.0f,
		Camello_MovY = 0.0f,
		Camello_MovZ = 0.0f,
		Camello_MovCuerpo = 0.0f,
		Camello_MovRodillaT = 0.0f,
		Camello_MovRodillaD = 0.0f,
		Camello_MovCuello = 0.0f,
		Camello_MovCabeza = 0.0f,
		Camello_MovPiernaD = 0.0f,
		Camello_MovPiernaT = 0.0f;

#define MAX_FRAMES 100
int i_max_steps = 120;
int i_curr_steps = 0;
typedef struct _frame
{
	float	Gorila_MovX,
			Gorila_MovXInc;
	float	Gorila_MovY,
			Gorila_MovYInc;
	float	Gorila_MovZ,
			Gorila_MovZInc;
	float	Gorila_MovCuerpo,
			Gorila_MovCuerpoInc;
	float	Gorila_MovBrazo,
			Gorila_MovBrazoInc;
	float	Gorila_MovManoL,
			Gorila_MovManoLInc;
	float	Gorila_MovManoR,
			Gorila_MovManoRInc;

	float	Camello_MovX,
			Camello_MovXInc;
	float	Camello_MovY,
			Camello_MovYInc;
	float	Camello_MovZ,
			Camello_MovZInc;
	float	Camello_MovCuerpo,
			Camello_MovCuerpoInc;
	float	Camello_MovRodillaD,
			Camello_MovRodillaDInc;
	float	Camello_MovRodillaT,
			Camello_MovRodillaTInc;
	float	Camello_MovPiernaD,
			Camello_MovPiernaDInc;
	float	Camello_MovPiernaT,
			Camello_MovPiernaTInc;
	float	Camello_MovCuello,
			Camello_MovCuelloInc;
	float	Camello_MovCabeza,
			Camello_MovCabezaInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 7;			//introducir datos

//start debe ser false para que inicie solo
bool start = false, play = false;
bool subeAla = true;
bool avanzarM = true; 
bool retrocederM = false;
bool rotM1 = true;
bool rotM2 = false;
bool subePata = true, bajaPata = false;
bool avanzarR = true;
bool retrocederR = false;
bool rotR1 = true;
bool rotR2 = false;
int playIndex = 0;

void saveFrame(void) //tecla L
{

	printf("frameindex %d\n", FrameIndex);

	/*
	KeyFrame[FrameIndex].Gorila_MovX = Gorila_MovX;
	KeyFrame[FrameIndex].Gorila_MovY = Gorila_MovY;
	KeyFrame[FrameIndex].Gorila_MovZ = Gorila_MovZ;
	KeyFrame[FrameIndex].Gorila_MovCuerpo = Gorila_MovCuerpo;
	KeyFrame[FrameIndex].Gorila_MovBrazo = Gorila_MovBrazo;
	KeyFrame[FrameIndex].Gorila_MovManoL = Gorila_MovManoL;
	KeyFrame[FrameIndex].Gorila_MovManoR = Gorila_MovManoR;
	*/

	KeyFrame[FrameIndex].Camello_MovCuerpo = Camello_MovCuerpo;
	KeyFrame[FrameIndex].Camello_MovCuello = Camello_MovCuello;
	KeyFrame[FrameIndex].Camello_MovCabeza = Camello_MovCabeza;
	KeyFrame[FrameIndex].Camello_MovRodillaD = Camello_MovRodillaD;
	KeyFrame[FrameIndex].Camello_MovRodillaT = Camello_MovRodillaT;
	KeyFrame[FrameIndex].Camello_MovPiernaT = Camello_MovPiernaT;
	KeyFrame[FrameIndex].Camello_MovPiernaD = Camello_MovPiernaD;


	FrameIndex++;
}

void resetElements(void) //Tecla 0
{
	/*
	Gorila_MovX = KeyFrame[0].Gorila_MovX;
	Gorila_MovY = KeyFrame[0].Gorila_MovY;
	Gorila_MovZ = KeyFrame[0].Gorila_MovZ;
	*/
	Gorila_MovCuerpo = KeyFrame[0].Gorila_MovCuerpo;
	Gorila_MovBrazo = KeyFrame[0].Gorila_MovBrazo;
	Gorila_MovManoL = KeyFrame[0].Gorila_MovManoL;
	Gorila_MovManoR = KeyFrame[0].Gorila_MovManoR;
	
	Camello_MovCuerpo = KeyFrame[0].Camello_MovCuerpo;
	Camello_MovCuello = KeyFrame[0].Camello_MovCuello;
	Camello_MovCabeza = KeyFrame[0].Camello_MovCabeza;
	Camello_MovRodillaD = KeyFrame[0].Camello_MovRodillaD;
	Camello_MovRodillaT = KeyFrame[0].Camello_MovRodillaT;
	Camello_MovPiernaD = KeyFrame[0].Camello_MovPiernaD;
	Camello_MovPiernaT = KeyFrame[0].Camello_MovPiernaT;
}

void interpolation(void)
{
	KeyFrame[playIndex].Gorila_MovXInc = (KeyFrame[playIndex + 1].Gorila_MovX - KeyFrame[playIndex].Gorila_MovX) / i_max_steps;
	KeyFrame[playIndex].Gorila_MovYInc = (KeyFrame[playIndex + 1].Gorila_MovY - KeyFrame[playIndex].Gorila_MovY) / i_max_steps;
	KeyFrame[playIndex].Gorila_MovZInc = (KeyFrame[playIndex + 1].Gorila_MovZ - KeyFrame[playIndex].Gorila_MovZ) / i_max_steps;
	KeyFrame[playIndex].Gorila_MovCuerpoInc = (KeyFrame[playIndex + 1].Gorila_MovCuerpo - KeyFrame[playIndex].Gorila_MovCuerpo) / i_max_steps;
	KeyFrame[playIndex].Gorila_MovBrazoInc = (KeyFrame[playIndex + 1].Gorila_MovBrazo - KeyFrame[playIndex].Gorila_MovBrazo) / i_max_steps;
	KeyFrame[playIndex].Gorila_MovManoLInc = (KeyFrame[playIndex + 1].Gorila_MovManoL - KeyFrame[playIndex].Gorila_MovManoL) / i_max_steps;
	KeyFrame[playIndex].Gorila_MovManoRInc = (KeyFrame[playIndex + 1].Gorila_MovManoR - KeyFrame[playIndex].Gorila_MovManoR) / i_max_steps;
	// Camello
	KeyFrame[playIndex].Camello_MovXInc = (KeyFrame[playIndex + 1].Camello_MovX - KeyFrame[playIndex].Camello_MovX) / i_max_steps;
	KeyFrame[playIndex].Camello_MovYInc = (KeyFrame[playIndex + 1].Camello_MovY - KeyFrame[playIndex].Camello_MovY) / i_max_steps;
	KeyFrame[playIndex].Camello_MovZInc = (KeyFrame[playIndex + 1].Camello_MovZ - KeyFrame[playIndex].Camello_MovZ) / i_max_steps;
	KeyFrame[playIndex].Camello_MovCuerpoInc = (KeyFrame[playIndex + 1].Camello_MovCuerpo - KeyFrame[playIndex].Camello_MovCuerpo) / i_max_steps;
	KeyFrame[playIndex].Camello_MovCuelloInc = (KeyFrame[playIndex + 1].Camello_MovCuello - KeyFrame[playIndex].Camello_MovCuello) / i_max_steps;
	KeyFrame[playIndex].Camello_MovCabezaInc = (KeyFrame[playIndex + 1].Camello_MovCabeza - KeyFrame[playIndex].Camello_MovCabeza) / i_max_steps;
	KeyFrame[playIndex].Camello_MovRodillaDInc = (KeyFrame[playIndex + 1].Camello_MovRodillaD - KeyFrame[playIndex].Camello_MovRodillaD) / i_max_steps;
	KeyFrame[playIndex].Camello_MovRodillaTInc = (KeyFrame[playIndex + 1].Camello_MovRodillaT - KeyFrame[playIndex].Camello_MovRodillaT) / i_max_steps;
	KeyFrame[playIndex].Camello_MovPiernaDInc = (KeyFrame[playIndex + 1].Camello_MovPiernaD - KeyFrame[playIndex].Camello_MovPiernaD) / i_max_steps;
	KeyFrame[playIndex].Camello_MovPiernaTInc = (KeyFrame[playIndex + 1].Camello_MovPiernaT - KeyFrame[playIndex].Camello_MovPiernaT) / i_max_steps;
}

void animate(void)
{
	
	if (!start) {
		resetElements();
		//First Interpolation				
		interpolation();
		//play = true;
		playIndex = 0;
		i_curr_steps = 0;
		//printf("Frame index= %d\n", FrameIndex);
		//printf("termino la animacion\n");
		playIndex = 0;
		//play = false;
		start = true;
		play = true;
	}
	//Movimiento del objeto con barra espaciadora
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //fin de animación entre frames?
		{
			playIndex++;
			//printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//Fin de toda la animación con último frame?
			{
				resetElements();
				//First Interpolation				
				interpolation();
				//play = true;
				playIndex = 0;
				i_curr_steps = 0;
				//printf("Frame index= %d\n", FrameIndex);
				//printf("termino la animacion\n");
				playIndex = 0;
				//play = false;
			}
			else //Interpolación del próximo cuadro
			{

				i_curr_steps = 0; //Resetea contador
				//Interpolar
				interpolation();
			}
		}
		else
		{
			//Dibujar Animación
			Gorila_MovX = KeyFrame[playIndex].Gorila_MovX;
			Gorila_MovY = KeyFrame[playIndex].Gorila_MovY;
			Gorila_MovZ = KeyFrame[playIndex].Gorila_MovZ;
			Gorila_MovCuerpo += KeyFrame[playIndex].Gorila_MovCuerpoInc;
			Gorila_MovBrazo += KeyFrame[playIndex].Gorila_MovBrazoInc;
			Gorila_MovManoL += KeyFrame[playIndex].Gorila_MovManoLInc;
			Gorila_MovManoR += KeyFrame[playIndex].Gorila_MovManoRInc;
			//Camello
			Camello_MovX = KeyFrame[playIndex].Camello_MovX;
			Camello_MovY = KeyFrame[playIndex].Camello_MovY;
			Camello_MovZ = KeyFrame[playIndex].Camello_MovZ;
			Camello_MovCuerpo += KeyFrame[playIndex].Camello_MovCuerpoInc;
			Camello_MovCuello += KeyFrame[playIndex].Camello_MovCuelloInc;
			Camello_MovCabeza += KeyFrame[playIndex].Camello_MovCabezaInc;
			Camello_MovRodillaD += KeyFrame[playIndex].Camello_MovRodillaDInc;
			Camello_MovRodillaT += KeyFrame[playIndex].Camello_MovRodillaTInc;
			Camello_MovPiernaD += KeyFrame[playIndex].Camello_MovPiernaDInc;
			Camello_MovPiernaT += KeyFrame[playIndex].Camello_MovPiernaTInc;

			i_curr_steps++;
		}

	}
	if (subeAla) {
		if (mov_alas <= 45.0f) {
			mov_alas +=0.8f;
		}
		else {
			subeAla = false;
		}
	}else {
		if (mov_alas >= -45.0f) {
			mov_alas -= 0.8f;
		}
		else {
			subeAla = true; 
		}
	}
	 if (subePata) {
		 if (mov_Patas <= 10.0f) {
			 mov_Patas += 0.7f;
		 }
		 else {
			 subePata = false;
			 bajaPata = true;
		 }
	 }
	 if (bajaPata) {
		 if (mov_Patas >= -10.0f) {
			 mov_Patas -= 0.7f;
		 }
		 else {
			 bajaPata = false;
			 subePata= true;
		 }
	 }
	 angulovaria += 10.0f * deltaTime;

	 if (avanzarM) {
		 if (movMariposa > -50.0f){
			 movMariposa -= 0.1 * deltaTime;

		 } else {
			 if (rotM1) {
				if (rotM < 180.0f) {
					rotM += 10.0f* deltaTime;
				}
				else {
					avanzarM = false;
					retrocederM = true;
					rotM1 = false;
					rotM2 = true;
				}
			 }
			 
			 
		 }
	 }
	 else {
		 if (retrocederM) {
			 if (movMariposa < 0.0f) {
				 movMariposa += 0.1 * deltaTime;
				 //printf("avanza%f \n ",movCoche);
			 }
			 else {
				 if (rotM2) {
					 if (rotM >0.0f) {
						 rotM -= 10.0 * deltaTime;
					 }
					 else {
						 avanzarM = true;
						 retrocederM = false;
						 rotM1 = true;
						 rotM2 = false;
					 }
				 }
			 }
		 }
	 }
	 if (avanzarR) {
		 if (movRana > -25.0f) {
			 movRana -= 0.1;;

		 }
		 else {
			 if (rotR1) {
				 if (rotR < 180.0f) {
					 rotR += 10.0f;
				 }
				 else {
					 avanzarR = false;
					 retrocederR = true;
					 rotR1 = false;
					 rotR2 = true;
				 }
			 }


		 }
	 }
	 else {
		 if (retrocederR) {
			 if (movRana < 0.0f) {
				 movRana += 0.1;;
			 }
			 else {
				 if (rotR2) {
					 if (rotR > 0.0f) {
						 rotR -= 10.0;
					 }
					 else {
						 avanzarR = true;
						 retrocederR = false;
						 rotR1 = true;
						 rotR2 = false;
					 }
				 }
			 }
		 }
	 }
	 

}

///////////////* FIN KEYFRAMES*////////////////////////////


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/TCom_Pavement_Terrazzo2_512_albedo.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	MariposaTexture = Texture("Textures/1.jpg");
	MariposaTexture.LoadTexture();
	//Cargamos textura del tigre
	TigreTexture = Texture("Textures/Tigre.jpeg");
	TigreTexture.LoadTextureA();

	//Cargamos textura del vegetación
	PlantaTaro = Texture("Textures/taroP.png");
	PlantaTaro.LoadTextureA();
	JungleTexture = Texture("Textures/ExtraPlant.png");
	JungleTexture.LoadTextureA();

	Domo_Exterior = Texture("Textures/Domo_exterior");
	Domo_Interior = Texture("Textures/Domo_interior");

	Tierra_Texture = Texture("Textures/Earth.jpg");

	//Texturas Gorila
	//GorilaTexture = Texture("Textures/Body-TM_u0_v0.png");
	//GorilaTexture.LoadTextureA();
	
	//RANA
	Rana = Model();
	Rana.LoadModel("Models/rana.obj");
	Rana_p1 = Model();
	Rana_p1.LoadModel("Models/rana_pda.obj");
	Rana_p2 = Model();
	Rana_p2.LoadModel("Models/rana_pia.obj");
	Rana_p3 = Model();
	Rana_p3.LoadModel("Models/rana_pde.obj");
	Rana_p4 = Model();
	Rana_p4.LoadModel("Models/rana_pie.obj");


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Domos = Model();
	Domos.LoadModel("Models/Domoobj.obj");
	Tierra = Model();
	Tierra.LoadModel("Models/TierraOBJ.obj");
	Mariposa = Model();
	Mariposa.LoadModel("Models/mariposa.obj");
	Lampara = Model();
	Lampara.LoadModel("Models/lampara.obj");
	Helecho = Model();
	Helecho.LoadModel("Models/helecho.obj");
	Monstera = Model();
	Monstera.LoadModel("Models/monsterra2.obj");

	//cuerpo mariposa
	Cuerpo_Mariposa = Model();
	Cuerpo_Mariposa.LoadModel("Models/cuerpo_mariposa.obj");
	ala1 = Model();
	ala1.LoadModel("Models/ala1.obj");
	ala2 = Model();
	ala2.LoadModel("Models/ala2.obj");

	//Cargamos Tigre
	Tigre = Model();
	Tigre.LoadModel("Models/TigreLoop.fbx");

	//Cargamos Vegetación Sala2
	Taro = Model();
	Taro.LoadModel("Models/Taro.fbx");
	ArbolS = Model();
	ArbolS.LoadModel("Models/Oldtree.fbx");
	JunglePlant = Model();
	JunglePlant.LoadModel("Models/ExtraPlant.fbx");
	
  
  //Gorila
	CabezaGorila = Model();
	CabezaGorila.LoadModel("Models/Gorila/Cabeza.obj");

	CuerpoGorila = Model();
	CuerpoGorila.LoadModel("Models/Gorila/Cuerpo.obj");

	AnteBrazoLGorila = Model();
	AnteBrazoLGorila.LoadModel("Models/Gorila/AnteBrazoL.obj");

	AnteBrazoRGorila = Model();
	AnteBrazoRGorila.LoadModel("Models/Gorila/AnteBrazoR.obj");

	BrazoLGorila = Model();
	BrazoLGorila.LoadModel("Models/Gorila/BrazoL.obj");

	BrazoRGorila = Model();
	BrazoRGorila.LoadModel("Models/Gorila/BrazoR.obj");

	CaderaGorila = Model();
	CaderaGorila.LoadModel("Models/Gorila/Cadera.obj");

	MusloLGorila = Model();
	MusloLGorila.LoadModel("Models/Gorila/MusloL.obj");

	MusloRGorila = Model();
	MusloRGorila.LoadModel("Models/Gorila/MusloR.obj");

	PiernaLGorila = Model();
	PiernaLGorila.LoadModel("Models/Gorila/PiernaL.obj");

	PiernaRGorila = Model();
	PiernaRGorila.LoadModel("Models/Gorila/PiernaR.obj");

	PieLGorila = Model();
	PieLGorila.LoadModel("Models/Gorila/PieL.obj");

	PieRGorila = Model();
	PieRGorila.LoadModel("Models/Gorila/PieR.obj");

	Pivote = Model();
	Pivote.LoadModel("Models/Gorila/pivote.obj");

	//Camello
	CabezaCamello = Model();
	CabezaCamello.LoadModel("Models/Camello/Cabeza.obj");

	CuelloCamello = Model();
	CuelloCamello.LoadModel("Models/Camello/Cuello.obj");

	CuerpoCamello = Model();
	CuerpoCamello.LoadModel("Models/Camello/Cuerpo.obj");

	RodillaDLCamello = Model();
	RodillaDLCamello.LoadModel("Models/Camello/DL_Rodilla.obj");

	RodillaDRCamello = Model();
	RodillaDRCamello.LoadModel("Models/Camello/DR_Rodilla.obj");

	RodillaTLCamello = Model();
	RodillaTLCamello.LoadModel("Models/Camello/TL_Rodilla.obj");

	RodillaTRCamello = Model();
	RodillaTRCamello.LoadModel("Models/Camello/TR_Rodilla.obj");

	PiernaDLCamello = Model();
	PiernaDLCamello.LoadModel("Models/Camello/DL_Pata.obj");

	PiernaDRCamello = Model();
	PiernaDRCamello.LoadModel("Models/Camello/DR_Pata.obj");

	PiernaTLCamello = Model();
	PiernaTLCamello.LoadModel("Models/Camello/TL_Pata.obj");

	PiernaTRCamello = Model();
	PiernaTRCamello.LoadModel("Models/Camello/TR_Pata.obj");

	Armadillo = Model();
	Armadillo.LoadModel("Models/Armadillo/Armadillo.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/majesty_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/majesty_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/majesty_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/majesty_up.tga");
	skyboxFaces.push_back("Textures/Skybox/majesty_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/majesty_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);//CEF BRILLO
	Material_opaco = Material(0.3f, 4);

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(0.4f, 0.4f, 0.4f,
		0.6f, 0.6f,
		0.0f, -1.0f, 0.0f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	unsigned int spotLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.79f, 0.156f,
		2.5f, 2.5f,
		-70.0f, 60.0f, 0.0f,
		0.3f, 0.2f, 0.001f);
	pointLightCount++;
	/*
	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
		0.0f, 0.0f,
		20.0f, 7.5f, -35.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;*/

	//linterna
	//spotLightCount++;
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		30.0f, 8.0f,
		90.0f, 10.0f, 45.0f,
		-45.0f, 50.0f, 0.0f,
		0.02f, 0.2f, 0.05f,
		50.0f);
	spotLightCount++;
	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		5.0f, 4.0f,
		20.0f, 13.0f, -35.0f,
		180.0f, 50.0f, -180.0f,
		0.02f, 0.2f, 0.05f,
		70.0f);
	spotLightCount++;
	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
		5.0f, 4.0f,
		18.0f, 12.0f, 45.0f,
		180.0f, 50.0f, -180.0f,
		0.02f, 0.2f, 0.05f,
		70.0f);
	spotLightCount++;
	spotLights[3] = SpotLight(1.0f, 1.0f, 1.0f,
		30.0f, 8.0f,
		90.0f, 12.0f, -35.0f,
		-90.0f, 100.0f, 0.0f,
		0.02f, 0.2f, 0.05f,
		70.0f);
	spotLightCount++;
	/*
	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		0.0f, 0.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(0.0f, 1.0f, 1.0f,
		0.0f, 0.0f,
		-15.0f, 10.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//se crean mas luces puntuales y spotlight

	*/
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	//KeyFrames Guardados

	KeyFrame[0].Gorila_MovX = 13.8000006;
	KeyFrame[0].Gorila_MovY = 1.5;
	KeyFrame[0].Gorila_MovZ = 10.800004;
	KeyFrame[0].Gorila_MovCuerpo = 24.299986;
	KeyFrame[0].Gorila_MovBrazo = -61.499893;
	KeyFrame[0].Gorila_MovManoL = -130.800568;
	KeyFrame[0].Gorila_MovManoR = 52.499916;

	KeyFrame[0].Camello_MovX = -10.10002;
	KeyFrame[0].Camello_MovY = 1.1;
	KeyFrame[0].Camello_MovZ = 6.650007;
	KeyFrame[0].Camello_MovCuerpo = 2.099999;
	KeyFrame[0].Camello_MovCuello = 0.0;
	KeyFrame[0].Camello_MovCabeza = 0.0;
	KeyFrame[0].Camello_MovRodillaD = 0.0;
	KeyFrame[0].Camello_MovRodillaT = 0.0;
	KeyFrame[0].Camello_MovPiernaD = 0.0;
	KeyFrame[0].Camello_MovPiernaT = 0.0;

	KeyFrame[1].Gorila_MovX = 13.8000006;
	KeyFrame[1].Gorila_MovY = 1.5;
	KeyFrame[1].Gorila_MovZ = 10.800004;
	KeyFrame[1].Gorila_MovCuerpo = 6.600;
	KeyFrame[1].Gorila_MovBrazo = -61.499893;
	KeyFrame[1].Gorila_MovManoL = -61.499897;
	KeyFrame[1].Gorila_MovManoR = 122.400482;

	KeyFrame[1].Camello_MovX = -10.10002;
	KeyFrame[1].Camello_MovY = 1.1;
	KeyFrame[1].Camello_MovZ = 6.650007;
	KeyFrame[1].Camello_MovCuerpo = 13.200005;
	KeyFrame[1].Camello_MovCuello = 38.699951;
	KeyFrame[1].Camello_MovCabeza = 0.0;
	KeyFrame[1].Camello_MovRodillaD = -77.100021;
	KeyFrame[1].Camello_MovRodillaT = 2.70;
	KeyFrame[1].Camello_MovPiernaD = 41.999943;
	KeyFrame[1].Camello_MovPiernaT = 0.0;

	KeyFrame[2].Gorila_MovX = 13.8000006;
	KeyFrame[2].Gorila_MovY = 1.5;
	KeyFrame[2].Gorila_MovZ = 10.800004;
	KeyFrame[2].Gorila_MovCuerpo = 24.299986;
	KeyFrame[2].Gorila_MovBrazo = -61.499893;
	KeyFrame[2].Gorila_MovManoL = -130.800568;
	KeyFrame[2].Gorila_MovManoR = 52.499916;

	KeyFrame[2].Camello_MovX = -10.10002;
	KeyFrame[2].Camello_MovY = 1.1;
	KeyFrame[2].Camello_MovZ = 6.650007;
	KeyFrame[2].Camello_MovCuerpo = 13.200005;
	KeyFrame[2].Camello_MovCuello = 38.699951;
	KeyFrame[2].Camello_MovCabeza = -22.199991;
	KeyFrame[2].Camello_MovRodillaD = -77.100021;
	KeyFrame[2].Camello_MovRodillaT = 2.70;
	KeyFrame[2].Camello_MovPiernaD = 41.999943;
	KeyFrame[2].Camello_MovPiernaT = 0.0;

	KeyFrame[3].Gorila_MovX = 13.8000006;
	KeyFrame[3].Gorila_MovY = 1.5;
	KeyFrame[3].Gorila_MovZ = 10.800004;
	KeyFrame[3].Gorila_MovCuerpo = 6.600;
	KeyFrame[3].Gorila_MovBrazo = -61.499893;
	KeyFrame[3].Gorila_MovManoL = -61.499897;
	KeyFrame[3].Gorila_MovManoR = 122.400482;

	KeyFrame[3].Camello_MovX = -10.10002;
	KeyFrame[3].Camello_MovY = 1.1;
	KeyFrame[3].Camello_MovZ = 6.650007;
	KeyFrame[3].Camello_MovCuerpo = 13.200005;
	KeyFrame[3].Camello_MovCuello = 38.699951;
	KeyFrame[3].Camello_MovCabeza = 5.100001;
	KeyFrame[3].Camello_MovRodillaD = -77.100021;
	KeyFrame[3].Camello_MovRodillaT = 2.70;
	KeyFrame[3].Camello_MovPiernaD = 41.999943;
	KeyFrame[3].Camello_MovPiernaT = 0.0;

	KeyFrame[4].Gorila_MovX = 13.8000006;
	KeyFrame[4].Gorila_MovY = 1.5;
	KeyFrame[4].Gorila_MovZ = 10.800004;
	KeyFrame[4].Gorila_MovCuerpo = 24.299986;
	KeyFrame[4].Gorila_MovBrazo = -61.499893;
	KeyFrame[4].Gorila_MovManoL = -130.800568;
	KeyFrame[4].Gorila_MovManoR = 52.499916;

	KeyFrame[4].Camello_MovX = -10.10002;
	KeyFrame[4].Camello_MovY = 1.1;
	KeyFrame[4].Camello_MovZ = 6.650007;
	KeyFrame[4].Camello_MovCuerpo = 2.099999;
	KeyFrame[4].Camello_MovCuello = 0.0;
	KeyFrame[4].Camello_MovCabeza = 0.0;
	KeyFrame[4].Camello_MovRodillaD = 0.0;
	KeyFrame[4].Camello_MovRodillaT = 0.0;
	KeyFrame[4].Camello_MovPiernaD = 0.0;
	KeyFrame[4].Camello_MovPiernaT = 0.0;

	KeyFrame[5].Gorila_MovX = 13.8000006;
	KeyFrame[5].Gorila_MovY = 1.5;
	KeyFrame[5].Gorila_MovZ = 10.800004;
	KeyFrame[5].Gorila_MovCuerpo = 6.600;
	KeyFrame[5].Gorila_MovBrazo = -61.499893;
	KeyFrame[5].Gorila_MovManoL = -61.499897;
	KeyFrame[5].Gorila_MovManoR = 122.400482;

	KeyFrame[5].Camello_MovX = -10.10002;
	KeyFrame[5].Camello_MovY = 1.1;
	KeyFrame[5].Camello_MovZ = 6.650007;
	KeyFrame[5].Camello_MovCuerpo = 2.099999;
	KeyFrame[5].Camello_MovCuello = 0.0;
	KeyFrame[5].Camello_MovCabeza = 0.0;
	KeyFrame[5].Camello_MovRodillaD = 0.0;
	KeyFrame[5].Camello_MovRodillaT = 0.0;
	KeyFrame[5].Camello_MovPiernaD = 0.0;
	KeyFrame[5].Camello_MovPiernaT = 0.0;

	KeyFrame[6].Gorila_MovX = 13.8000006;
	KeyFrame[6].Gorila_MovY = 1.5;
	KeyFrame[6].Gorila_MovZ = 10.800004;
	KeyFrame[6].Gorila_MovCuerpo = 24.299986;
	KeyFrame[6].Gorila_MovBrazo = -61.499893;
	KeyFrame[6].Gorila_MovManoL = -130.800568;
	KeyFrame[6].Gorila_MovManoR = 52.499916;

	KeyFrame[6].Camello_MovX = -10.10002;
	KeyFrame[6].Camello_MovY = 1.1;
	KeyFrame[6].Camello_MovZ = 6.650007;
	KeyFrame[6].Camello_MovCuerpo = 2.099999;
	KeyFrame[6].Camello_MovCuello = 0.0;
	KeyFrame[6].Camello_MovCabeza = 0.0;
	KeyFrame[6].Camello_MovRodillaD = 0.0;
	KeyFrame[6].Camello_MovRodillaT = 0.0;
	KeyFrame[6].Camello_MovPiernaD = 0.0;
	KeyFrame[6].Camello_MovPiernaT = 0.0;
	//Se agregan nuevos frames 
	printf("\nTeclas para uso de Keyframes:\n1.-Presionar barra espaciadora para reproducir animacion.\n2.-Presionar 0 para volver a habilitar reproduccion de la animacion\n");
	printf("3.-Presiona L para guardar frame\n4.-Presiona P para habilitar guardar nuevo frame\n5.-Presiona 1 para mover en X\n6.-Presiona 2 para habilitar mover en X");

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//Instancia del domo
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		Domo_Exterior.UseTexture();
		Domo_Interior.UseTexture();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Domos.RenderModel();
		earthRotationAngle += 0.5f * deltaTime;

		//Instancia de la Tierra
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 0.5f, 5.0f));
		model = glm::scale(model, glm::vec3(0.50f, 0.50f, 0.50f));
		model = glm::rotate(model, earthRotationAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Tierra_Texture.UseTexture();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tierra.RenderModel();

		//mariposa centro
		//cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f+movMariposa, 25.5f+ mov_alas*(0.1), 5.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, glm::radians(rotM), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cuerpo_Mariposa.RenderModel();
		//alas
		model = modelaux;
		model = glm::rotate(model, glm::radians(mov_alas), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ala1.RenderModel();
		
		model = modelaux;
		model = glm::rotate(model, glm::radians(-mov_alas), glm::vec3(1.0f,0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ala2.RenderModel();

		//mariposa atras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, 40.5f, 5.0f));
		model = glm::scale(model, glm::vec3(0.40f, 0.40f, 0.40f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mariposa.RenderModel();
		//mariposa enfrente
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, 45.5f, 5.0f));
		model = glm::scale(model, glm::vec3(0.40f, 0.40f, 0.40f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mariposa.RenderModel();

		//mariposa enfrente 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, 35.5f, 15.0f));
		model = glm::scale(model, glm::vec3(0.30f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mariposa.RenderModel();
		//mariposa centro derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 50.5f, 25.0f));
		model = glm::scale(model, glm::vec3(0.50f, 0.50f, 0.50f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mariposa.RenderModel();
		//mariposa centro izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 50.5f, -25.0f));
		model = glm::scale(model, glm::vec3(0.50f, 0.50f, 0.50f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mariposa.RenderModel();

		//LAMPARAS
		//sala1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 7.5f, 45.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 225 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.0f, 7.5f, 45.0f));
		model = glm::scale(model, glm::vec3(0.20f, 0.20f, 0.20f));
		model = glm::rotate(model, 115 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.0f, 7.5f, -35.0f));
		model = glm::scale(model, glm::vec3(0.20f, 0.20f, 0.20f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Lampara.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 7.5f, -35.0f));
		model = glm::scale(model, glm::vec3(0.20f, 0.20f, 0.20f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();
		//sala2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, 7.5f, 35.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.10f, 0.10f));
		model = glm::rotate(model, 225 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 7.5f, 35.0f));
		model = glm::scale(model, glm::vec3(0.10f, 0.10f, 0.10f));
		model = glm::rotate(model, 115 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 7.5f, -35.0f));
		model = glm::scale(model, glm::vec3(0.10f, 0.10f, 0.10f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Lampara.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, 7.5f, -35.0f));
		model = glm::scale(model, glm::vec3(0.10f, 0.10f, 0.10f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();
    
		//Helechos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 7.5f, 35.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helecho.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helecho.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		Helecho.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(30.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helecho.RenderModel();
		
		model = modelaux;
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helecho.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helecho.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(60.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helecho.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helecho.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Helecho.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helecho.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helecho.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 7.5f, -35.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helecho.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helecho.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Helecho.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(30.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helecho.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helecho.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helecho.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(60.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helecho.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helecho.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Helecho.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helecho.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helecho.RenderModel();

		//Monstera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 7.5f, 45.0f));
		model = glm::scale(model, glm::vec3(0.9f, 1.5f, 0.9f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Monstera.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(35.0f, 7.5f, 45.0f));
		model = glm::scale(model, glm::vec3(0.9f, 1.5f, 0.9f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Monstera.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(65.0f, 7.5f, 60.0f));
		model = glm::scale(model, glm::vec3(0.9f, 1.5f, 0.9f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Monstera.RenderModel();

		//Monstera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 7.5f, -45.0f));
		model = glm::scale(model, glm::vec3(0.9f, 1.5f, 0.9f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Monstera.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(35.0f, 7.5f, -45.0f));
		model = glm::scale(model, glm::vec3(0.9f, 1.5f, 0.9f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Monstera.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(65.0f, 7.5f, -62.0f));
		model = glm::scale(model, glm::vec3(0.9f, 1.5f, 0.9f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Monstera.RenderModel();

		//----------
		//Vegetación Sala 2 Parte 1
		//----------
		float angleCactus = 180.0f; // Ángulo de rotación en grados
		float angleCactusR = glm::radians(angleCactus); // Convertir a radianes
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, 8.0f, 35.0f));
		model = glm::rotate(model, angleCactusR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 8.0f, 42.0f));
		model = glm::rotate(model, angleCactusR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-75.0f, 8.0f, 35.0f));
		model = glm::rotate(model, angleCactusR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-95.0f, 8.0f, 54.0f));
		model = glm::rotate(model, angleCactusR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-59.0f, 8.0f, 65.0f));
		model = glm::rotate(model, angleCactusR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 8.0f, 55.0f));
		model = glm::rotate(model, angleCactusR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-105.0f, 8.0f, 45.0f));
		model = glm::rotate(model, angleCactusR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, 8.0f, 65.0f));
		model = glm::rotate(model, angleCactusR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		// ------------------
		// Vegetación Planta Extra 
		// ------------------
		float anglePlanta = 180.0f; // Ángulo de rotación en grados
		float anglePlantaR = glm::radians(anglePlanta); // Convertir a radianes
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 8.0f, 40.0f));
		model = glm::rotate(model, anglePlantaR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JungleTexture.UseTexture();
		JunglePlant.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-56.0f, 8.0f, 50.0f));
		model = glm::rotate(model, anglePlantaR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JungleTexture.UseTexture();
		JunglePlant.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, 8.0f, 35.0f));
		model = glm::rotate(model, anglePlantaR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JungleTexture.UseTexture();
		JunglePlant.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-110.0f, 8.0f, 45.0f));
		model = glm::rotate(model, anglePlantaR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JungleTexture.UseTexture();
		JunglePlant.RenderModel();

		//----------
		//Vegetación Sala 2 Parte 2
		//----------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, 8.0f, -35.0f));
		model = glm::rotate(model, angleCactusR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 8.0f, -42.0f));
		model = glm::rotate(model, angleCactusR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-75.0f, 8.0f, -35.0f));
		model = glm::rotate(model, angleCactusR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-95.0f, 8.0f, -54.0f));
		model = glm::rotate(model, angleCactusR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-59.0f, 8.0f, -65.0f));
		model = glm::rotate(model, angleCactusR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 8.0f, -55.0f));
		model = glm::rotate(model, angleCactusR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-105.0f, 8.0f, -45.0f));
		model = glm::rotate(model, angleCactusR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, 8.0f, -65.0f));
		model = glm::rotate(model, angleCactusR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		// ------------------
		// Vegetación Planta Extra 
		// ------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 8.0f, -40.0f));
		model = glm::rotate(model, anglePlantaR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JungleTexture.UseTexture();
		JunglePlant.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-56.0f, 8.0f, -50.0f));
		model = glm::rotate(model, anglePlantaR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JungleTexture.UseTexture();
		JunglePlant.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, 8.0f, -35.0f));
		model = glm::rotate(model, anglePlantaR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JungleTexture.UseTexture();
		JunglePlant.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-110.0f, 8.0f, -45.0f));
		model = glm::rotate(model, anglePlantaR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JungleTexture.UseTexture();
		JunglePlant.RenderModel();

		//Tigre
		float angleInDegrees = 180.0f; // Ángulo de rotación en grados
		float angleInRadians = glm::radians(angleInDegrees); // Convertir a radianes
		// Crear la matriz de transformación
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(35.0f, 7.5f, 40.0f));
		model = glm::rotate(model, angleInRadians, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		// Enviar la matriz al shader y renderizar el modelo
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TigreTexture.UseTexture();
		Tigre.RenderModel();

		// -------------------------------------------------------------------------------------------------------------------------
		// Gorila
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::translate(model, glm::vec3(Gorila_MovX, Gorila_MovY, Gorila_MovZ));
		tmpRotGorila = model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 1.9f, 0.0f));
		tmpCaderaGorila = model = glm::rotate(model, glm::radians(Gorila_MovCuerpo), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, -1.9f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CaderaGorila.RenderModel();

		model = glm::translate(tmpCaderaGorila, glm::vec3(0.0f, -1.9f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 2.10f, 0.0f));
		tmpCuerpoGorila = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, -2.10f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuerpoGorila.RenderModel();

		model = glm::translate(tmpCuerpoGorila, glm::vec3(0.0f, -2.10f, 0.0f));
		model = glm::translate(model, glm::vec3(0.7f, 3.899f, -0.3f));
		tmpAnteBrazoLGorila = model = glm::rotate(model, glm::radians(Gorila_MovBrazo), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.7f, -3.899f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AnteBrazoLGorila.RenderModel();

		model = glm::translate(tmpCuerpoGorila, glm::vec3(0.0f, -2.10f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.7f, 3.899f, -0.3f));
		tmpAnteBrazoRGorila = model = glm::rotate(model, glm::radians(Gorila_MovBrazo), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.7f, -3.899f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AnteBrazoRGorila.RenderModel();

		model = glm::translate(tmpRotGorila, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpMusloLGorila = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MusloLGorila.RenderModel();

		model = glm::translate(tmpRotGorila, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpMusloRGorila = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MusloRGorila.RenderModel();

		model = glm::translate(tmpCuerpoGorila, glm::vec3(0.0f, -2.10, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CabezaGorila.RenderModel();

		model = glm::translate(tmpAnteBrazoLGorila, glm::vec3(-0.7f, -3.899f, 0.3f));
		model = glm::translate(model, glm::vec3(1.7f, 3.199f, -0.50f));
		model = glm::rotate(model, glm::radians(Gorila_MovManoL), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-1.7f, -3.199f, 0.50f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BrazoLGorila.RenderModel();

		model = glm::translate(tmpAnteBrazoRGorila, glm::vec3(0.7f, -3.899f, 0.3f));
		model = glm::translate(model, glm::vec3(-1.7f, 3.199f, -0.50f));
		model = glm::rotate(model, glm::radians(Gorila_MovManoR), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(1.7f, -3.199f, 0.50f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BrazoRGorila.RenderModel();

		model = glm::translate(tmpMusloLGorila, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpPiernaLGorila = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PiernaLGorila.RenderModel();

		model = glm::translate(tmpMusloRGorila, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpPiernaRGorila = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PiernaRGorila.RenderModel();

		model = glm::translate(tmpPiernaLGorila, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PieLGorila.RenderModel();

		model = glm::translate(tmpPiernaRGorila, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PieRGorila.RenderModel();
		// -------------------------------------------------------------------------------------------------------------------------
		// Camello
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::translate(model, glm::vec3(Camello_MovX, Camello_MovY, Camello_MovZ));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-0.05f, 1.45f, -0.55f));
		tmpCuerpoCamello = model = glm::rotate(model, glm::radians(Camello_MovCuerpo), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.05f, -1.45f, 0.55f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuerpoCamello.RenderModel();

		model = glm::translate(tmpCuerpoCamello, glm::vec3(0.05f, -1.45f, 0.55f));
		model = glm::translate(model, glm::vec3(-0.05f, 0.95f, -0.55));
		model = glm::rotate(model, glm::radians(Camello_MovRodillaT), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.05f, -0.95f, 0.55f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RodillaTLCamello.RenderModel();

		model = glm::translate(tmpCuerpoCamello, glm::vec3(0.05f, -1.45f, 0.55f));
		model = glm::translate(model, glm::vec3(-0.05f, 0.95f, -0.55));
		tmpRodillaTCamello = model = glm::rotate(model, glm::radians(Camello_MovRodillaT), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.05f, -0.95f, 0.55));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RodillaTRCamello.RenderModel();

		model = glm::translate(tmpRodillaTCamello, glm::vec3(0.05f, -0.95f, 0.55f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -0.65f));
		model = glm::rotate(model, glm::radians(Camello_MovPiernaT), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.65f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PiernaTLCamello.RenderModel();

		model = glm::translate(tmpRodillaTCamello, glm::vec3(0.05f, -0.95f, 0.55));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -0.65f));
		model = glm::rotate(model, glm::radians(Camello_MovPiernaT), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.65f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PiernaTRCamello.RenderModel();
		//
		model = glm::translate(tmpCuerpoCamello, glm::vec3(0.05f, -1.45f, 0.55f));
		model = glm::translate(model, glm::vec3(0.0f, 0.85f, 0.45f));
		tmpRodillaDCamello = model = glm::rotate(model, glm::radians(Camello_MovRodillaD), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.85f, -0.45f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RodillaDLCamello.RenderModel();

		model = glm::translate(tmpCuerpoCamello, glm::vec3(0.05f, -1.45f, 0.55f));
		model = glm::translate(model, glm::vec3(0.0f, 0.85f, 0.45f));
		model = glm::rotate(model, glm::radians(Camello_MovRodillaD), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.85f, -0.45f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RodillaDRCamello.RenderModel();

		model = glm::translate(tmpRodillaDCamello, glm::vec3(0.0f, -0.85f, -0.45f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.4f));
		model = glm::rotate(model, glm::radians(Camello_MovPiernaD), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PiernaDLCamello.RenderModel();

		model = glm::translate(tmpRodillaDCamello, glm::vec3(0.0f, -0.85f, -0.45f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.4f));
		model = glm::rotate(model, glm::radians(Camello_MovPiernaD), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PiernaDRCamello.RenderModel();

		model = glm::translate(tmpCuerpoCamello, glm::vec3(0.05f, -1.45f, 0.55f));
		model = glm::translate(model, glm::vec3(0.0f, 1.3f, 0.75f));
		tmpCuelloCamello = model = rotate(model, glm::radians(Camello_MovCuello), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -1.3f, -0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuelloCamello.RenderModel();

		model = glm::translate(tmpCuelloCamello, glm::vec3(0.0f, -1.3f, -0.75f));
		model = glm::translate(model, glm::vec3(0.0f, 1.7f, 1.4f));
		model = rotate(model, glm::radians(Camello_MovCabeza), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -1.7f, -1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CabezaCamello.RenderModel();

		//model = glm::scale(glm::mat4(1.0f), glm::vec3(2.5f,2.5f,2.5f));
		model = glm::translate(model, glm::vec3(Camello_MovX, Camello_MovY, Camello_MovZ));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Pivote.RenderModel();

		//rana
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f+(movRana), 9.5f+mov_Patas*(0.1), -45.0f));
		model = glm::scale(model, glm::vec3(0.10f, 0.10f, 0.10f));
		model = glm::rotate(model, (rotR-90) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rana.RenderModel();
		model = modelaux;
		model = glm::rotate(model, mov_Patas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rana_p1.RenderModel();

		model = modelaux;
		model = glm::rotate(model, mov_Patas * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rana_p2.RenderModel();

		model = modelaux;
		model = glm::rotate(model, (mov_Patas-10) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rana_p3.RenderModel();

		model = modelaux;
		model = glm::rotate(model, (mov_Patas - 10) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rana_p4.RenderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f,1.0f,1.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Armadillo.RenderModel();


		glUseProgram(0);

		mainWindow.swapBuffers();
	}
	return 0;
}

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_J])
		Camello_MovX = (mainWindow.getCamello_MovX());
	if (keys[GLFW_KEY_L])
		Camello_MovX = (mainWindow.getCamello_MovX());
	if (keys[GLFW_KEY_I])
		Camello_MovY = (mainWindow.getCamello_MovY());
	if (keys[GLFW_KEY_K])
		Camello_MovY = (mainWindow.getCamello_MovY());
	if (keys[GLFW_KEY_Y])
		Camello_MovZ = (mainWindow.getCamello_MovZ());
	if (keys[GLFW_KEY_H])
		Camello_MovZ = (mainWindow.getCamello_MovZ());
	if (keys[GLFW_KEY_1])
		Camello_MovCuerpo = (mainWindow.getCamello_MovCuerpo());
	if (keys[GLFW_KEY_2])
		Camello_MovCuerpo = (mainWindow.getCamello_MovCuerpo());
	if (keys[GLFW_KEY_3])
		Camello_MovRodillaD = (mainWindow.getCamello_MovRodillaD());
	if (keys[GLFW_KEY_4])
		Camello_MovRodillaD = (mainWindow.getCamello_MovRodillaD());
	if (keys[GLFW_KEY_5])
		Camello_MovRodillaT = (mainWindow.getCamello_MovRodillaT());
	if (keys[GLFW_KEY_6])
		Camello_MovRodillaT = (mainWindow.getCamello_MovRodillaT());
	if (keys[GLFW_KEY_7])
		Camello_MovPiernaD = (mainWindow.getCamello_MovPiernaD());
	if (keys[GLFW_KEY_8])
		Camello_MovPiernaD = (mainWindow.getCamello_MovPiernaD());
	if (keys[GLFW_KEY_B])
		Camello_MovPiernaT = (mainWindow.getCamello_MovPiernaT());
	if (keys[GLFW_KEY_N])
		Camello_MovPiernaT = (mainWindow.getCamello_MovPiernaT());
	if (keys[GLFW_KEY_Z])
		Camello_MovCuello = (mainWindow.getCamello_MovCuello());
	if (keys[GLFW_KEY_X])
		Camello_MovCuello = (mainWindow.getCamello_MovCuello());
	if (keys[GLFW_KEY_C])
		Camello_MovCabeza = (mainWindow.getCamello_MovCabeza());
	if (keys[GLFW_KEY_V])
		Camello_MovCabeza = (mainWindow.getCamello_MovCabeza());

	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;

			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1 && reproduciranimacion>0)
		{
			printf("Ya puedes reproducir de nuevo la animación con la tecla de barra espaciadora'\n");
			reproduciranimacion = 0;

		}
	}

	if (keys[GLFW_KEY_M])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			/*
			printf("Gorila_MovX es: %f\n", Gorila_MovX);
			printf("Gorila_MovY es: %f\n", Gorila_MovY);
			printf("Gorila_MovZ es: %f\n", Gorila_MovZ);
			printf("Gorila_MovCuerpo es: %f\n", Gorila_MovCuerpo);
			printf("Gorila_MovBrazo es: %f\n", Gorila_MovBrazo);
			printf("Gorila_MovManoL es: %f\n", Gorila_MovManoL);
			printf("Gorila_MovManoR es: %f\n", Gorila_MovManoR);
			*/
			printf("Camello_MovX es: %f\n", Camello_MovX);
			printf("Camello_MovY es: %f\n", Camello_MovY);
			printf("Camello_MovZ es: %f\n", Camello_MovZ);
			printf("Camello_MovCuerpo es: %f\n", Camello_MovCuerpo);
			printf("Camello_MovCuello es: %f\n", Camello_MovCuello);
			printf("Camello_MovCabeza es: %f\n", Camello_MovCabeza);
			printf("Camello_MovRodillaD es: %f\n", Camello_MovRodillaD);
			printf("Camello_MovRodillaT es: %f\n", Camello_MovRodillaT);
			printf("Camello_MovPiernaD es: %f\n", Camello_MovPiernaD);
			printf("Camello_MovPiernaT es: %f\n", Camello_MovPiernaT);

			printf("presiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1 && guardoFrame>0)
		{
			guardoFrame = 0;
			printf("Ya puedes guardar otro frame presionando la tecla L'\n");
		}
	}
}