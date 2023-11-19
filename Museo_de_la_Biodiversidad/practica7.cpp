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
#include"Model.h"
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

Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;
Model Dado_M;
Model Domos;
Model Tierra;
Model Mariposa;
Model Lampara;

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

glm::mat4 tmpRotGorila = glm::mat4(1.0f);
glm::mat4 tmpCaderaGorila = glm::mat4(1.0f);
glm::mat4 tmpCuerpoGorila = glm::mat4(1.0f);
glm::mat4 tmpAnteBrazoLGorila = glm::mat4(1.0f);
glm::mat4 tmpAnteBrazoRGorila = glm::mat4(1.0f);
glm::mat4 tmpMusloLGorila = glm::mat4(1.0f);
glm::mat4 tmpMusloRGorila = glm::mat4(1.0f);
glm::mat4 tmpPiernaLGorila = glm::mat4(1.0f);
glm::mat4 tmpPiernaRGorila = glm::mat4(1.0f);

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

float	Gorila_MovX = 0.0f,
		Gorila_MovY = 0.0f,
		Gorila_MovZ = 0.0f,
		Gorila_MovCadera = 0.0f,
		Gorila_MovCuerpo = 0.0f,
		Gorila_MovBrazo = 0.0f,
		Gorila_MovManoL = 0.0f,
		Gorila_MovManoR = 0.0f;
/*
float posXavion = 2.0, posYavion = 5.0, posZavion = -3.0;
float	movAvion_x = 0.0f, movAvion_y = 0.0f;
float giroAvion = 0;
*/

#define MAX_FRAMES 100
int i_max_steps = 240;
int i_curr_steps = 3;
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
	//Variables para GUARDAR Key Frames
	/*
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
	float giroAvion;
	float giroAvionInc;
	*/
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 3;			//introducir datos

bool start = false, play = false;
int playIndex = 0;

