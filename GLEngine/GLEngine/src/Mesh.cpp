#include "Mesh.h"

Mesh::Mesh()
{
	transform = glm::mat4();
}


void Mesh::LoadMesh(const char* filePath)
{
	meshData = new FBXFile();
	meshData->load(filePath);
	boundingSpheres = new BoundingSphere[meshData->getMeshCount()];
}

void Mesh::CreateBuffers()
{
	for (GLuint i = 0; i < meshData->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = meshData->getMeshByIndex(i);

		boundingSpheres[i].calculateSphere(mesh);

		GLuint* bufferData = new GLuint[3];

		std::cout << meshData->getTextureCount();

		glGenVertexArrays(1, &bufferData[0]);
		glBindVertexArray(bufferData[0]);

		glGenBuffers(1, &bufferData[1]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferData[1]);
		glBufferData(GL_ARRAY_BUFFER, mesh->m_vertices.size() * sizeof(FBXVertex), mesh->m_vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &bufferData[2]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferData[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->m_indices.size() * sizeof(GLuint), mesh->m_indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), ((char*)0) + FBXVertex::NormalOffset);
		
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), ((char*)0) + FBXVertex::TangentOffset);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), ((char*)0) + FBXVertex::BiNormalOffset);
		
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), ((char*)0) + FBXVertex::TexCoord1Offset);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mesh->m_userData = bufferData;
	}
}
void Mesh::RemoveBuffers()
{
	for (GLuint i = 0; i < meshData->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = meshData->getMeshByIndex(i);
		GLuint* bufferData = (GLuint*) mesh->m_userData;

		glDeleteVertexArrays(1, &bufferData[0]);
		glDeleteBuffers(1, &bufferData[1]);
		glDeleteBuffers(1, &bufferData[2]);

		delete[] bufferData;
	}
}


void Mesh::translate(vec3 position)
{
	transform = glm::translate(transform, position);
	for (GLuint i = 0; i < meshData->getMeshCount(); ++i)
	{
		boundingSpheres[i].reCalcutalteSphere(transform);
	}
}
void Mesh::scale(float scalar)
{
	transform = glm::scale(transform, vec3(scalar));
	for (GLuint i = 0; i < meshData->getMeshCount(); ++i)
	{
		boundingSpheres[i].reCalcutalteSphere(transform);
	}
}


void Mesh::Draw(Camera& camera, ShaderProgram& shader)
{
	shader.useProgram();

	for (GLuint i = 0; i < meshData->getMeshCount(); ++i)
	{
		if (boundingSpheres[i].isColliding(camera.getFrustrum()))
		{
			FBXMeshNode* mesh = meshData->getMeshByIndex(i);
			GLuint* bufferData = (GLuint*)mesh->m_userData;

			shader.setMat4("transform", transform);

			glBindVertexArray(bufferData[0]);
			glDrawElements(GL_TRIANGLES, mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
		}	
	}

	
}


