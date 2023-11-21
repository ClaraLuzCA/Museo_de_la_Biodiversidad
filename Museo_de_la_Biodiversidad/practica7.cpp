/*
Semestre 2024-1
Pr�ctica 7: Iluminaci�n 1 
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

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;
Model Dado_M;
Model Domos;
Model Tierra;
Model Mariposa;
Model Lampara;
Model Helecho;
Model Monstera;

//Tigre
Model Tigre;
//Vegetación Sala 2
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


//funci�n de calculo de normales por promedio de v�rtices 
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
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	MariposaTexture = Texture("Textures/1.jpg");
	MariposaTexture.LoadTexture();
	//Cargamos textura del tigre
	TigreTexture = Texture("Textures/Tigre.jpeg");
	TigreTexture.LoadTextureA();

	//Cargamos textura del vegetación
	PlantaTaro = Texture("Textures/Taro.png");
	PlantaTaro.LoadTextureA();
	JungleTexture = Texture("Textures/ExtraPlant.png");
	JungleTexture.LoadTextureA();

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

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);//CEF BRILLO
	Material_opaco = Material(0.3f, 4);


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaraci�n de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(0.0f, 0.0f, 1.0f,
		0.0f, 1.0f,
		6.0f, 4.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(0.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		-15.0f, 10.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	
	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
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
		
		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	
		//sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz
			glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//informaci�n al shader de fuentes de iluminaci�n
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
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
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
		Monstera.RenderModel();

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
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JungleTexture.UseTexture();
		JunglePlant.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-56.0f, 8.0f, 50.0f));
		model = glm::rotate(model, anglePlantaR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
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
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JungleTexture.UseTexture();
		JunglePlant.RenderModel();

		//----------
		//Vegetación Sala 2 Parte 2
		//----------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, 8.0f, -35.0f));
		model = glm::rotate(model, anglePlantaR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 8.0f, -42.0f));
		model = glm::rotate(model, anglePlantaR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-75.0f, 8.0f, -35.0f));
		model = glm::rotate(model, anglePlantaR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-95.0f, 8.0f, -54.0f));
		model = glm::rotate(model, anglePlantaR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-59.0f, 8.0f, -65.0f));
		model = glm::rotate(model, anglePlantaR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 8.0f, -55.0f));
		model = glm::rotate(model, anglePlantaR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-105.0f, 8.0f, -45.0f));
		model = glm::rotate(model, anglePlantaR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PlantaTaro.UseTexture();
		Taro.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, 8.0f, -65.0f));
		model = glm::rotate(model, anglePlantaR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
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
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JungleTexture.UseTexture();
		JunglePlant.RenderModel();
		//Vegetación Sala 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-56.0f, 8.0f, -50.0f));
		model = glm::rotate(model, anglePlantaR, glm::vec3(0.0f, 1.0f, 1.0f)); // Rotar 
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
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
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JungleTexture.UseTexture();
		JunglePlant.RenderModel();

		//Tigre
		float angleInDegrees = 180.0f; // Ángulo de rotación en grados
		float angleInRadians = glm::radians(angleInDegrees); // Convertir a radianes
		// Crear la matriz de transformación
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-65.0f, 7.5f, 40.0f));
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

		model = glm::translate(tmpRodillaTCamello, glm::vec3(Camello_MovX + 0.05f, Camello_MovY - 0.95f, Camello_MovZ + 0.55f));
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

		//Agave �qu� sucede si lo renderizan antes del coche y el helic�ptero? // DESAPAECE EL COCHE Y EL HELICOPTERO
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

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
