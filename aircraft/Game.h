#pragma once
#include<memory>
#include "GameObject.h"
#include "Shader.h"
#include<GLFW/glfw3.h>
#include <glm.hpp>
#include <map>
#include <string>
#include "Skybox.h"
#include "PointLight.h"
#include "Ground.h"
#include "md2model.h"

#include "ThirdCamera.h"


class Game
{
private:

	ShaderPtr basicShader;
	Skybox sky;
	Ground* _ground;


	CMD2Model mymodel;
	animState_t model_anim_state;
	KeyFrameAnimationShader animatedModelShader;

	glm::mat4 model_animated;
	glm::vec3 model_animated_Position;
	glm::vec3 model_animated_Rotate;


	PointLight playerLight, model_animated_light;

public:
	std::unique_ptr<ThirdCamera> Camera;
	GameObjectPtr plan;


	Game();
	~Game();

	void setupShaderUniforms(ShaderPtr shdr);
	void update(double delta);
	void render();
};
typedef std::shared_ptr<Game> GamePtr;
