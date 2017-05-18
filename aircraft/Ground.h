#pragma once
#include "Model.h"
#include "GameObject.h"


class Ground : public GameObject
{
	GameObjectPtr _cube;
	std::vector<glm::vec3> _offsets;

	char **_ground;
	int x,y;
public:
	Ground(int _x, int _y);
	~Ground();

	void draw(glm::mat4 vp,int is_Instancing = -1) override;
};