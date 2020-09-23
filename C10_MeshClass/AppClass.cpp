#include "AppClass.h"
void Application::InitVariables(void)
{

	//Make objects
	cubes = 46;
	for (int i = 0; i < cubes; i++) {
		MyMesh* pMesh = new MyMesh();
		cubeList.push_back(pMesh);
		cubeList[i]->GenerateCube(1.0f, C_WHITE);
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//taken from solution, gives matrix based on camera
	matrix4 projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 view = m_pCameraMngr->GetViewMatrix();

	//increment timer for movement across screen
	static float timer = 0;
	static int ticks = m_pSystem->GenClock();
	timer += static_cast<float>(m_pSystem->GetDeltaTime(ticks));

	//start objects on left of screen
	float distanceToSide = -15.0f;

	//use this vector to adjust translation
	vector3 positionVector(distanceToSide, 0.0f, 0.0f);

	//add onto position vector with timer values
	positionVector.x = distanceToSide + timer;

	//vector math to translate in future lines of code
	vector3 startingPos = vector3(-5.0f, -3.0f, -15.0f);
	matrix4 positionTranslate = glm::translate(startingPos) * glm::translate(positionVector);

	//getting cubes on screen
	int i = 0;
	//top
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(2.0f, 7.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(8.0f, 7.0f, 0.0f))); i++;

	//2nd
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(3.0f, 6.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(7.0f, 6.0f, 0.0f))); i++;

	//3rd
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(2.0f, 5.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(3.0f, 5.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(4.0f, 5.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(5.0f, 5.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(6.0f, 5.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(7.0f, 5.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(8.0f, 5.0f, 0.0f))); i++;

	//4th
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(1.0f, 4.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(2.0f, 4.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(4.0f, 4.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(5.0f, 4.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(6.0f, 4.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(8.0f, 4.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(9.0f, 4.0f, 0.0f))); i++;

	//5th
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(0.0f, 3.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(1.0f, 3.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(2.0f, 3.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(3.0f, 3.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(4.0f, 3.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(5.0f, 3.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(6.0f, 3.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(7.0f, 3.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(8.0f, 3.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(9.0f, 3.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(10.0f, 3.0f, 0.0f))); i++;

	//6th
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(0.0f, 2.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(2.0f, 2.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(3.0f, 2.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(4.0f, 2.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(5.0f, 2.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(6.0f, 2.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(7.0f, 2.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(8.0f, 2.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(10.0f, 2.0f, 0.0f))); i++;

	//7th
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(0.0f, 1.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(2.0f, 1.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(8.0f, 1.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(10.0f, 1.0f, 0.0f))); i++;

	//8th
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(3.0f, 0.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(4.0f, 0.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(6.0f, 0.0f, 0.0f))); i++;
	cubeList[i]->Render(projection, view, glm::translate(positionTranslate, vector3(7.0f, 0.0f, 0.0f))); i++;
	

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	std::vector<MyMesh*> cubeList;
	int size = cubeList.size();
	for (int i = 0; i < size; ++i)
	{
		if (cubeList[i] != nullptr)
		{
			delete cubeList[i];
			cubeList[i] = nullptr;
		}
	}
	//release GUI
	ShutdownGUI();
}