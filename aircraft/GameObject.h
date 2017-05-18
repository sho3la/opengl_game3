#pragma once
#include "Model.h"
#include <gtx/transform.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx\rotate_vector.hpp>
#include <glm.hpp>
#include <memory>


class GameObject
{
public:
	ModelPtr mesh;
	glm::vec3 position,rotation,scale;
	glm::mat4  orientmat;
	GameObject(ModelPtr m,glm::vec3 pos=glm::vec3(0),glm::vec3 rot=glm::vec3(0),glm::vec3 scl=glm::vec3(1))
	{
		mesh = m;
		position = pos;
		rotation = rot;
		scale = scl;

		orientmat = glm::mat4(1.0f);
	}

	virtual ~GameObject()
	{
		mesh = nullptr;
	}

	void setorient(glm::vec3 orient , glm::vec3 rotAxis);

	glm::mat4 getModel();

	virtual void draw(glm::mat4 vp,int is_Instancing = -1);
};
typedef std::shared_ptr<GameObject> GameObjectPtr;