#include "Shader.h"
#include "Objects.h"
#include "Camera.h"
#include "Matrixes.h"

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

//obiekty programu
Shader objectShader; 
Shader ballShader; //nieuzywany
Shader textureShader; 
Shader planeteShader; 
Objects objects; 
Camera camera; 

//inicjalizacja obiektow
void InitializeObjects()
{
	objectShader = Shader::LoadProgram("object.vert", "color.frag");
	ballShader = Shader::LoadProgram("ball.vert", "color.frag");
	textureShader = Shader::LoadProgram("texture.vert", "texture.frag");
	planeteShader = Shader::LoadProgram("planete.vert", "texture.frag");
	objects.LoadMeshes();
	objects.LoadModels();
}

//inicjalizacja programu
void init()
{
	glutFullScreen(); 
	glutSetWindowTitle("Maciej Nowak - Projekt GRK");
	InitializeObjects(); 

	glEnable(GL_CULL_FACE); //cull facing - wlaczamy nierysowanie niewidocznych powierzchni
	glCullFace(GL_BACK); //nie bedzie renderowal strony tylniej
	glFrontFace(GL_CW); //okreslamy strone przednia - CW - wierzcholki ulozone zgodnie z ruchem wskazowek zegara

	glEnable(GL_DEPTH_TEST); //testowanie glebokosci rysowania, bryly na siebie nie nachodza, rysowane jest to co jest blizej, wartosc Z, w przeciwnym razie decyduje kolejnosc rysowania
	glDepthMask(GL_TRUE); //wlaczamy zapis do depth buffer, Z-buffer
	glDepthFunc(GL_LEQUAL); //wartos jaka bedzie uzywal depth buffer do porownywania, pobiera wartosc i porownuje z obecna wartosc w depth bufferze, w tym wypadku rysuje to co jest z przodu, dzieki LEQUAL gdy 2 obiekty na siebie nachodza to jes do dobrze rysowane
	glDepthRange(0.0f, 1.0f); //ustawiamy zakres glebokosci, od 0 do 1 (pelne), czyli min i max wartosci w depth buffer
}

//obsluga klawiszy specjalnych
void SpecialInput(int key, int x, int y)
{
	objects.keyboardArrows(key, x, y); //sterowanie obiektem (statkiem) za pomoca klawiszy specjalnych
} 

