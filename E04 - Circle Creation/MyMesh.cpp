#include "MyMesh.h"
void MyMesh::GenerateCircle(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	Release();
	Init();

	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	/*
		Calculate a_nSubdivisions number of points around a center point in a radial manner
		then call the AddTri function to generate a_nSubdivision number of faces
	*/


	//make vector array to store points
	std::vector<vector3 > vertex;
	std::vector<vector3 > vertex2;
	std::vector<vector3 > vertex3;



	//starting point for outer circle points
	GLfloat first = 0;

	//this gives us how much we want to go around the circle by to begin the next triangle in the circle
	GLfloat second = (2 * PI / a_nSubdivisions);

	//for each subdivision, calculate the x and y positions for the outside points
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//make a point in space relative to center point
		//cos gives x pos
		//sin gives y pos
		//z is depth, not needed for circle
		vector3 temp = vector3(
			sin(first) * a_fRadius,
			cos(first) * a_fRadius,
			0.0f
			
		);

		//add onto the outer angle to begin at the next point
		first += second;

		//add point onto vertex array
		vertex.push_back(temp);
	}

	//calculate triangles and their respective points
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//first is the center of the circle, second is the vertex that was pushed to the array, 
		//third is the next point after the previous tri adjusted to the number of subdivisions
		AddTri(
			vector3(0.0f,0.0f,0.0f), 
			vertex[(i + 1) % a_nSubdivisions],
			vertex[i]
		);
	}

	
	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

																									   // Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader);

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));

	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

													   //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}

