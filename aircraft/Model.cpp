#include "Model.h"



Model::Model(GeometryPtr g, MaterialPtr m)
{
	material = m;
	VAO = -1;
	VBO = -1;
	IBO = -1;
	setGeometry(g);
}
Model::Model(){
	geometry = nullptr;
	material = nullptr;
	VAO = -1;
	VBO = -1;
	IBO = -1;
}
Model::~Model()
{
	geometry = nullptr;
	material = nullptr;
	if(VAO != -1)
		glDeleteVertexArrays(1,&VAO);
	if(VBO != -1)
		glDeleteBuffers(1,&VBO);
	if(IBO != -1)
		glDeleteBuffers(1,&IBO);

}

GeometryPtr Model::getGeometry()
{
	return geometry;
}

void Model::setGeometry(GeometryPtr val)
{
	if(geometry != val){
		geometry = val;
		if(VAO != -1){
			glDeleteVertexArrays(1,&VAO);
		}
		if(VBO != -1){
			glDeleteBuffers(1,&VBO);
		}
		if(IBO != -1){
			glDeleteBuffers(1,&IBO);
		}
		glGenVertexArrays(1,&VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1,&VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, geometry->getVerticesCount() * sizeof(Vertex), &geometry->getVertices()[0], GL_STATIC_DRAW);

		glGenBuffers(1,&IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,geometry->getIndicesCount()*sizeof(unsigned short),&geometry->getIndices()[0],GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)12);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)24);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)36);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)44);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	}
}

void Model::draw(int is_instancing){
	if(material){
		if(material->diffuse)
			material->diffuse->bind(0);
		if(material->normal){
			material->normal->bind(1);
			material->shader->setUniform("hasNormals",1);
		}
		if(material->mode!=NONE)
			material->shader->setUniform("MODE",material->mode);
	}
	glBindVertexArray(VAO);
	if(is_instancing==-1)
		glDrawElements(GL_TRIANGLES,geometry->getIndicesCount(),GL_UNSIGNED_SHORT,0);
	else
		glDrawElementsInstanced(GL_TRIANGLES,geometry->getIndicesCount(),GL_UNSIGNED_SHORT,0,is_instancing);
	//glDrawArrays(GL_TRIANGLES,0,geometry->getVerticesCount());
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D,0);
	
	for(int i=0;i<_subModeles.size();i++)
		_subModeles[i]->draw();
}

void Model::addSubModel(ModelPtr val){
	_subModeles.push_back(val);
}

void Model::setShader(ShaderPtr val){
	material->shader = val;
	for(int i=0;i<_subModeles.size();i++)
		_subModeles[i]->material->shader = val;
}