//rendering obrazu
void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //ustawienie koloru czyszczenia (tlo)
	glClearDepth(1.0f); //ustawianie glebokosci czyszczenia - nie mniejsze niz 2 argument glDepthRange
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //czyszczenie z 2 opcjami wczesniej ustawionymi, czyszczenie bufora koloru

	//tworzenie worldToCamera matrix
	const glm::vec3 &camPos = camera.ResolveCamPosition();  //oblicza pozycje kamery w worldSpace
	glutil::MatrixStack camMatrix;
	camMatrix.LookAt(camPos, camera.cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f)); //pozycja kamery, punkt w worldSpace na ktory ma patrzec, wektor spojrzenia (rotacja)

	//ustawianie macierzy kamery dla kazdego shadera
	glUseProgram(objectShader.theProgram);
	glUniformMatrix4fv(objectShader.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(camMatrix.Top()));
	glUseProgram(textureShader.theProgram);
	glUniformMatrix4fv(textureShader.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(camMatrix.Top()));
	glUseProgram(planeteShader.theProgram);
	glUniformMatrix4fv(planeteShader.worldToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(camMatrix.Top()));
	glUseProgram(0);

	//rysowanie obiektow
	glutil::MatrixStack modelMatrix;

	//PUSTYNIA
	{
		glutil::PushStack push(modelMatrix); //odkladamy biezaca macierz na stos
		objects.DrawDesert(modelMatrix, textureShader, objects.desert);
	}

	//ZIEMIA
	{
		glutil::PushStack push(modelMatrix);	
		modelMatrix.Scale(6.0f, 6.0f, 6.0f);
		modelMatrix.Translate(glm::vec3(0.0f, 20.0f, -30.0f));
		glm::vec3 offset = glm::vec3(2.0f, 0.5f, 3.0f);
		objects.DrawPlanete(modelMatrix, planeteShader, objects.earth, offset);
	}

	//MARS
	{
		glutil::PushStack push(modelMatrix);		
		modelMatrix.Scale(4.0f, 4.0f, 4.0f);
		modelMatrix.Translate(glm::vec3(-30.0f, 25.0f, -25.0f));
		glm::vec3 offset = glm::vec3(2.0f, 5.5f, 3.0f);
		objects.DrawPlanete(modelMatrix, planeteShader, objects.mars, offset);
	}

	//KSIEZYC
	{
		glutil::PushStack push(modelMatrix);		
		modelMatrix.Scale(6.0f, 6.0f, 6.0f);
		modelMatrix.Translate(glm::vec3(0.0f, 20.0f, -30.0f));
		glm::vec3 offset = glm::vec3(10.0f, 0.0f, 10.0f);
		objects.DrawPlanete(modelMatrix, planeteShader, objects.moon, offset);
	}

	//SLONCE
	{
		glutil::PushStack push(modelMatrix);		
		modelMatrix.Scale(12.0f, 12.0f, 12.0f);
		modelMatrix.Translate(glm::vec3(-20.0f, 10.0f, 0.0f));
		glm::vec3 offset = glm::vec3(2.0f, 0.0f, 2.0f);
		objects.DrawPlanete(modelMatrix, planeteShader, objects.sun, offset);
	}
		
	//PIRAMIDA 1
	{
		glutil::PushStack push(modelMatrix);
		modelMatrix.Scale(6.0f, 6.0f, 6.0f);
		modelMatrix.Translate(glm::vec3(10.0f, 0.0f, -10.0f));
		objects.DrawPyramid(modelMatrix, objectShader);
	}

	//PIRAMIDA 2
	{
		glutil::PushStack push(modelMatrix);
		modelMatrix.Scale(4.0f, 4.0f, 4.0f);
		modelMatrix.Translate(glm::vec3(-12.0f, 0.0f, -20.0f));
		objects.DrawPyramid(modelMatrix, objectShader);
	}

	//PIRAMIDA 3
	{
		glutil::PushStack push(modelMatrix);
		modelMatrix.Scale(3.0f, 3.0f, 3.0f);
		modelMatrix.Translate(glm::vec3(30.0f, 0.0f, 20.0f));
		objects.DrawPyramid(modelMatrix, objectShader);
	}

	//DRZEWO 1
	{
		glutil::PushStack push(modelMatrix);
		modelMatrix.Scale(2.0f, 2.0f, 2.0f);
		modelMatrix.Translate(glm::vec3(-30.0f, 3.0f, 20.0f));
		objects.DrawTree(modelMatrix, objectShader);
	}

	//DRZEWO 2
	{
		glutil::PushStack push(modelMatrix);
		modelMatrix.Scale(2.0f, 2.0f, 2.0f);
		modelMatrix.Translate(glm::vec3(-20.0f, 3.0f, 10.0f));
		objects.DrawTree(modelMatrix, objectShader);
	}

	//DRZEWO 3
	{
		glutil::PushStack push(modelMatrix);
		modelMatrix.Scale(2.0f, 2.0f, 2.0f);
		modelMatrix.Translate(glm::vec3(-25.0f, 3.0f, 0.0f));
		objects.DrawTree(modelMatrix, objectShader);
	}

	//STATEK
	{
		objects.DrawShip(objectShader);
	}

	glutSpecialFunc(SpecialInput); //wywolanie metody obslugi klawiszy specjalnych

	glutSwapBuffers(); //zamienia buffory obrazu, sa 2 buffory i dzieki temu ze sie przelaczaja nie ma efektu klatkowania
	glutPostRedisplay(); //odswiezanie zawartosci okna
}

//obsluga zmiany wielkosci okna
void reshape (int w, int h)
{
	//tworzenie macierzy perspektywy - projekcji
	glutil::MatrixStack persMatrix;
	persMatrix.Perspective(45.0f, (w / (float)h), 1.0f, 1000.0f); //odleglosc, proporcje, renderowanie z odleglosci [1, 1000] (jak np 100 - 1000, to nie renderuje tego co jest mniej niz 100 w stosunku do kamery
	//ustawianie macierzy kamery dla kazdego shadera
	glUseProgram(objectShader.theProgram);
	glUniformMatrix4fv(objectShader.cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(persMatrix.Top()));
	glUseProgram(textureShader.theProgram);
	glUniformMatrix4fv(textureShader.cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(persMatrix.Top()));
	glUseProgram(planeteShader.theProgram);
	glUniformMatrix4fv(planeteShader.cameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(persMatrix.Top()));
	glUseProgram(0);

	glViewport(0, 0, (GLsizei) w, (GLsizei) h); //modyfikacja obszaru renderingu, 0, 0 - wsp dolnego lewego obszaru renderingu wzgledem dolnego lewego naroznika okna
	glutPostRedisplay(); 
}

//obsluga klawiatury
void keyboard(unsigned char key, int x, int y)
{
	camera.keyboard(key, x, y); //sterowanie kamera
	objects.keyboard(key, x, y); //sterowanie obiektem (statkiem)
}

//wartosci domyslne programu
unsigned int defaults(unsigned int displayMode, int &width, int &height) 
{
	return displayMode;
}