void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	Mesh* pMesh = new Mesh();
	pMesh->GenerateCone(a_fRadius, a_fHeight, a_nSubdivisions, a_v3Color);
	m_lVertexPos = pMesh->GetVertexList();
	m_uVertexCount = m_lVertexPos.size();
	SafeDelete(pMesh);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//make vector array to store points
	std::vector<vector3 > vertex;
	std::vector<vector3 > vertexCopy;
	GLfloat height = a_fHeight / 2.0f;


	//starting point for outer circle points
	GLfloat first = 0;

	//this gives us how much we want to go around the circle by to begin the next triangle in the circle
	GLfloat second = (2 * PI / a_nSubdivisions);

	//for each subdivision, calculate the x and y positions for the outside points
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//make a point in space relative to center point
		//cos gives x pos
		//sin gives y pos
		//z is depth, not needed for circle
		GLfloat sin_angle = sin(first) * a_fRadius;
		GLfloat cos_angle = cos(first) * a_fRadius;
		
		vector3 temp = vector3(
			sin_angle,
			height,
			cos_angle
		);
		temp = temp - vector3(0.0f, height, 0.0f);
		vector3 tempCopyVector = vector3(
		    sin_angle,
			height,
			cos_angle);
		tempCopyVector = tempCopyVector + vector3(0.0f, height, 0.0f);

		//add onto the outer angle to begin at the next point
		first += second;

		//add point onto vertex array
		vertex.push_back(temp);
		vertexCopy.push_back(tempCopyVector);
	}

	//calculate triangles and their respective points
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//first is the center of the circle, second is the vertex that was pushed to the array, 
		//third is the next point after the previous tri adjusted to the number of subdivisions
		//top
		AddTri(
			vector3(0.0f, 0.0f , 0.0f)-vector3(0.0f, height, 0.0f),
			vertex[(i + 1) % a_nSubdivisions] - vector3(0.0f, height, 0.0f),
			vertex[i] - vector3(0.0f, height, 0.0f)
			
		);
		//bottom
		AddTri(
			vector3(0.0f, height*2, 0.0f) - vector3(0.0f, height, 0.0f),
			vertexCopy[i] - vector3(0.0f, height, 0.0f),
			vertexCopy[(i + 1) % a_nSubdivisions] - vector3(0.0f, height, 0.0f)
		);

	}

	for (int i = 0; i < a_nSubdivisions; i++) {
		AddQuad(vertexCopy[(i + 1) % a_nSubdivisions] - vector3(0.0f, height, 0.0f), 
			vertexCopy[i] - vector3(0.0f, height, 0.0f), 
			vertex[(i + 1) % a_nSubdivisions] - vector3(0.0f, height, 0.0f), 
			vertex[i] - vector3(0.0f, height, 0.0f));
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	/*
	Mesh* pMesh = new Mesh();
	pMesh->GenerateTube(a_fOuterRadius, a_fInnerRadius, a_fHeight, a_nSubdivisions, a_v3Color);
	m_lVertexPos = pMesh->GetVertexList();
	m_uVertexCount = m_lVertexPos.size();
	SafeDelete(pMesh);
	// -------------------------------
	*/

	//make vector array to store points
	std::vector<vector3 > vertex;
	std::vector<vector3 > vertexCopy;
	std::vector<vector3 > innerCircle_lower;
	std::vector<vector3 > innerCircle_upper;


	GLfloat height = a_fHeight / 2.0f;


	//starting point for outer circle points
	GLfloat first = 0;

	//this gives us how much we want to go around the circle by to begin the next triangle in the circle
	GLfloat second = (2 * PI / a_nSubdivisions);

	//generate circle
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//make a point in space relative to center point
		//cos gives x pos
		//sin gives y pos
		//z is depth, not needed for circle
		GLfloat sin_angle = sin(first) * a_fOuterRadius;
		GLfloat cos_angle = cos(first) * a_fOuterRadius;

		GLfloat sin_angle_inner = sin(first) * a_fInnerRadius;
		GLfloat cos_angle_inner = cos(first) * a_fInnerRadius;

		vector3 temp = vector3(
			sin_angle,
			height,
			cos_angle
		);
		temp = temp - vector3(0.0f, height, 0.0f);
		vector3 tempCopyVector = vector3(
			sin_angle,
			height,
			cos_angle);
		tempCopyVector = tempCopyVector + vector3(0.0f, height, 0.0f);

		vector3 tempInner = vector3(
			sin_angle_inner,
			height,
			cos_angle_inner
		);
		tempInner = tempInner - vector3(0.0f, height, 0.0f);

		vector3 tempInnerCopyVector = vector3(
			sin_angle_inner,
			height,
			cos_angle_inner
		);
		tempInnerCopyVector = tempInnerCopyVector + vector3(0.0f, height, 0.0f);

		//add onto the outer angle to begin at the next point
		first += second;

		//add point onto vertex array
		vertex.push_back(temp);
		vertexCopy.push_back(tempCopyVector);

		innerCircle_upper.push_back(tempInner);
		innerCircle_lower.push_back(tempInnerCopyVector);


	}

	for (int i = 0; i < a_nSubdivisions; i++) {
		//outter
		AddQuad(
			vertexCopy[(i + 1) % a_nSubdivisions] - vector3(0.0f, height, 0.0f),
			vertexCopy[i] - vector3(0.0f, height, 0.0f),
			vertex[(i + 1) % a_nSubdivisions] - vector3(0.0f, height, 0.0f),
			vertex[i] - vector3(0.0f, height, 0.0f)
		);

		//inner
		AddQuad(
			innerCircle_upper[(i + 1) % a_nSubdivisions] - vector3(0.0f, height, 0.0f),
			innerCircle_upper[i] - vector3(0.0f, height, 0.0f),
			innerCircle_lower[(i + 1) % a_nSubdivisions] - vector3(0.0f, height, 0.0f),
			innerCircle_lower[i] - vector3(0.0f, height, 0.0f)
		);

		//top
		AddQuad(
			innerCircle_lower[(i + 1) % a_nSubdivisions] - vector3(0.0f, height, 0.0f),
			innerCircle_lower[i] - vector3(0.0f, height, 0.0f),
			vertexCopy[(i + 1) % a_nSubdivisions] - vector3(0.0f, height, 0.0f),
			vertexCopy[i] - vector3(0.0f, height, 0.0f)
		);
		//bottom
		AddQuad(
			vertex[(i + 1) % a_nSubdivisions] - vector3(0.0f, height, 0.0f),
			vertex[i] - vector3(0.0f, height, 0.0f),
			innerCircle_upper[(i + 1) % a_nSubdivisions] - vector3(0.0f, height, 0.0f),
			innerCircle_upper[i] - vector3(0.0f, height, 0.0f)
			
		);
	}
	
	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	/* Mesh* pMesh = new Mesh();
	pMesh->GenerateTorus(a_fOuterRadius, a_fInnerRadius, a_nSubdivisionsA, a_nSubdivisionsB, a_v3Color);
	m_lVertexPos = pMesh->GetVertexList();
	m_uVertexCount = m_lVertexPos.size();
	SafeDelete(pMesh);
	// -------------------------------
	*/

	//make vector array to store points
	std::vector<vector3 > vertex;
	std::vector<vector3 > vertex2;
	std::vector<vector3 > vertex3;
	std::vector<vector3 > vertex4;

	//radius of circle
	GLfloat radius = ((a_fOuterRadius - a_fInnerRadius) / 2);

	//circle centers should this far from 0
	GLfloat distanceFromZero = a_fInnerRadius + radius;

	//starting point for outer circle points
	GLfloat first = 0;

	//this gives us how much we want to go around the circle by to begin the next triangle in the circle
	GLfloat second = (2 * PI / a_nSubdivisionsA);

	//gen circle
	for (int i = 0; i < a_nSubdivisionsA; i++)
	{
		vector3 temp = vector3(
			sin(first) * radius,
			cos(first) * radius,
			0.0f

		);

		//rotate temp vector3 by subdivision(first)


		//translate temp vector3 using distanceFromZero as distance to move
		//1 is position, 0 is direction
		temp = glm::vec3(glm::translate(vector3(distanceFromZero, 0, 0)) * vector4(temp, 1));
		temp = glm::rotate(IDENTITY_M4, glm::radians(0.0f), AXIS_Y)*vector4(temp, 1);

		//add onto the outer angle to begin at the next point
		first += second;

		//add point onto vertex array
		vertex.push_back(temp);

		//-------------------------------- TEST

		vector3 temp2 = vector3(
			sin(first) * radius,
			cos(first) * radius,
			0.0f

		);

		//rotate temp vector3 by subdivision(first)


		//translate temp vector3 using distanceFromZero as distance to move
		//1 is position, 0 is direction
		temp2 = glm::vec3(glm::translate(vector3(distanceFromZero, 0, 0)) * vector4(temp2, 1));
		temp2 = glm::rotate(IDENTITY_M4, glm::radians(90.0f), AXIS_Y) * vector4(temp2, 1);

		//add onto the outer angle to begin at the next point
		//first += second;

		//add point onto vertex array
		vertex2.push_back(temp2);

		//-------------------------------- TEST

		vector3 temp3 = vector3(
			sin(first) * radius,
			cos(first) * radius,
			0.0f

		);

		//rotate temp vector3 by subdivision(first)


		//translate temp vector3 using distanceFromZero as distance to move
		//1 is position, 0 is direction
		temp3 = glm::vec3(glm::translate(vector3(distanceFromZero, 0, 0)) * vector4(temp3, 1));
		temp3 = glm::rotate(IDENTITY_M4, glm::radians(180.0f), AXIS_Y) * vector4(temp3, 1);

		//add onto the outer angle to begin at the next point
		//first += second;

		//add point onto vertex array
		vertex3.push_back(temp3);

		//-------------------------------- TEST

		vector3 temp4 = vector3(
			sin(first) * radius,
			cos(first) * radius,
			0.0f

		);

		//rotate temp vector3 by subdivision(first)


		//translate temp vector3 using distanceFromZero as distance to move
		//1 is position, 0 is direction
		temp4 = glm::vec3(glm::translate(vector3(distanceFromZero, 0, 0)) * vector4(temp4, 1));
		temp4 = glm::rotate(IDENTITY_M4, glm::radians(270.0f), AXIS_Y) * vector4(temp4, 1);

		//add onto the outer angle to begin at the next point
		//first += second;

		//add point onto vertex array
		vertex4.push_back(temp4);


	}

	//calculate triangles and their respective points
	for (int i = 0; i < a_nSubdivisionsA; i++)
	{
		//first is the center of the circle, second is the vertex that was pushed to the array, 
		//third is the next point after the previous tri adjusted to the number of subdivisions
		AddQuad(
			vertex2[(i + 1) % a_nSubdivisionsA],
			vertex2[i],
			vertex[(i + 1) % a_nSubdivisionsA],
			vertex[i]
		);
		AddQuad(
			vertex3[(i + 1) % a_nSubdivisionsA],
			vertex3[i],
			vertex2[(i + 1) % a_nSubdivisionsA],
			vertex2[i]
		);
		AddQuad(
			vertex4[(i + 1) % a_nSubdivisionsA],
			vertex4[i],
			vertex3[(i + 1) % a_nSubdivisionsA],
			vertex3[i]
		);
		AddQuad(
			vertex[(i + 1) % a_nSubdivisionsA],
			vertex[i],
			vertex4[(i + 1) % a_nSubdivisionsA],
			vertex4[i]
		);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	// Replace this with your code
	Mesh* pMesh = new Mesh();
	pMesh->GenerateSphere(a_fRadius, a_nSubdivisions, a_v3Color);
	m_lVertexPos = pMesh->GetVertexList();
	m_uVertexCount = m_lVertexPos.size();
	SafeDelete(pMesh);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}