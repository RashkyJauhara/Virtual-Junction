#pragma once
#include "RenderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>

class Demo :
	public RenderEngine
{
public:
	Demo();
	~Demo();
private:
	GLuint shaderProgram,
		VBO, VAO, EBO, texture, //table1
		VBO2, VAO2, EBO2, texture2, //plane
		VBO3, VAO3, EBO3, texture3, //table2
		VBO4, VAO4, EBO4, texture4, //pilar1
		VBO5, VAO5, EBO5, texture5, //table3
		VBO6, VAO6, EBO6, texture6, //table4
		VBO7, VAO7, EBO7, texture7, //pilar2
		VBO8, VAO8, EBO8, texture8, //tangga
		VBO9, VAO9, EBO9, texture9, //tembok f
		VBO10, VAO10, EBO10, texture10, //tembok g
		VBO11, VAO11, EBO11, texture11,//atap
		VBO12, VAO12, EBO12, texture12, //depan
		VBO13, VAO13, EBO13, texture13; //belakang
	float viewCamX, viewCamY, viewCamZ, upCamX, upCamY, upCamZ, posCamX, posCamY, posCamZ, CAMERA_SPEED, fovy;
	virtual void Init();
	virtual void DeInit();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void ProcessInput(GLFWwindow *window);
	
	void BuildTable1();
	void DrawTable1();
	
	void BuildTable2();
	void DrawTable2();
	
	void BuildTable3();
	void DrawTable3();
	
	void BuildTable4();
	void DrawTable4();

	void BuildPilar1();
	void DrawPilar1();

	void BuildPilar2();
	void DrawPilar2();

	void BuildTangga();
	void DrawTangga();

	void BuildTembokF();
	void DrawTembokF();

	void BuildTembokG();
	void DrawTembokG();

	void BuildAtap();
	void DrawAtap();

	void BuildTembokDepan();
	void DrawTembokDepan();

	void BuildTembokBelakang();
	void DrawTembokBelakang();

	void BuildTexturedPlane();
	void DrawColoredPlane();

	void MoveCamera(float speed);
	void StrafeCamera(float speed);
	void RotateCamera(float speed);
	void InitCamera();
};

