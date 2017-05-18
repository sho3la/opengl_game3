#pragma once
#pragma once
#include "Cube.h"
#include "TextureCube.h"
#include <memory>
#include <glm.hpp>
using namespace std;
using namespace glm;


class Skybox
{

public:
	Cube _cube;
	TextureCube* _textureCube;
	int size;
	int offset;

	Skybox()
	{
		size = 100;     //20
		offset = 2.5;
		_cube._cube->scale = vec3(size+offset,size+offset,size+offset);
		vector<string> files;
		files.push_back("res\\ely_cloudtop\\cloudtop_rt.jpg");
		files.push_back("res\\ely_cloudtop\\cloudtop_lf.jpg");
		files.push_back("res\\ely_cloudtop\\cloudtop_up.jpg");
		files.push_back("res\\ely_cloudtop\\cloudtop_dn.jpg");
		files.push_back("res\\ely_cloudtop\\cloudtop_bk.jpg");
		files.push_back("res\\ely_cloudtop\\cloudtop_ft.jpg");
		_textureCube = new TextureCube(files,0); 
	}
	~Skybox()
	{
		delete _textureCube;
	}

	void setShader(ShaderPtr val)
	{
		_cube._cube->mesh->setShader(val);
	}

	void draw(mat4 vp)
	{
		//hat el material w est5dm el shader program ya fraan 
		_cube._cube->mesh->getMaterial()->shader->bind();

		//bind el swar 
		_textureCube->bind();


		_cube._cube->draw(vp);

		glBindTexture(GL_TEXTURE_CUBE_MAP,0);
	}
};