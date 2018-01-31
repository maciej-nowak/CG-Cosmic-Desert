#include "Camera.h"


Camera::Camera(void)
{
	cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); //punkt docelowy, spojrzenia
	sphereCameraPosistion = glm::vec3(45.0f, -25.0f, 450.0f); //reprezentuje sferyczny uklad wspolrzednych, phi, theta [-90, 90], radiany
}


Camera::~Camera(void)
{
}

//Kamere ma pozycje i orientacje w worldspace. Te parametry definiuja transformacje z world do camera space
//zamiast ukladu wsp. normalnych posluzymy sie ukladem wsp. sferycznych, ktory ma 3 wartosci:
//r - (radiany) - dystans wspolrzednej od srodka ukladu wspolrzednych [0, nieskonoczonosc)
//phi - kat na plaszczyznie eliptycznej [0, 360)
//theta - kat powyzej lub ponizej plaszyzny eliptycznej [0, 180], 0 - prosto w gore, 180 - prosto w dol
//funkcja ta transformuje uklad wsp. sferycznych na Euklidesowy uklad geometryczny i jest okreslona wzorm
//WYLICZA POZYCJE KAMERY W WORLDSPACE
glm::vec3 Camera::ResolveCamPosition()
{
	glutil::MatrixStack tempMat;

	float phi = Framework::DegToRad(sphereCameraPosistion.x);
	float theta = Framework::DegToRad(sphereCameraPosistion.y + 90.0f); //dodajemy 90 bo nie jest z zakresu 0, 180 tylko -90, 90

	float fSinTheta = sinf(theta);
	float fCosTheta = cosf(theta);
	float fCosPhi = cosf(phi);
	float fSinPhi = sinf(phi);

	glm::vec3 dirToCamera(fSinTheta * fCosPhi, fCosTheta, fSinTheta * fSinPhi); //wektor kierunku
	return (dirToCamera * sphereCameraPosistion.z) + cameraTarget; //wyliczamy przestrzen Euklidesowa dodajac cel kamery, offset 
}

//KLAWIATURA DLA KAMERY
void Camera::keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{

	case 27:
		glutLeaveMainLoop();
		return;

	case 'w': cameraTarget.z -= 2.0f; break;
	case 's': cameraTarget.z += 2.0f; break;
	case 'd': cameraTarget.x += 2.0f; break;
	case 'a': cameraTarget.x -= 2.0f; break;
	case 'e': cameraTarget.y -= 2.0f; break;
	case 'q': cameraTarget.y += 2.0f; break;
	case 'i': sphereCameraPosistion.y -= 5.0f; break;
	case 'k': sphereCameraPosistion.y += 5.0f; break;
	case 'j': sphereCameraPosistion.x -= 5.0f; break;
	case 'l': sphereCameraPosistion.x += 5.0f; break;
	case 'o': sphereCameraPosistion.z -= 2.5f; break;
	case 'u': sphereCameraPosistion.z += 2.5f; break;

	//WYSWIETLANIE DANYCH DO KONSOLI
	case 32:
		printf("Target: %f, %f, %f\n", cameraTarget.x, cameraTarget.y, cameraTarget.z);
		printf("Position: %f, %f, %f\n", sphereCameraPosistion.x, sphereCameraPosistion.y, sphereCameraPosistion.z);
		break;
	}

	//SPRAWDZANIE ZAKRESU 
	sphereCameraPosistion.y = glm::clamp(sphereCameraPosistion.y, -89.00f, -5.0f); //zapobiega skokom kamery - nie pozwala za duzy lub zbyt maly obrot
	cameraTarget.y = cameraTarget.y > 0.0f ? cameraTarget.y : 0.0f; //nie pozwala na spogladanie spod planszy
	sphereCameraPosistion.z = sphereCameraPosistion.z > 5.0f ? sphereCameraPosistion.z : 5.0f; //nie pozwala przenikac obiektu

	glutPostRedisplay();
}

