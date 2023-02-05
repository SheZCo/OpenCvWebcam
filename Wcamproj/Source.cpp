#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <chrono>
#include <thread>
using namespace cv;

// ChatGpt Password from file Function -------------
std::string readFile(const std::string &filename) // & used to pass via reference, passes variables to function w/o making copy of variable
{
	std::ifstream file(filename);
	std::string fuknme;
	if (!file.is_open())
	{
		std::cout << "Enter a password: ";
		std::cin >> fuknme;
		
		std::ofstream new_file(filename);
		if (!new_file.is_open())
		{
			throw std::runtime_error("Error: Could not create file " + filename);
		}
		std::string* defpass = &fuknme;
		new_file << *defpass << std::endl;
		new_file.close();
		return *defpass;
	}
	std::string password;
	std::getline(file, password);
	file.close();
	return password;
}
// -------------------------------------------------


int main()
{
	//----- Gotta declare these up here ----------
	int mboxval;
	int* portal; // To store the message box in
	portal = &mboxval; // Empty integer ptr assigned to message box
	// -------------------------------------------

	// ---------- Pw nonsense --------------------------
	try {
		std::string pwrd = readFile("pass.txt");
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	std::string pwrd = readFile("pass.txt"); // Reads Pw from file
	std::string* pwpto = &pwrd; // Obfuscation ptr nonsense
	std::string check; // Pwd to enter
	// -------------------------------------------------

	// ---------------- Dont quit if wrong password entered
	while (check != *pwpto) // While the typed pw ! = password123, run
	{
		std::cout << "Enter your password to enter: "; // Ui helping
		std::cin >> check; // Take in password from user

	// ---------- Checks for pword
		if (check.length() < pwrd.length()) // If the password < pwrd length
		{
			std::cout << "Password too short, try again" << std::endl;
			Sleep(2000);
			system("cls"); 
			continue;
		}

		if (check.length() > pwrd.length()) // If input is longer than pwrd length
		{
			std::cout << "Password too long, try again" << std::endl;
			Sleep(2000);
			system("cls");
			continue;
		}
		if (check == *pwpto) // If it is right pw, continue on with operation
		{
			break; // basically the, continue
		}
		std::cout << "Wrong password, try again" << std::endl;
	}
	//--------------------------------------------

	// Open Cv Webcam Stuff --------------
	VideoCapture video(0, 0); // Video capture object, Streamdata/Webcam/Filename
	CascadeClassifier facedetect;
	Mat img; // stores current frame, frame processing
	std::vector<Rect> faces;
	const int fps = 120; // Fps cap
	

	if (check == *pwpto) {
		facedetect.load("C:/Users/sneki/OneDrive/Documents/Visual Studio 2022/libraries/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml"); // loads face finding module
		mboxval = MessageBoxA(NULL, "Start Webcam Feed ?", "WebCam", MB_ICONQUESTION | MB_YESNO); // Makes message box pop up
		double t = 0;
		double delta = 0.1;
		double M_PI = acos(-1);

		switch (*portal) { // Points to the message box, 6 is yes, 7 No. 
		case 6: // What happens if you press yes
			if (!video.isOpened()) // if not/cant open, dont do anything, so kill program
			{
				return -1;
			}
			// ------- if yes inits
			Sleep(3000);
			std::cout << "You clicked, Yes" << std::endl;
			video.set(CAP_PROP_FPS, fps); // Init Fps Cap
			// ----------
			while (video.read(img)) // while vid reads in frame information
			{
				video >> img; // stream operation to push next frames
				facedetect.detectMultiScale(img, faces, 1.3, 5);
				for (int pyp = 0; pyp < faces.size(); pyp++) { // For how many faces seen, do

					int red = (sin(t) + 1) * 128; // Rainbow Math
					int green = (sin(t + 2 * M_PI / 3) + 1) * 128; // ^
					int blue = (sin(t + 4 * M_PI / 3) + 1) * 128; // ^

					rectangle(img, faces[pyp].tl(), faces[pyp].br(), Scalar(blue, green, red), 3); // Draw Rectangle
					putText(img, std::to_string(faces.size()), Point(10, 40), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255), 1); // Count how many, "faces" found
				}
				putText(img, std::to_string(faces.size()) + "Face(s) found", Point(10, 40), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255), 1);

				imshow("Webcam", img); // Shows webcam, frames getting pushed 
				int key = waitKey(1);
				if (key >= 0) break;
				t += delta;
			}
			return true;
			// ----------------------------------------------------------------------------------
		case 7: // If pressed No
			Sleep(4000);
			std::cout << "You selected No" << std::endl;
			// Default cus Default
		default:
			return false;
		}
	}
	return 0;
}