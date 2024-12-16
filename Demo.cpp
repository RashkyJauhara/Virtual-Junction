#include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildTable1();
	BuildTable2();
	BuildTable3();
	BuildTable4();

	BuildPilar1();
	BuildPilar2();

	BuildTangga();

	BuildTembokF();
	BuildTembokG();

	BuildAtap();

	BuildTembokDepan();
	BuildTembokBelakang();

	BuildTexturedPlane();

	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	
	// zoom camera
	// -----------
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.0001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.0001f;
		}
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth/2;
	double midY = screenHeight/2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);


	

}

void Demo::Update(double deltaTime) {
	
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.678f, 0.847f, 0.9f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));



	DrawTable1();
	DrawTable2();
	DrawTable3();
	DrawTable4();

	DrawPilar1();
	DrawPilar2();

	DrawTangga();

	DrawTembokF();
	DrawTembokG();

	DrawAtap();

	DrawTembokDepan();
	DrawTembokBelakang();

	DrawColoredPlane();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildTable1() {
	// Texture loading for table and chairs
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("dark-wood.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Vertex data for table and chairs
	float vertices[] = {
		//MEJA
		// atas
		-1.0, 0.5,  0.5,  0, 0,  // 0
		1.0, 0.5,  0.5,  1, 0,  // 1
		1.0, 0.5, -0.5,  1, 1,  // 2
		-1.0, 0.5, -0.5,  0, 1,  // 3

		// depan kiri
		-0.9,  0.0,  0.4,  0, 0,  // 4
		-0.8,  0.0,  0.4,  1, 0,  // 5
		-0.8,  0.5,  0.4,  1, 1,  // 6
		-0.9,  0.5,  0.4,  0, 1,  // 7

		// depan kanan
		0.8,  0.0,  0.4,  0, 0,  // 8
		0.9,  0.0,  0.4,  1, 0,  // 9
		0.9,  0.5,  0.4,  1, 1,  // 10
		0.8,  0.5,  0.4,  0, 1,  // 11

		// belakang kiri
		-0.9,  0.5, -0.4,  0, 0,  // 12
		-0.8,  0.5, -0.4,  1, 0,  // 13
		-0.8,  0.0, -0.4,  1, 1,  // 14
		-0.9,  0.0, -0.4,  0, 1,  // 15

		// belakang kanan
		0.8,  0.5, -0.4,  0, 0,  // 16
		0.9,  0.5, -0.4,  1, 0,  // 17
		0.9,  0.0, -0.4,  1, 1,  // 18
		0.8,  0.0, -0.4,  0, 1,  // 19

		//KURSI1
		//atas
		-0.9, 0.3,  -0.6,  0, 0,  // 20
		0.9, 0.3,  -0.6,  1, 0,  // 21
		0.9, 0.3, -1.1,  1, 1,  // 22
		-0.9, 0.3, -1.1,  0, 1,  // 23

		// depan kiri
		-0.8,  0.0,  -0.7,  0, 0,  // 24
		-0.7,  0.0,  -0.7,  1, 0,  // 25
		-0.7,  0.3,  -0.7,  1, 1,  // 26
		-0.8,  0.3,  -0.7,  0, 1,  // 27

		// depan kanan
		0.8,  0.0,  -0.7,  0, 0,  // 28
		0.7,  0.0,  -0.7,  1, 0,  // 29
		0.7,  0.3,  -0.7,  1, 1,  // 30
		0.8,  0.3,  -0.7,  0, 1,  // 31

		// belakang kiri
		-0.8,  0.0,  -1,  0, 0,  // 32
		-0.7,  0.0,  -1,  1, 0,  // 33
		-0.7,  0.3,  -1,  1, 1,  // 34
		-0.8,  0.3,  -1,  0, 1,  // 35

		// belakang kanan
		0.8,  0.0,  -1,  0, 0,  // 36
		0.7,  0.0,  -1,  1, 0,  // 37
		0.7,  0.3,  -1,  1, 1,  // 38
		0.8,  0.3,  -1,  0, 1,  // 39

		//KURSI2
		//atas
		-0.9, 0.3, 0.6, 0, 0,  // 40
		0.9, 0.3, 0.6, 1, 0,  // 41
		0.9, 0.3, 1.1, 1, 1,  // 42
		-0.9, 0.3, 1.1, 0, 1,  // 43

		// depan kiri
		-0.8,  0.0, 0.7, 0, 0,  // 44
		-0.7,  0.0, 0.7, 1, 0,  // 45
		-0.7,  0.3, 0.7, 1, 1,  // 46
		-0.8,  0.3, 0.7, 0, 1,  // 47

		// depan kanan
		0.8,  0.0, 0.7, 0, 0,  // 48
		0.7,  0.0, 0.7, 1, 0,  // 49
		0.7,  0.3, 0.7, 1, 1,  // 50
		0.8,  0.3, 0.7, 0, 1,  // 51

		// belakang kiri
		-0.8,  0.0, 1, 0, 0,  // 52
		-0.7,  0.0, 1, 1, 0,  // 53
		-0.7,  0.3, 1, 1, 1,  // 54
		-0.8,  0.3, 1, 0, 1,  // 55

		// belakang kanan
		0.8,  0.0, 1,  0, 0,  // 56
		0.7,  0.0, 1,  1, 0,  // 57
		0.7,  0.3, 1,  1, 1,  // 58
		0.8,  0.3, 1,  0, 1,  // 59

	};

	unsigned int indices[] = {
		//MEJA
		0, 1, 2, 0, 2, 3, //atas
		4, 5, 6, 4, 6, 7, // depan kiri
		8, 9, 10, 8, 10, 11, // depan kanan
		12, 13, 14, 12, 14, 15, // belakang kiri
		16, 17, 18, 16, 18, 19, // belakang kanan

		//KURSI1
		20, 21, 22, 20, 22, 23, //atas
		24, 25, 26, 24, 26, 27, //depan kiri
		28, 29, 30, 28, 30, 31, // depan kanan
		32, 33, 34, 32, 34, 35, // belakang kiri
		36, 37, 38, 36, 38, 39, // belakang kanan

		40, 41, 42, 40, 42, 43,
		44, 45, 46, 44, 46, 47,
		48, 49, 50, 48, 50, 51,
		52, 53, 54, 52, 54, 55,
		56, 57, 58, 56, 58, 59
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// Define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTable1()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTable2() {
	// Texture loading for table and chairs
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("dark-wood.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Vertex data for table and chairs
	float vertices[] = {
		//MEJA
		// atas
		-6.0, 0.5,  0.5,  0, 0,  // 0
		-4.0, 0.5,  0.5,  1, 0,  // 1
		-4.0, 0.5, -0.5,  1, 1,  // 2
		-6.0, 0.5, -0.5,  0, 1,  // 3

		// depan kiri
		-5.9,  0.0,  0.4,  0, 0,  // 4
		-5.8,  0.0,  0.4,  1, 0,  // 5
		-5.8,  0.5,  0.4,  1, 1,  // 6
		-5.9,  0.5,  0.4,  0, 1,  // 7

		// depan kanan
		-4.2,  0.0,  0.4,  0, 0,  // 8
		-4.3,  0.0,  0.4,  1, 0,  // 9
		-4.3,  0.5,  0.4,  1, 1,  // 10
		-4.2,  0.5,  0.4,  0, 1,  // 11

		// belakang kiri
		-5.9,  0.5, -0.4,  0, 0,  // 12
		-5.8,  0.5, -0.4,  1, 0,  // 13
		-5.8,  0.0, -0.4,  1, 1,  // 14
		-5.9,  0.0, -0.4,  0, 1,  // 15

		// belakang kanan
		-4.2,  0.5, -0.4,  0, 0,  // 16
		-4.3,  0.5, -0.4,  1, 0,  // 17
		-4.3,  0.0, -0.4,  1, 1,  // 18
		-4.2,  0.0, -0.4,  0, 1,  // 19

		//KURSI1
		//atas
		-5.9, 0.3,  -0.6,  0, 0,  // 20
		-4.1, 0.3,  -0.6,  1, 0,  // 21
		-4.1, 0.3, -1.1,  1, 1,  // 22
		-5.9, 0.3, -1.1,  0, 1,  // 23

		// depan kiri
		-5.8,  0.0,  -0.7,  0, 0,  // 24
		-5.7,  0.0,  -0.7,  1, 0,  // 25
		-5.7,  0.3,  -0.7,  1, 1,  // 26
		-5.8,  0.3,  -0.7,  0, 1,  // 27

		// depan kanan
		-4.2,  0.0,  -0.7,  0, 0,  // 28
		-4.3,  0.0,  -0.7,  1, 0,  // 29
		-4.3,  0.3,  -0.7,  1, 1,  // 30
		-4.2,  0.3,  -0.7,  0, 1,  // 31

		// belakang kiri
		-5.8,  0.0,  -1,  0, 0,  // 32
		-5.7,  0.0,  -1,  1, 0,  // 33
		-5.7,  0.3,  -1,  1, 1,  // 34
		-5.8,  0.3,  -1,  0, 1,  // 35

		// belakang kanan
		-4.2,  0.0,  -1,  0, 0,  // 36
		-4.3,  0.0,  -1,  1, 0,  // 37
		-4.3,  0.3,  -1,  1, 1,  // 38
		-4.2,  0.3,  -1,  0, 1,  // 39

		//KURSI2
		//atas
		-5.9, 0.3, 0.6, 0, 0,  // 40
		-4.1, 0.3, 0.6, 1, 0,  // 41
		-4.1, 0.3, 1.1, 1, 1,  // 42
		-5.9, 0.3, 1.1, 0, 1,  // 43

		// depan kiri
		-5.8,  0.0, 0.7, 0, 0,  // 44
		-5.7,  0.0, 0.7, 1, 0,  // 45
		-5.7,  0.3, 0.7, 1, 1,  // 46
		-5.8,  0.3, 0.7, 0, 1,  // 47

		// depan kanan
		-4.2,  0.0, 0.7, 0, 0,  // 48
		-4.3,  0.0, 0.7, 1, 0,  // 49
		-4.3,  0.3, 0.7, 1, 1,  // 50
		-4.2,  0.3, 0.7, 0, 1,  // 51

		// belakang kiri
		-5.8,  0.0, 1, 0, 0,  // 52
		-5.7,  0.0, 1, 1, 0,  // 53
		-5.7,  0.3, 1, 1, 1,  // 54
		-5.8,  0.3, 1, 0, 1,  // 55

		// belakang kanan
		-4.2,  0.0, 1,  0, 0,  // 56
		-4.3,  0.0, 1,  1, 0,  // 57
		-4.3,  0.3, 1,  1, 1,  // 58
		-4.2,  0.3, 1,  0, 1,  // 59

	};

	unsigned int indices[] = {
		//MEJA
		0, 1, 2, 0, 2, 3, //atas
		4, 5, 6, 4, 6, 7, // depan kiri
		8, 9, 10, 8, 10, 11, // depan kanan
		12, 13, 14, 12, 14, 15, // belakang kiri
		16, 17, 18, 16, 18, 19, // belakang kanan

		//KURSI1
		20, 21, 22, 20, 22, 23, //atas
		24, 25, 26, 24, 26, 27, //depan kiri
		28, 29, 30, 28, 30, 31, // depan kanan
		32, 33, 34, 32, 34, 35, // belakang kiri
		36, 37, 38, 36, 38, 39, // belakang kanan

		40, 41, 42, 40, 42, 43,
		44, 45, 46, 44, 46, 47,
		48, 49, 50, 48, 50, 51,
		52, 53, 54, 52, 54, 55,
		56, 57, 58, 56, 58, 59
	};

	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);

	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// Define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTable2()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO3); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTable3() {
	// Texture loading for table and chairs
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("dark-wood.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Vertex data for table and chairs
	float vertices[] = {
		//MEJA
		// atas
		-1.0, 0.5, 5.5,  0, 0,  // 0
		1.0, 0.5,  5.5,  1, 0,  // 1
		1.0, 0.5,  4.5,  1, 1,  // 2
		-1.0, 0.5, 4.5,  0, 1,  // 3

		// depan kiri
		-0.9,  0.0,  5.4,  0, 0,  // 4
		-0.8,  0.0,  5.4,  1, 0,  // 5
		-0.8,  0.5,  5.4,  1, 1,  // 6
		-0.9,  0.5,  5.4,  0, 1,  // 7

		// depan kanan
		0.8,  0.0,  5.4,  0, 0,  // 8
		0.9,  0.0,  5.4,  1, 0,  // 9
		0.9,  0.5,  5.4,  1, 1,  // 10
		0.8,  0.5,  5.4,  0, 1,  // 11

		// belakang kiri
		-0.9,  0.5, 4.6,  0, 0,  // 12
		-0.8,  0.5, 4.6,  1, 0,  // 13
		-0.8,  0.0, 4.6,  1, 1,  // 14
		-0.9,  0.0, 4.6,  0, 1,  // 15

		// belakang kanan
		0.8,  0.5, 4.6,  0, 0,  // 16
		0.9,  0.5, 4.6,  1, 0,  // 17
		0.9,  0.0, 4.6,  1, 1,  // 18
		0.8,  0.0, 4.6,  0, 1,  // 19

		//KURSI1
		//atas
		-0.9, 0.3,  4.4,  0, 0,  // 20
		0.9, 0.3,  4.4,  1, 0,  // 21
		0.9, 0.3, 3.9,  1, 1,  // 22
		-0.9, 0.3, 3.9,  0, 1,  // 23

		// depan kiri
		-0.8,  0.0,  4.3,  0, 0,  // 24
		-0.7,  0.0,  4.3,  1, 0,  // 25
		-0.7,  0.3,  4.3,  1, 1,  // 26
		-0.8,  0.3,  4.3,  0, 1,  // 27

		// depan kanan
		0.8,  0.0,  4.3,  0, 0,  // 28
		0.7,  0.0,  4.3,  1, 0,  // 29
		0.7,  0.3,  4.3,  1, 1,  // 30
		0.8,  0.3,  4.3,  0, 1,  // 31

		// belakang kiri
		-0.8,  0.0,  4,  0, 0,  // 32
		-0.7,  0.0,  4,  1, 0,  // 33
		-0.7,  0.3,  4,  1, 1,  // 34
		-0.8,  0.3,  4,  0, 1,  // 35

		// belakang kanan
		0.8,  0.0,  4,  0, 0,  // 36
		0.7,  0.0,  4,  1, 0,  // 37
		0.7,  0.3,  4,  1, 1,  // 38
		0.8,  0.3,  4,  0, 1,  // 39

		//KURSI2
		//atas
		-0.9, 0.3, 6.1, 0, 0,  // 40
		0.9, 0.3, 6.1, 1, 0,  // 41
		0.9, 0.3, 5.6, 1, 1,  // 42
		-0.9, 0.3, 5.6, 0, 1,  // 43

		// depan kiri
		-0.8,  0.0, 6, 0, 0,  // 44
		-0.7,  0.0, 6, 1, 0,  // 45
		-0.7,  0.3, 6, 1, 1,  // 46
		-0.8,  0.3, 6, 0, 1,  // 47

		// depan kanan
		0.8,  0.0, 6, 0, 0,  // 48
		0.7,  0.0, 6, 1, 0,  // 49
		0.7,  0.3, 6, 1, 1,  // 50
		0.8,  0.3, 6, 0, 1,  // 51

		// belakang kiri
		-0.8,  0.0, 5.7, 0, 0,  // 52
		-0.7,  0.0, 5.7, 1, 0,  // 53
		-0.7,  0.3, 5.7, 1, 1,  // 54
		-0.8,  0.3, 5.7, 0, 1,  // 55

		// belakang kanan
		0.8,  0.0, 5.7,  0, 0,  // 56
		0.7,  0.0, 5.7,  1, 0,  // 57
		0.7,  0.3, 5.7,  1, 1,  // 58
		0.8,  0.3, 5.7,  0, 1,  // 59

	};

	unsigned int indices[] = {
		//MEJA
		0, 1, 2, 0, 2, 3, //atas
		4, 5, 6, 4, 6, 7, // depan kiri
		8, 9, 10, 8, 10, 11, // depan kanan
		12, 13, 14, 12, 14, 15, // belakang kiri
		16, 17, 18, 16, 18, 19, // belakang kanan

		//KURSI1
		20, 21, 22, 20, 22, 23, //atas
		24, 25, 26, 24, 26, 27, //depan kiri
		28, 29, 30, 28, 30, 31, // depan kanan
		32, 33, 34, 32, 34, 35, // belakang kiri
		36, 37, 38, 36, 38, 39, // belakang kanan

		40, 41, 42, 40, 42, 43,
		44, 45, 46, 44, 46, 47,
		48, 49, 50, 48, 50, 51,
		52, 53, 54, 52, 54, 55,
		56, 57, 58, 56, 58, 59
	};

	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	glGenBuffers(1, &EBO5);

	glBindVertexArray(VAO5);

	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// Define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTable3()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO5); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTable4() {
	// Texture loading for table and chairs
	glGenTextures(1, &texture6);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("dark-wood.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Vertex data for table and chairs
	float vertices[] = {
		//MEJA
		// atas
		-6, 0.5, 5.5,  0, 0,  // 0
		-4, 0.5,  5.5,  1, 0,  // 1
		-4, 0.5,  4.5,  1, 1,  // 2
		-6, 0.5, 4.5,  0, 1,  // 3

		// depan kiri
		-5.9,  0.0,  5.4,  0, 0,  // 4
		-5.8,  0.0,  5.4,  1, 0,  // 5
		-5.8,  0.5,  5.4,  1, 1,  // 6
		-5.9,  0.5,  5.4,  0, 1,  // 7

		// depan kanan
		-4.2,  0.0,  5.4,  0, 0,  // 8
		-4.3,  0.0,  5.4,  1, 0,  // 9
		-4.3,  0.5,  5.4,  1, 1,  // 10
		-4.2,  0.5,  5.4,  0, 1,  // 11

		// belakang kiri
		-5.9,  0.5, 4.6,  0, 0,  // 12
		-5.8,  0.5, 4.6,  1, 0,  // 13
		-5.8,  0.0, 4.6,  1, 1,  // 14
		-5.9,  0.0, 4.6,  0, 1,  // 15

		// belakang kanan
		-4.2,  0.5, 4.6,  0, 0,  // 16
		-4.3,  0.5, 4.6,  1, 0,  // 17
		-4.3,  0.0, 4.6,  1, 1,  // 18
		-4.2,  0.0, 4.6,  0, 1,  // 19

		//KURSI1
		//atas
		-5.9, 0.3,  4.4,  0, 0,  // 20
		-4.1, 0.3,  4.4,  1, 0,  // 21
		-4.1, 0.3, 3.9,  1, 1,  // 22
		-5.9, 0.3, 3.9,  0, 1,  // 23

		// depan kiri
		-5.8,  0.0,  4.3,  0, 0,  // 24
		-5.7,  0.0,  4.3,  1, 0,  // 25
		-5.7,  0.3,  4.3,  1, 1,  // 26
		-5.8,  0.3,  4.3,  0, 1,  // 27

		// depan kanan
		-4.2,  0.0,  4.3,  0, 0,  // 28
		-4.3,  0.0,  4.3,  1, 0,  // 29
		-4.3,  0.3,  4.3,  1, 1,  // 30
		-4.2,  0.3,  4.3,  0, 1,  // 31

		// belakang kiri
		-5.8,  0.0,  4,  0, 0,  // 32
		-5.7,  0.0,  4,  1, 0,  // 33
		-5.7,  0.3,  4,  1, 1,  // 34
		-5.8,  0.3,  4,  0, 1,  // 35

		// belakang kanan
		-4.2,  0.0,  4,  0, 0,  // 36
		-4.3,  0.0,  4,  1, 0,  // 37
		-4.3,  0.3,  4,  1, 1,  // 38
		-4.2,  0.3,  4,  0, 1,  // 39

		//KURSI2
		//atas
		-5.9, 0.3, 6.1, 0, 0,  // 40
		-4.1, 0.3, 6.1, 1, 0,  // 41
		-4.1, 0.3, 5.6, 1, 1,  // 42
		-5.9, 0.3, 5.6, 0, 1,  // 43

		// depan kiri
		-5.8,  0.0, 6, 0, 0,  // 44
		-5.7,  0.0, 6, 1, 0,  // 45
		-5.7,  0.3, 6, 1, 1,  // 46
		-5.8,  0.3, 6, 0, 1,  // 47

		// depan kanan
		-4.2,  0.0, 6, 0, 0,  // 48
		-4.3,  0.0, 6, 1, 0,  // 49
		-4.3,  0.3, 6, 1, 1,  // 50
		-4.2,  0.3, 6, 0, 1,  // 51

		// belakang kiri
		-5.8,  0.0, 5.7, 0, 0,  // 52
		-5.7,  0.0, 5.7, 1, 0,  // 53
		-5.7,  0.3, 5.7, 1, 1,  // 54
		-5.8,  0.3, 5.7, 0, 1,  // 55

		// belakang kanan
		-4.2,  0.0, 5.7,  0, 0,  // 56
		-4.3,  0.0, 5.7,  1, 0,  // 57
		-4.3,  0.3, 5.7,  1, 1,  // 58
		-4.2,  0.3, 5.7,  0, 1,  // 59

	};

	unsigned int indices[] = {
		//MEJA
		0, 1, 2, 0, 2, 3, //atas
		4, 5, 6, 4, 6, 7, // depan kiri
		8, 9, 10, 8, 10, 11, // depan kanan
		12, 13, 14, 12, 14, 15, // belakang kiri
		16, 17, 18, 16, 18, 19, // belakang kanan

		//KURSI1
		20, 21, 22, 20, 22, 23, //atas
		24, 25, 26, 24, 26, 27, //depan kiri
		28, 29, 30, 28, 30, 31, // depan kanan
		32, 33, 34, 32, 34, 35, // belakang kiri
		36, 37, 38, 36, 38, 39, // belakang kanan

		40, 41, 42, 40, 42, 43,
		44, 45, 46, 44, 46, 47,
		48, 49, 50, 48, 50, 51,
		52, 53, 54, 52, 54, 55,
		56, 57, 58, 56, 58, 59
	};

	glGenVertexArrays(1, &VAO6);
	glGenBuffers(1, &VBO6);
	glGenBuffers(1, &EBO6);

	glBindVertexArray(VAO6);

	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO6);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// Define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTable4()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO6); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 90, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildPilar1() {
	// Texture loading for table and chairs
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("pale-yellow.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float vertices[] = {
		// atas
		-3.0, 8,  0.5,  0, 0,  // 0
		-2.0, 8,  0.5,  1, 0,  // 1
		-2.0, 8, -0.5,  1, 1,  // 2
		-3.0, 8, -0.5,  0, 1,  // 3

		// bawah
		-3.0,  0.0,  0.5,  0, 0,  // 4
		-2.0,  0.0,  0.5,  1, 0,  // 5
		-2.0,  0.0,  -0.5,  1, 1,  // 6
		-3.0,  0.0,  -0.5,  0, 1,  // 7

		// kiri
		-3.0,  8,  0.5,  0, 0,  // 8
		-3.0,  8, -0.5,  1, 0,  // 9
		-3.0,  0,  -0.5,  1, 1,  // 10
		-3.0,  0,   0.5,  0, 1,  // 11

		// kanan
		-2.0, 8,  -0.5,  0, 0,  // 12
		-2.0, 8,  0.5,  1, 0,  // 13
		-2.0, 0.0, 0.5,  1, 1,  // 14
		-2.0, 0.0, -0.5,  0, 1,  // 15

		// depan
		-3,  8, -0.5,  0, 0,  // 16
		-2,  8, -0.5,  1, 0,  // 17
		-2,  0.0, -0.5,  1, 1,  // 18
		-3,  0.0, -0.5,  0, 1,  // 19

		// belakang
		-3, 8,  0.5,  0, 0,  // 20
		-2, 8,  0.5,  1, 0,  // 21
		-2, 0.0,  0.5,  1, 1,  // 22
		-3, 0.0,  0.5,  0, 1,  // 23
	};

	unsigned int indices[] = {
		0, 1, 2, 0, 2, 3, //atas
		4, 5, 6, 4, 6, 7, // depan kiri
		8, 9, 10, 8, 10, 11, // depan kanan
		12, 13, 14, 12, 14, 15, // belakang kiri
		16, 17, 18, 16, 18, 19, // belakang kanan
		20, 21, 22, 20, 22, 23
	};

	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glGenBuffers(1, &EBO4);

	glBindVertexArray(VAO4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// Define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawPilar1()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO4); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildPilar2() {
	// Texture loading for table and chairs
	glGenTextures(1, &texture7);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("pale-yellow.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float vertices[] = {
		// atas
		-3.0, 8,  5.5,  0, 0,  // 0
		-2.0, 8,  5.5,  1, 0,  // 1
		-2.0, 8,  4.5,  1, 1,  // 2
		-3.0, 8,  4.5,  0, 1,  // 3

		// bawah
		-3.0,  0.0,  5.5,  0, 0,  // 4
		-2.0,  0.0,  5.5,  1, 0,  // 5
		-2.0,  0.0,  4.5,  1, 1,  // 6
		-3.0,  0.0,  4.5,  0, 1,  // 7

		// kiri
		-3.0,  8,  5.5,  0, 0,  // 8
		-3.0,  8,  4.5,  1, 0,  // 9
		-3.0,  0,   4.5,  1, 1,  // 10
		-3.0,  0,   5.5,  0, 1,  // 11

		// kanan
		-2.0, 8,  4.5,  0, 0,  // 12
		-2.0, 8,  5.5,  1, 0,  // 13
		-2.0, 0.0, 5.5,  1, 1,  // 14
		-2.0, 0.0, 4.5,  0, 1,  // 15

		// depan
		-3,  8,  4.5,  0, 0,  // 16
		-2,  8,  4.5,  1, 0,  // 17
		-2,  0.0, 4.5,  1, 1,  // 18
		-3,  0.0, 4.5,  0, 1,  // 19

		// belakang
		-3, 8,  5.5,  0, 0,  // 20
		-2, 8,  5.5,  1, 0,  // 21
		-2, 0.0,  5.5,  1, 1,  // 22
		-3, 0.0,  5.5,  0, 1,  // 23
	};

	unsigned int indices[] = {
		0, 1, 2, 0, 2, 3, //atas
		4, 5, 6, 4, 6, 7, // depan kiri
		8, 9, 10, 8, 10, 11, // depan kanan
		12, 13, 14, 12, 14, 15, // belakang kiri
		16, 17, 18, 16, 18, 19, // belakang kanan
		20, 21, 22, 20, 22, 23
	};

	glGenVertexArrays(1, &VAO7);
	glGenBuffers(1, &VBO7);
	glGenBuffers(1, &EBO7);

	glBindVertexArray(VAO7);

	glBindBuffer(GL_ARRAY_BUFFER, VBO7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO7);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// Define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawPilar2()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO7); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTangga() {
	// Texture loading for table and chairs
	glGenTextures(1, &texture8);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("18191.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float vertices[] = {
		//1
		-8, 0.2,  -2,  0, 0,  // 0
		-8, 0.2,   7,  1, 0,  // 1
		-8, 0.0,   7,  1, 1,  // 2
		-8, 0.0,  -2,  0, 1,  // 3

		-8.5, 0.2,  7, 0, 0,  // 4
		-8,   0.2,  7, 1, 0,  // 5
		-8,   0.2, -2, 1, 1,  // 6
		-8.5, 0.2, -2, 0, 1,  // 7
		
		//2
		-8.5, 0.4,  -2,  0, 0,  // 8
		-8.5, 0.4,   7,  1, 0,  // 9
		-8.5, 0.2,   7,  1, 1,  // 10
		-8.5, 0.2,  -2,  0, 1,  // 11

		-9,   0.4,  7, 0, 0,  // 12
		-8.5, 0.4,  7, 1, 0,  // 13
		-8.5, 0.4, -2, 1, 1,  // 14
		-9,   0.4, -2, 0, 1,  // 15

		//3
		-9, 0.6,  -2,  0, 0,  // 16
		-9, 0.6,   7,  1, 0,  // 17
		-9, 0.4,   7,  1, 1,  // 18
		-9, 0.4,  -2,  0, 1,  // 19

		-9.5, 0.6,  7,  0, 0,  // 20
		-9,   0.6,  7,  1, 0,  // 21
		-9,   0.6, -2,  1, 1,  // 22
		-9.5, 0.6, -2,  0, 1,  // 23

		//4
		-9.5, 0.8,  -2,  0, 0,  // 24
		-9.5, 0.8,  7,  1, 0,  // 25
		-9.5, 0.6,  7,  1, 1,  // 26
		-9.5, 0.6,  -2,  0, 1,  // 27

		-10,   0.8, 7,  0, 0,  // 28
		-9.5,  0.8, 7,  1, 0,  // 29
		-9.5,  0.8, -2,  1, 1,  // 30
		-10,   0.8, -2,  0, 1,  // 31

		//kanan
		-10,  0.2, -2,  0, 0,  // 32
		-8,   0.2, -2,  1, 0,  // 33
		-8,   0.0, -2,  1, 1,  // 34
		-10,  0.0, -2,  0, 1,  // 35

		-10,  0.4, -2,  0, 0,  // 32
		-8.5, 0.4, -2,  1, 0,  // 33
		-8.5, 0.2, -2,  1, 1,  // 34
		-10,  0.2, -2,  0, 1,  // 35

		-10,  0.6, -2,  0, 0,  // 36
		-9,   0.6, -2,  1, 0,  // 37
		-9,   0.4, -2,  1, 1,  // 38
		-10,  0.4, -2,  0, 1,  // 39

		-10,  0.8, -2,  0, 0,  // 40
		-9.5, 0.8, -2,  1, 0,  // 41
		-9.5, 0.6, -2,  1, 1,  // 42
		-10,  0.6, -2,  0, 1,  // 43

		//kiri
		-10,  0.2, 7,  0, 0,  // 44
		-8,   0.2, 7,  1, 0,  // 45
		-8,   0.0, 7,  1, 1,  // 46
		-10,  0.0, 7,  0, 1,  // 47

		-10,  0.4, 7,  0, 0,  // 48
		-8.5, 0.4, 7,  1, 0,  // 49
		-8.5, 0.2, 7,  1, 1,  // 50
		-10,  0.2, 7,  0, 1,  // 51

		-10,  0.6, 7,  0, 0,  // 52
		-9,   0.6, 7,  1, 0,  // 53
		-9,   0.4, 7,  1, 1,  // 54
		-10,  0.4, 7,  0, 1,  // 55

		-10,  0.8, 7,  0, 0,  // 56
		-9.5, 0.8, 7,  1, 0,  // 57
		-9.5, 0.6, 7,  1, 1,  // 58
		-10,  0.6, 7,  0, 1,  // 59

		//belakang
		-10, 0.8,  7,  0, 0,  // 44
		-10, 0.8, -2,  1, 0,  // 45
		-10, 0.0, -2,  1, 1,  // 46
		-10, 0.0,  7,  0, 1,  // 47
	};

	unsigned int indices[] = {
		0, 1, 2, 0, 2, 3, 
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11, 
		12, 13, 14, 12, 14, 15, 
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23,
		24, 25, 26, 24, 26, 27,
		28, 29, 30, 28, 30, 31,
		32, 33, 34, 32, 34, 35,
		36, 37, 38, 36, 38, 39,
		40, 41, 42, 40, 42, 43,
		44, 45, 46, 44, 46, 47,
		48, 49, 50, 48, 50, 51,
		52, 53, 54, 52, 54, 55,
		56, 57, 58, 56, 58, 59,
		60, 61, 62, 60, 62, 63,
		64, 65, 66, 64, 66, 67
	};

	glGenVertexArrays(1, &VAO8);
	glGenBuffers(1, &VBO8);
	glGenBuffers(1, &EBO8);

	glBindVertexArray(VAO8);

	glBindBuffer(GL_ARRAY_BUFFER, VBO8);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO8);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// Define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTangga()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO8); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 102, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTembokF() {
	// Texture loading for table and chairs
	glGenTextures(1, &texture9);
	glBindTexture(GL_TEXTURE_2D, texture9);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("pale-yellow.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float vertices[] = {
		//kiri
		-10, 8,   -2,  0, 0,  // 0
		-10, 8,    1,  1, 0,  // 1
		-10, 0.8,   1,  1, 1,  // 2
		-10, 0.8,  -2,  0, 1,  // 3

		//tengah
		-10, 8,   1,  0, 0,  // 4
		-10, 8,   4,  1, 0,  // 5
		-10, 2.5,  4,  1, 1,  // 6
		-10, 2.5,  1,  0, 1,  // 7

		//kanan
		-10, 8,   4,  0, 0,  // 8
		-10, 8,   7,  1, 0,  // 9
		-10, 0.8,  7,  1, 1,  // 10
		-10, 0.8,  4,  0, 1,  // 11
	};

	unsigned int indices[] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11
	};

	glGenVertexArrays(1, &VAO9);
	glGenBuffers(1, &VBO9);
	glGenBuffers(1, &EBO9);

	glBindVertexArray(VAO9);

	glBindBuffer(GL_ARRAY_BUFFER, VBO9);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO9);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// Define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTembokF()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture9);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO9); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTembokG() {
	glGenTextures(1, &texture10);
	glBindTexture(GL_TEXTURE_2D, texture10);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("pale-yellow.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float vertices[] = {
		//kanan
		3, 8,   -2,  0, 0,  // 0
		3, 8,    1,  1, 0,  // 1
		3, 0.0,   1,  1, 1,  // 2
		3, 0.0,  -2,  0, 1,  // 3

		//tengah
		3, 8,   1,  0, 0,  // 4
		3, 8,   4,  1, 0,  // 5
		3, 2,  4,  1, 1,  // 6
		3, 2,  1,  0, 1,  // 7

		//kiri
		3, 8,   4,  0, 0,  // 8
		3, 8,   7,  1, 0,  // 9
		3, 0.0,  7,  1, 1,  // 10
		3, 0.0,  4,  0, 1,  // 11
	};

	unsigned int indices[] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11
	};

	glGenVertexArrays(1, &VAO10);
	glGenBuffers(1, &VBO10);
	glGenBuffers(1, &EBO10);

	glBindVertexArray(VAO10);

	glBindBuffer(GL_ARRAY_BUFFER, VBO10);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO10);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// Define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTembokG()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture10);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO10); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildAtap() {
	glGenTextures(1, &texture11);
	glBindTexture(GL_TEXTURE_2D, texture11);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("pale-yellow.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float vertices[] = {
		-10, 8,   7,  0, 0,  // 0
		  3, 8,   7,  1, 0,  // 1
		  3, 8,  -2,  1, 1,  // 2
		-10, 8,  -2,  0, 1,  // 3
	};

	unsigned int indices[] = {
		0, 1, 2, 0, 2, 3,
	};

	glGenVertexArrays(1, &VAO11);
	glGenBuffers(1, &VBO11);
	glGenBuffers(1, &EBO11);

	glBindVertexArray(VAO11);

	glBindBuffer(GL_ARRAY_BUFFER, VBO11);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO11);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// Define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawAtap()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture11);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO11); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTembokDepan() {
	glGenTextures(1, &texture12);
	glBindTexture(GL_TEXTURE_2D, texture12);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("pale-yellow.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float vertices[] = {
		-10, 8,  7,  0, 0,  // 0
		  3, 8,  7,  1, 0,  // 1
		  3, 4,  7,  1, 1,  // 2
		-10, 4,  7,  0, 1,  // 3
	};

	unsigned int indices[] = {
		0, 1, 2, 0, 2, 3,
	};

	glGenVertexArrays(1, &VAO12);
	glGenBuffers(1, &VBO12);
	glGenBuffers(1, &EBO12);

	glBindVertexArray(VAO12);

	glBindBuffer(GL_ARRAY_BUFFER, VBO12);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO12);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// Define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTembokDepan()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture12);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO12); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTembokBelakang() {
	glGenTextures(1, &texture13);
	glBindTexture(GL_TEXTURE_2D, texture13);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("pale-yellow.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float vertices[] = {
		-10, 8,  -2,  0, 0,  // 0
		  3, 8,  -2,  1, 0,  // 1
		  3, 4,  -2,  1, 1,  // 2
		-10, 4,  -2,  0, 1,  // 3
	};

	unsigned int indices[] = {
		0, 1, 2, 0, 2, 3,
	};

	glGenVertexArrays(1, &VAO13);
	glGenBuffers(1, &VBO13);
	glGenBuffers(1, &EBO13);

	glBindVertexArray(VAO13);

	glBindBuffer(GL_ARRAY_BUFFER, VBO13);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO13);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// Define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTembokBelakang()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture13);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO13); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void Demo::BuildTexturedPlane()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("light-gray-marble.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-10.0, 0,  7.0,  0,  0,
		  3.0, 0,  7.0, 13,  0,
		  3.0, 0, -2.0, 13, 9,
		-10.0, 0, -2.0,  0, 9,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawColoredPlane()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 1.0f;
	posCamZ = 8.0f;
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.001f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}


int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Camera: Free Camera Implementation", 800, 600, false, true);
}