void saveFrame(void) //tecla L
{

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].Gorila_MovX = Gorila_MovX;
	KeyFrame[FrameIndex].Gorila_MovY = Gorila_MovY;
	KeyFrame[FrameIndex].Gorila_MovZ = Gorila_MovZ;
	KeyFrame[FrameIndex].Gorila_MovCuerpo = Gorila_MovCuerpo;
	KeyFrame[FrameIndex].Gorila_MovBrazo = Gorila_MovBrazo;
	KeyFrame[FrameIndex].Gorila_MovManoL = Gorila_MovManoL;
	KeyFrame[FrameIndex].Gorila_MovManoR = Gorila_MovManoR;
	/*
	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].giroAvion = giroAvion;//completar
	//no volatil, agregar una forma de escribir a un archivo para guardar los frames

	*/
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
	/*
	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	giroAvion = KeyFrame[0].giroAvion;
	*/
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
	/*
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;
	*/

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
			printf("playindex : %d\n", playIndex);
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

			i_curr_steps++;
			/*
			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			i_curr_steps++;
			*/
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
	MariposaTexture = Texture("Texture/1.jpg");
	MariposaTexture.LoadTexture();

	Domo_Exterior = Texture("Textures/Domo_exterior");
	Domo_Interior = Texture("Textures/Domo_interior");

	Tierra_Texture = Texture("Textures/Earth.jpg");

	//Texturas Gorila
	//GorilaTexture = Texture("Textures/Body-TM_u0_v0.png");
	//GorilaTexture.LoadTextureA();

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
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.6f, 0.6f,
		0.0f, -1.0f, 0.0f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	unsigned int spotLightCount = 0;
	/*
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 0.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(0.0f, 0.0f, 1.0f,
		0.0f, 0.0f,
		6.0f, 4.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

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

	KeyFrame[1].Gorila_MovX = 13.8000006;
	KeyFrame[1].Gorila_MovY = 1.5;
	KeyFrame[1].Gorila_MovZ = 10.800004;
	KeyFrame[1].Gorila_MovCuerpo = 6.600;
	KeyFrame[1].Gorila_MovBrazo = -61.499893;
	KeyFrame[1].Gorila_MovManoL = -61.499897;
	KeyFrame[1].Gorila_MovManoR = 122.400482;

	KeyFrame[2].Gorila_MovX = 13.8000006;
	KeyFrame[2].Gorila_MovY = 1.5;
	KeyFrame[2].Gorila_MovZ = 10.800004;
	KeyFrame[2].Gorila_MovCuerpo = 24.299986;
	KeyFrame[2].Gorila_MovBrazo = -61.499893;
	KeyFrame[2].Gorila_MovManoL = -130.800568;
	KeyFrame[2].Gorila_MovManoR = 52.499916;

	/*
	//---------PARA TENER KEYFRAMES GUARDADOS NO VOLATILES QUE SIEMPRE SE UTILIZARAN SE DECLARAN AQUÍ

	KeyFrame[0].movAvion_x = 0.0f;
	KeyFrame[0].movAvion_y = 0.0f;
	KeyFrame[0].giroAvion = 0;


	KeyFrame[1].movAvion_x = -1.0f;
	KeyFrame[1].movAvion_y = 2.0f;
	KeyFrame[1].giroAvion = 0;


	KeyFrame[2].movAvion_x = -2.0f;
	KeyFrame[2].movAvion_y = 0.0f;
	KeyFrame[2].giroAvion = 0;


	KeyFrame[3].movAvion_x = -3.0f;
	KeyFrame[3].movAvion_y = -2.0f;
	KeyFrame[3].giroAvion = 0;


	KeyFrame[4].movAvion_x = -3.0f;
	KeyFrame[4].movAvion_y = -2.0f;
	KeyFrame[4].giroAvion = 180.0f;

	KeyFrame[5].movAvion_x = 0.0f;
	KeyFrame[5].movAvion_y = 0.0f;
	KeyFrame[5].giroAvion = 180.0;

	KeyFrame[6].movAvion_x = 0.0f;
	KeyFrame[6].movAvion_y = 0.0f;
	KeyFrame[6].giroAvion = 0.0f;

	*/

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
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

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

		//Instancia Mariposas
		//mariposa centro
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 50.5f, 5.0f));
		model = glm::scale(model, glm::vec3(0.50f, 0.50f, 0.50f));		
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mariposa.RenderModel();
		//mariposa atras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, 40.5f, 5.0f));
		model = glm::scale(model, glm::vec3(0.40f, 0.40f, 0.40f)); 
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 45* toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		
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
		model = glm::translate(model, glm::vec3(20.0f, 7.5f, 35.0f));
		model = glm::scale(model, glm::vec3(0.10f, 0.10f, 0.10f));
		model = glm::rotate(model,225 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.0f, 7.5f, 35.0f));
		model = glm::scale(model, glm::vec3(0.10f, 0.10f, 0.10f));
		model = glm::rotate(model, 115 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.0f, 7.5f, -35.0f));
		model = glm::scale(model, glm::vec3(0.10f, 0.10f, 0.10f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Lampara.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 7.5f, -35.0f));
		model = glm::scale(model, glm::vec3(0.10f, 0.10f, 0.10f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();
		//sala2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, 7.5f, 35.0f));
		model = glm::scale(model, glm::vec3(0.10f, 0.10f, 0.10f));
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
		//glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0));
		//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CaderaGorila.RenderModel();

		model = glm::translate(tmpCaderaGorila, glm::vec3(0.0f,-1.9f,0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 2.10f, 0.0f));
		tmpCuerpoGorila = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, -2.10f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuerpoGorila.RenderModel();

		model = glm::translate(tmpCuerpoGorila, glm::vec3(0.0f, -2.10f, 0.0f));
		model = glm::translate(model, glm::vec3(0.7f, 3.899f, -0.3f));
		tmpAnteBrazoLGorila = model = glm::rotate(model, glm::radians(Gorila_MovBrazo), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.7f,-3.899f, 0.3f));
		//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AnteBrazoLGorila.RenderModel();

		model = glm::translate(tmpCuerpoGorila, glm::vec3(0.0f, -2.10f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.7f, 3.899f, -0.3f));
		tmpAnteBrazoRGorila = model = glm::rotate(model, glm::radians(Gorila_MovBrazo), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.7f, -3.899f, 0.3f));
		//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AnteBrazoRGorila.RenderModel();

		model = glm::translate(tmpRotGorila, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpMusloLGorila = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MusloLGorila.RenderModel();

		model = glm::translate(tmpRotGorila, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpMusloRGorila = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MusloRGorila.RenderModel();

		model = glm::translate(tmpCuerpoGorila, glm::vec3(0.0f, -2.10, 0.0f));
		//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CabezaGorila.RenderModel();

		model = glm::translate(tmpAnteBrazoLGorila, glm::vec3(-0.7f, -3.899f, 0.3f));
		model = glm::translate(model, glm::vec3(1.7f, 3.199f, -0.50f));
		model = glm::rotate(model, glm::radians(Gorila_MovManoL), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-1.7f, -3.199f, 0.50f));
		//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BrazoLGorila.RenderModel();

		model = glm::translate(tmpAnteBrazoRGorila, glm::vec3(0.7f, -3.899f, 0.3f));
		model = glm::translate(model, glm::vec3(-1.7f, 3.199f, -0.50f));
		model = glm::rotate(model, glm::radians(Gorila_MovManoR), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(1.7f, -3.199f, 0.50f));
		//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BrazoRGorila.RenderModel();

		model = glm::translate(tmpMusloLGorila, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpPiernaLGorila = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PiernaLGorila.RenderModel();

		model = glm::translate(tmpMusloRGorila, glm::vec3(0.0f, 0.0f, 0.0f));
		tmpPiernaRGorila = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PiernaRGorila.RenderModel();

		model = glm::translate(tmpPiernaLGorila, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PieLGorila.RenderModel();

		model = glm::translate(tmpPiernaRGorila, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PieRGorila.RenderModel();

		model = glm::translate(tmpRotGorila, glm::vec3(Gorila_MovX, Gorila_MovY, Gorila_MovZ));
		//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Pivote.RenderModel();

		/*
		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 0.5f, -3.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();
	

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero? // DESAPAECE EL COCHE Y EL HELICOPTERO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);
		*/

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_J])
		Gorila_MovX = (mainWindow.getGorila_MovX());
	if (keys[GLFW_KEY_L])
		Gorila_MovX = (mainWindow.getGorila_MovX());
	if (keys[GLFW_KEY_I])
		Gorila_MovY = (mainWindow.getGorila_MovY());
	if (keys[GLFW_KEY_K])
		Gorila_MovY = (mainWindow.getGorila_MovY());
	if (keys[GLFW_KEY_Y])
		Gorila_MovZ = (mainWindow.getGorila_MovZ());
	if (keys[GLFW_KEY_H])
		Gorila_MovZ = (mainWindow.getGorila_MovZ());
	if (keys[GLFW_KEY_1])
		Gorila_MovCuerpo = (mainWindow.getGorila_MovCuerpo());
	if (keys[GLFW_KEY_2])
		Gorila_MovCuerpo = (mainWindow.getGorila_MovCuerpo());
	if (keys[GLFW_KEY_3])
		Gorila_MovBrazo = (mainWindow.getGorila_MovBrazo());
	if (keys[GLFW_KEY_4])
		Gorila_MovBrazo = (mainWindow.getGorila_MovBrazo());
	if (keys[GLFW_KEY_5])
		Gorila_MovManoL = (mainWindow.getGorila_MovManoL());
	if (keys[GLFW_KEY_6])
		Gorila_MovManoL = (mainWindow.getGorila_MovManoL());
	if (keys[GLFW_KEY_7])
		Gorila_MovManoR = (mainWindow.getGorila_MovManoR());
	if (keys[GLFW_KEY_8])
		Gorila_MovManoR = (mainWindow.getGorila_MovManoR());

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
			printf("Gorila_MovX es: %f\n", Gorila_MovX);
			printf("Gorila_MovY es: %f\n", Gorila_MovY);
			printf("Gorila_MovZ es: %f\n", Gorila_MovZ);
			printf("Gorila_MovCuerpo es: %f\n", Gorila_MovCuerpo);
			printf("Gorila_MovBrazo es: %f\n", Gorila_MovBrazo);
			printf("Gorila_MovManoL es: %f\n", Gorila_MovManoL);
			printf("Gorila_MovManoR es: %f\n", Gorila_MovManoR);
			//printf("movAvion_y es: %f\n", movAvion_y);
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