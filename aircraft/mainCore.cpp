#include "mainCore.h"
#include <exception>
#include <iostream>

using namespace std;

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glfw3.lib")
#pragma comment(lib,"assimp.lib")

mainCorePtr mainCore::_instance = nullptr;

mainCore::mainCore(int GLMAJOR /* = 2 */, int GLMINOR /* = 1 */)
{
	_versionMajor = GLMAJOR;
	_versionMinor = GLMINOR;
	_time = 0;
	_shutdown = false;
}
mainCore::~mainCore()
{
	if(_window)
		glfwDestroyWindow(_window);

	glfwTerminate();
}

mainCorePtr mainCore::get(int GLMajor,int GLMinor)
{

	if(_instance == nullptr)
	{
		_instance = mainCorePtr(new mainCore(GLMajor,GLMinor));
	}
	return _instance;
}

GLFWwindow* mainCore::getwindow()
{
	return _window;
}

GamePtr mainCore::getGame()
{
	return mainCore::get()->_game;
}

glm::vec2 mainCore::getMousePosition()
{
	return _mousePos;
}

void mainCore::setMousePosition(float x, float y)
{
	glfwSetCursorPos(_window,x,y);
	_mousePos = glm::vec2(x,y);
}

bool mainCore::init(unsigned int width /* = 800 */, unsigned int height /* = 600 */, std::string title /* = "Window" */)
{
	_width = width;
	_height = height;
	_title = title;
	try
	{
		initWindow();
		return true;
	}
	catch(exception e)
	{
		cerr<<e.what()<<endl;
		return false;
	}
}

void mainCore::initWindow()
{
	if(!glfwInit())
		throw exception("Cannot initialise GLFW");

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _versionMajor); //OpenGL version 3.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _versionMinor); // 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); //If requesting an OpenGL version below 3.2, GLFW_OPENGL_ANY_PROFILE

	_window = glfwCreateWindow(_width,_height,_title.c_str(),NULL,NULL);
	if(_window == nullptr)
		throw exception("Cannot create GLFW Window");

	glfwMakeContextCurrent(_window);

	if(glewInit() != GLEW_OK)
		throw exception("Cannot initialise GLEW");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void mainCore::start(GamePtr game)
{
	_game = game;
	glfwSetWindowSizeCallback(_window,&mainCore::windowResized);
	startLoop();
}
void mainCore::startLoop()
{
	while(!glfwWindowShouldClose(_window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		update();
		render();
		
		glfwSwapBuffers(_window);
		glfwPollEvents();
		//if shutdown then break the loop
		if(_shutdown)
			break;
	}
}

void mainCore::windowResized(GLFWwindow* window, int width, int height)
{
	mainCorePtr app = mainCore::get();
	app->_width = width;
	app->_height = height;
	glViewport(0,0,width,height);
}

void mainCore::update()
{
	double currentTime = glfwGetTime();
	double deltaTime = (currentTime - _time)*1000;
	_time = currentTime;

	_game->update(deltaTime);
}

void mainCore::render()
{
	_game->render();
}

glm::vec2 mainCore::getWindowSize()
{
	return glm::vec2(_width,_height);
}

void mainCore::shutdown()
{
	_shutdown = true;
}