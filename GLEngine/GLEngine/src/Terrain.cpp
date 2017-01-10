#include "Terrain.h"
#include "gTime.h"

void Terrain::Initialize(GLuint rows, GLuint cols)
{
	this->rows = rows;
	this->cols = cols;
	
	shader.CreateShaderProgram("Terrain.vert", "Terrain.frag");
	shader.setVec3("light.direction", vec3(0.2, 0.2, 0.2));
	shader.setVec3("light.diffuse", vec3(1));
	shader.setVec3("light.specular", vec3(1));

	baseTexture.SetDiffuse("data/TerrainTextures/lava_d.jpg");
	baseTexture.SetNormal("data/TerrainTextures/lava_n.jpg");
	baseTexture.SetSpecular("data/TerrainTextures/lava_s.jpg");

	heightMap = new Texture;
}


void Terrain::GenerateGrid()
{

	vertices = new Vertex[rows * cols];
	GLfloat* height = GenerateHeightMap();

	//Gen Verticies
	for (GLuint i = 0; i < rows; ++i)
	{
		for (GLuint j = 0; j < cols; ++j)
		{
			vertices[i * cols + j].position = vec3((float)j, height[i * cols + j], (float)i);
			vertices[i * cols + j].texCoord = vec2((float)j/(rows / 2), (float)i/(cols / 2));
		}
	}

	//Gen Normals
	for (GLuint i = 1; i < rows - 1; ++i)
	{
		for (GLuint j = 1; j < cols - 1; ++j)
		{
			vec3 up = vertices[(i - 1) * cols + j].position; //up
			vec3 upRight = vertices[(i - 1) * cols + (j + 1)].position; //upRight
			vec3 down = vertices[(i + 1) * cols + j].position; //down
			vec3 downLeft = vertices[(i + 1) * cols + (j - 1)].position; //downLeft
			vec3 left = vertices[i * cols + (j - 1)].position; //left
			vec3 right = vertices[i * cols + (j + 1)].position; //right

			vec3 normal1 = glm::cross(up, left);
			vec3 normal2 = glm::cross(upRight, up);
			vec3 normal3 = glm::cross(right, upRight);
			vec3 normal4 = glm::cross(down, right);
			vec3 normal5 = glm::cross(downLeft, down);
			vec3 normal6 = glm::cross(left, downLeft);

			vec3 sum = normal1 + normal2 + normal3 + normal4 + normal5 + normal6;

			vertices[i * cols + j].normal = (glm::length2(sum) == 0) ? sum : glm::normalize(sum);
		}
	}

	//GEN Indicies
	indices = new GLuint[(rows - 1) * (cols - 1) * 6];
	GLuint curIndex = 0;
	for (GLuint i = 0; i < (rows - 1); ++i)
	{
		for (GLuint j = 0; j < (cols - 1); ++j)
		{
			indices[curIndex++] = i * cols + j;
			indices[curIndex++] = (i + 1) * cols + j;
			indices[curIndex++] = (i + 1) * cols + (j + 1);

			indices[curIndex++] = i * cols + j;
			indices[curIndex++] = (i + 1) * cols + (j + 1);
			indices[curIndex++] = i * cols + (j + 1);
		}
	}

	delete[] height;
}

void Terrain::GenerateBuffers()
{
	if (!vertices || !indices)
		return;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1); //normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(vec3));
	glEnableVertexAttribArray(2); //texCoord 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec3) * 2));


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (cols - 1) * 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] vertices;
	delete[] indices;
}

void Terrain::GenerateHeightMapTexture()
{
	GLfloat* perlinData = new GLfloat[rows * cols];
	GLfloat scale = (1.f / cols) * 3;
	//GLfloat scale = (i * 0.99853f - j * 0.05408f);
	GLuint sampleAmount = 6;
	GLfloat amplitudeStep = 0.3f;
	
	for (GLuint i = 0; i < rows; ++i)
	{
		for (GLuint j = 0; j < cols; ++j)
		{
			GLfloat amplitude = 5.f;
			perlinData[j * cols + i] = 0;

			for (GLuint k = 0; k < sampleAmount; ++k)
			{
				GLfloat frequency = powf(6, (float)k);
				GLfloat perlinSample = glm::perlin(vec2(i, j) * scale * frequency) * 0.5f + 0.5f;
				perlinData[j * cols + i] += perlinSample * amplitude;
				amplitude *= amplitudeStep;
			}
		}
	}

	heightMap->LoadTexture(perlinData, rows, cols, GL_R32F, GL_RED);
	delete[] perlinData;
}

GLfloat* Terrain::GenerateHeightMap()
{
	GLfloat* perlinData = new GLfloat[rows * cols];
	GLfloat scale = (1.f / cols) * 3;
	//GLfloat scale = (i * 0.99853f - j * 0.05408f);
	GLuint sampleAmount = 6;
	GLfloat amplitudeStep = 0.3f;

	for (GLuint i = 0; i < rows; ++i)
	{
		for (GLuint j = 0; j < cols; ++j)
		{
			GLfloat amplitude = 20.f;
			perlinData[j * cols + i] = 0;

			for (GLuint k = 0; k < sampleAmount; ++k)
			{
				GLfloat frequency = powf(4, (float)k);
				GLfloat perlinSample = glm::perlin(vec2(i, j) * scale * frequency) * 0.5f + 0.5f;
				perlinData[j * cols + i] += perlinSample * amplitude;
				amplitude *= amplitudeStep;
			}
		}
	}

	//heightMap->LoadTexture(perlinData, rows, cols, GL_R32F, GL_RED);
	return perlinData;
}

void Terrain::draw(Camera& camera, vec3& lightDir)
{
	shader.setMat4("projectionView", camera.getProjectionViewTransform());
	shader.setVec3("light.direction", lightDir);


	shader.setVec3("cameraPos", camera.getPosition());


	shader.useProgram();
	glBindVertexArray(VAO);

	baseTexture.Bind(&shader);
	
	glActiveTexture(GL_TEXTURE4);
	shader.setInt("heightMap", 4);
	heightMap->Bind();

	glDrawElements(GL_TRIANGLES, (rows - 1) * (cols - 1) * 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


}