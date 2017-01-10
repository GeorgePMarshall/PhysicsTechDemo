#include "Application.h"

//calls load function in new thread and play loading icon
void Application::RunLoad()
{
	GLuint VAO, VBO, EBO;


	ShaderProgram shader;
	shader.CreateShaderProgram("Loading.vert", "Loading.frag");
	
	Texture background;
	Texture spinner;

	background.LoadTexture("data/Loading/loadingScreen.png");
	spinner.LoadTexture("data/Loading/Spinner.png");

	GLfloat vertexData[] =
	{
		0, 1, 0, 1,		0, 1,
		1, 1, 0, 1,		1, 1,
		1, 0, 0, 1,		1, 0,
		0, 0, 0, 1,		0, 0
	};

	GLuint indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //position
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);

	glEnableVertexAttribArray(1); //texCoords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)(sizeof(GLfloat) * 4));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::future<void> future = std::async(std::launch::async, [this] { this->ThreadedLoad(); });

	std::future_status status = future.wait_for(std::chrono::milliseconds(0));

	glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f);
	shader.setMat4("projection", projection);
	glm::mat4 backgroundTransform, spinnerTransform;
	
	backgroundTransform = glm::translate(backgroundTransform, vec3(0, 0, 0));
	backgroundTransform = glm::scale(backgroundTransform, vec3(1280, 720, 1));
	spinnerTransform = glm::translate(spinnerTransform, vec3(590, 400, 1));
	spinnerTransform = glm::scale(spinnerTransform, vec3(100, 100, 1));

	while (status != std::future_status::ready)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		status = future.wait_for(std::chrono::milliseconds(0));

		shader.useProgram();
		
		shader.setInt("diffuse", 0);
		glActiveTexture(GL_TEXTURE0);
		
		
		
		shader.setMat4("transform", backgroundTransform);
		background.Bind();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		spinnerTransform = glm::translate(spinnerTransform,vec3(0.5f, 0.5f , 0.0f));
		spinnerTransform = glm::rotate(spinnerTransform, glm::radians(2.0f), vec3(0, 0, 1));
		spinnerTransform = glm::translate(spinnerTransform, vec3(-0.5f , -0.5f , 0.0f));
		
		shader.setMat4("transform", spinnerTransform);
		spinner.Bind();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Load();
}

//creates window and main loop
int Application::Run()
{
	if (glfwInit() == false)
		return -1;

	window = glfwCreateWindow(1920, 1080, "Not A Wondow", nullptr , nullptr);

	if (!window)
	{
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1.f, 1.f, 1.f, 1.f);

	Input::Initialize(window);
	gTime::Initialize();
	Network::Initialize()->InitializeRaknet();

	Initialize();

	RunLoad();
	
	glClearColor(0.f, 0.1f, 0.2f, 1.f);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		gTime::getInstance()->Update();

		Update();
		Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

GLFWwindow* Application::GetWindow()
{
	return window;
}
