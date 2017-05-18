#pragma once
#include <gl/glew.h>
#include <memory>
#include "Geometry.h"
#include "Material.h"
#include "Shader.h"
#include <vector>

class GameObject;
class Model
{
	friend class GameObject;
public:
	Model(GeometryPtr g,MaterialPtr m);
	Model();
	~Model();

	GeometryPtr getGeometry();
	void setGeometry(GeometryPtr val);

	MaterialPtr getMaterial(){
		return material;
	}
	void setMaterial(MaterialPtr m){
		material = m;
	}

	void draw(int is_instancing = -1);

	void addSubModel(std::shared_ptr<Model> val);
	void setShader(ShaderPtr val);

private:
	std::vector<std::shared_ptr<Model> > _subModeles;
	GeometryPtr geometry;
	MaterialPtr material;
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;

};
typedef std::shared_ptr<Model> ModelPtr;