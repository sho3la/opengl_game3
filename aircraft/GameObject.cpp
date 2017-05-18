#include "GameObject.h"
#include "Material.h"
#include "Shader.h"




void GameObject::draw(glm::mat4 vp,int is_Instancing)
{
	mesh->getMaterial()->shader->setUniform("MVP",vp* getModel());

	mesh->getMaterial()->shader->setUniform("ModelMatrix",getModel());
	mesh->draw(is_Instancing);
}



glm::mat4 GameObject::getModel()
{
	glm::mat4 tm = glm::translate(glm::mat4(1),position);

	glm::mat4 rotx = glm::rotate(glm::mat4(1),rotation.x,glm::vec3(1,0,0));
	glm::mat4 roty = glm::rotate(glm::mat4(1),rotation.y,glm::vec3(0,1,0));
	glm::mat4 rotz = glm::rotate(glm::mat4(1),rotation.z,glm::vec3(0,0,1));
	glm::mat4 grot = rotx*roty*rotz;
	grot *= orientmat;

	glm::mat4 sclMat = glm::scale(glm::mat4(1),scale);

	return tm * grot * sclMat;
}

void GameObject::setorient(glm::vec3 orient , glm::vec3 rotAxis)
{
	orientmat = glm::orientation(orient,rotAxis);
}
