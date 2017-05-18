#include"Game.h"
#include "Geometry.h"
#include "mainCore.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp> 
#include <gtc/constants.hpp>
#include <gtx/transform.hpp>
#include <iostream>
#include "methods.h"

using namespace std;
using namespace glm;


GLFWwindow* wind;

const float kRotateSpeed = 0.5f;
const float kMoveSpeed = 0.5f;

float fRotation = 0.0f;
glm::vec3 vMovement = glm::vec3(0,0,0);
glm::vec3 vDirection;	
float	kAvatarMovement = 0.01f;
float	kAvatarRotation = 0.01f;

//init 
Game::Game()
{
	auto windowpos = mainCore::get()->getWindowSize();

	wind = mainCore::get()->getwindow();

	basicShader = std::make_shared<Shader>("Model.vertexshader", "Model.fragmentshader");


	auto _skyboxShader = std::make_shared<Shader>("Skybox.vertexshader", "Skybox.fragmentshader");

	sky.setShader(_skyboxShader);


	Camera = std::unique_ptr<ThirdCamera>(new ThirdCamera());

	auto mesh = methods::loadMesh("res/plane/SpaceShip.obj");
	mesh->setShader(basicShader);
	plan = std::make_shared<GameObject>(mesh); 
	plan->position = vec3(0.5,3,-1);
	plan->rotation = glm::vec3(0,0,1);
	plan->scale = glm::vec3(0.3,0.3,0.3);
	vDirection = plan->rotation;

	Camera->UpdatePosition(plan->position, plan->rotation);


	_ground = new Ground(5,30);

	model_animated_light.intensity = 1;
	model_animated_light.color = vec3(1,0,0);
	model_animated_light.position = vec3(0,3,0);

	playerLight.position = plan->position;
	playerLight.intensity = 1;
	playerLight.color = vec3(1,1,1);

	model_animated_Position = vec3(2.25,1.6,15.4);
	model_animated_Rotate = vec3(-90,0,180);
	model_animated = glm::translate(model_animated_Position) * glm::rotate(-90.0f,vec3(1.0f,0.0f,0.0f)) *glm::rotate(180.0f,vec3(0.0f,0.0f,1.0f)) * glm::scale(vec3(0.025f,0.025f,0.025f));

	//load model file..
	mymodel.LoadModel("res/drfreak/drfreak.md2");

	//set curent animation to play..
	model_anim_state = mymodel.StartAnimation(animType_t::CROUCH_WALK);

	//load animation shader.
	animatedModelShader.LoadProgram();

}

Game::~Game()
{
	delete _ground;
}


void Game::setupShaderUniforms(ShaderPtr shdr)
{
	shdr->setUniform("TxSampler",0);
	shdr->setUniform("NrmSampler",1);

	shdr->setUniform("ViewMatrix",Camera->View);
	//do the lights here
	shdr->setUniform("eyePos",Camera->mvPosition);

	shdr->setUniform("playerLightPosition",playerLight.position);
	shdr->setUniform("playerLightColor",playerLight.color);
	shdr->setUniform("playerLightIntensity",playerLight.intensity);

	shdr->setUniform("monsterLightPosition",model_animated_light.position);
	shdr->setUniform("monsterLightColor",model_animated_light.color);
	shdr->setUniform("monsterLightIntensity",model_animated_light.intensity);

}
void Game::update(double delta)
{
	//moving the light with the camera.
	model_animated_light.position = model_animated_Position;

	playerLight.position = plan->position;


	mymodel.UpdateAnimation(&model_anim_state,delta / 1000);

	vec3 bpos;



	if( glfwGetKey(wind, GLFW_KEY_W))
	{
		vDirection = plan->rotation;
		vMovement += vDirection * (kAvatarMovement);
	}
	if( glfwGetKey(wind, GLFW_KEY_S))
	{
		vDirection = plan->rotation;
		vMovement -= vDirection * (kAvatarMovement);
	}

	if( glfwGetKey(wind, GLFW_KEY_A))
	{		
		fRotation += kAvatarRotation;


		// Rotate the Orientation Vector of the avatar to face the correct direction.
		Camera->RotateVectorArbitrary(plan->rotation , glm::vec3(0,0,0), fRotation, glm::vec3(0,1,0));
		plan->rotation = glm::normalize(plan->rotation);

		plan->setorient(plan->rotation,glm::vec3(0,0,1));
		Camera->UpdatePosition(plan->position, plan->rotation);
	}

	if( glfwGetKey(wind, GLFW_KEY_D))
	{
		fRotation -= kAvatarRotation;


		Camera->RotateVectorArbitrary(plan->rotation , glm::vec3(0,0,0), fRotation, glm::vec3(0,1,0));
		plan->rotation = glm::normalize(plan->rotation);

		plan->setorient(plan->rotation,glm::vec3(0,0,1));
		Camera->UpdatePosition(plan->position, plan->rotation);
	}

	if( glfwGetKey(wind, GLFW_KEY_UP))
	{
		plan->position.y += 0.01;
		Camera->UpdatePosition(plan->position, plan->rotation);
	}

	if( glfwGetKey(wind, GLFW_KEY_DOWN) && plan->position.y >= 3)
	{
		plan->position.y -= 0.01;
		Camera->UpdatePosition(plan->position, plan->rotation);
	}


	if( glfwGetKey(wind, GLFW_KEY_ESCAPE))
	{
		mainCore::get()->shutdown();
	}


	if (vMovement != glm::vec3(0,0,0))
	{
		plan->position += vMovement;
		Camera->UpdatePosition(plan->position, plan->rotation);	
	}


	Camera->Look();

	vMovement = vec3(0,0,0);
	fRotation = 0;
}

void Game::render()
{
	glm::mat4 VP =  Camera->Proj* Camera->View;

	sky.draw(VP);

	_ground->draw(VP);

	plan->draw(VP);

	animatedModelShader.UseProgram();
	animatedModelShader.BindVPMatrix(&VP[0][0]); 

	animatedModelShader.BindModelMatrix(&model_animated[0][0]);

	mymodel.RenderModel(&model_anim_state,&animatedModelShader);
}