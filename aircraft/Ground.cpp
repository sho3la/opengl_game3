#include "Ground.h"
#include "methods.h"
#include "mainCore.h"
#include <fstream>
#include <iostream>

using namespace std;

Ground::Ground(int _x, int _y):GameObject(NULL)
{
	x = _x;
	y = _y;
	_ground = NULL;
	auto meshTest = methods::loadMesh("res/Ground/cube.obj");
	auto _genericShader = std::make_shared<Shader>("Model.vertexshader", "Model.fragmentshader");
	meshTest->setShader(_genericShader);
	_cube = std::make_shared<GameObject>(meshTest);

	for(int i=0;i<x;i++)
	{
		for(int j=0;j<y;j++)
		{
			_offsets.push_back(glm::vec3(i,0,j));
		}
	}
}

Ground::~Ground()
{ 
	if(_ground != NULL)
	{
		for(int i=0;i<y;i++)
		{
			delete[] _ground[i];
		}
	}
	delete[] _ground;
}

void Ground::draw(glm::mat4 vp,int is_Instancing)
{
		auto shdr = _cube->mesh->getMaterial()->shader;
		//useprogram
		shdr->bind();
		mainCore::getGame()->setupShaderUniforms(shdr);

		shdr->setUniform("isInstancing",1);

		for(int i=0;i<_offsets.size();i++)
		{
			shdr->setUniform("offsets["+to_string(i)+"]",_offsets[i]);
		}

		_cube->draw(vp,x*y);
}
