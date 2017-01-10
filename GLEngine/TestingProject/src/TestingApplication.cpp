#include "TestingApplication.h"
#include "Gizmos.h"




void TestingApplication::Initialize()
{
	showUI = !showUI;
	camera.toggleMouseInput(GetWindow());

	Gizmos::create();

	ground.Initialize();
	ground.setKinematic(true);
	ground.AddPlaneCollider(vec3(0, 1, 0), 0);
	physicsManager.registerRigidbody(ground);
	
	


	player.Initialize();
	player.position = vec3(0, 5, 3);
	player.rigidbody.setKinematic(true);
	physicsManager.registerRigidbody(player.rigidbody);
	
	player2.Initialize();
	player2.position = vec3(0, 3, 3);
	physicsManager.registerRigidbody(player2.rigidbody);

	cube.Initialize();
	cube.position = vec3(4.2, 20, 4.2);
	physicsManager.registerRigidbody(cube.rigidbody);

	cube2.Initialize();
	cube2.position = vec3(5, 5, 5);
	cube2.rigidbody.setKinematic(true);
	physicsManager.registerRigidbody(cube2.rigidbody);
	
	spring = new Spring(&player.rigidbody, &player2.rigidbody, 10.f, 1.f);
	physicsManager.registerSpring(*spring);

	for (int i = 0; i < 5; i++)
	{
		Player* prevPlayer = nullptr;
		for (int j = 0; j < 2; j++)
		{
			Player* newPlayer = new Player;
			newPlayer->Initialize();
			newPlayer->rigidbody.mass = 1.0f;
			newPlayer->rigidbody.elasticity = 1.0f;
			newPlayer->position = vec3(2 * i, (3 * j) + 3, 0);
			if (i == 0 && j == 0)
				newPlayer->position = vec3(-3, 6, 0);
			
			
			
			if (j == 0)
			{
				prevPlayer = newPlayer;
			}
			else
			{
				newPlayer->rigidbody.setKinematic(true);
				Spring* newSpring = new Spring(&newPlayer->rigidbody, &prevPlayer->rigidbody, 200.f, 1.f);
				springs.push_back(newSpring);
				physicsManager.registerSpring(*newSpring);
			}
	
			physicsManager.registerRigidbody(newPlayer->rigidbody);
			players.push_back(newPlayer);
		}
	
	}


	physX.Initialize();

}

void TestingApplication::ThreadedLoad()
{

	physX.ThreadedLoad();
}

void TestingApplication::Load()
{
	lightDirection = vec3(0, 35, 0);

	ImGui_ImplGlfwGL3_Init(GetWindow(), false);
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = 1280;
	io.DisplaySize.x = 720;


	camera.LookAt(vec3(15), vec3(0), vec3(0, 1, 0));
	camera.setPerspective(glm::radians(80.0f), 16 / 9.f);
	camera.initialize();

	shader.CreateShaderProgram("Specular.vert", "Specular.frag");

	physX.Load();
}


void TestingApplication::Update()
{

	if (Input::getInput()->getKeyDown(GLFW_KEY_ESCAPE))
	{
		showUI = !showUI;
		camera.toggleMouseInput(GetWindow());
	}
	camera.update();

	if(!Input::getInput()->getKey(GLFW_KEY_F))
		physicsManager.Update();

	if (Input::getInput()->getKey(GLFW_KEY_E))
	{
		charge += 20;
	}

	if (Input::getInput()->getKeyUp(GLFW_KEY_E))
	{
		Player* newPlayer = new Player;
		newPlayer->Initialize();
		newPlayer->position = camera.getPosition();
		newPlayer->rigidbody.AddForce(camera.getWorldTransform()[2].xyz * -1 * charge);
		physicsManager.registerRigidbody(newPlayer->rigidbody);
		players.push_back(newPlayer);
		charge = 0.0f;
	}

	if (Input::getInput()->getKeyUp(GLFW_KEY_G))
	{
		Cube* newPlayer = new Cube;
		newPlayer->Initialize();
		newPlayer->position = camera.getPosition();
		physicsManager.registerRigidbody(newPlayer->rigidbody);
		cubes.push_back(newPlayer);
	}


	physX.Update(camera);
}

void TestingApplication::Draw()
{
	Gizmos::clear();

	Gizmos::addTransform(glm::mat4());
	Gizmos::addAABBFilled(vec3(0), vec3(20, 0.00001f, 20), vec4(0.9f, 0.5f, 0.5f, 1.f));
	cube.Draw();
	cube2.Draw();
	player.Draw();
	player2.Draw();
	spring->Draw();
	
	for (Spring* spring : springs)
	{
		spring->Draw();
	}
	
	for (Player* p : players)
	{
		p->Draw();
	}

	for (Cube* c : cubes)
	{
		c->Draw();
	}


	physX.Draw();

	Gizmos::draw(camera.getProjectionViewTransform());
	//imgui
	if (showUI)
	{
		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::Text("Push Escape to toggle gui");
		ImGui::DragFloat3("Light Direction + emmiter position", glm::value_ptr(player.position));
		ImGui::Render();
	}

}

void TestingApplication::Shutdown()
{
	for (Spring* spring : springs)
	{
		delete spring;
	}
	for (Player* p : players)
	{
		delete p;
	}

	for (Cube* c : cubes)
	{
		delete c;
	}
	delete spring;

	physX.Shutdown();
	ImGui_ImplGlfwGL3_Shutdown();
}