#pragma once
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Game.h"
#include <glm.hpp>


class mainCore
{
private:

	int _versionMajor,_versionMinor;
	GLFWwindow* _window;
	unsigned int _width, _height;
	std::string _title;
	GamePtr _game;
	double _time;
	static std::shared_ptr<mainCore> _instance;
	glm::vec2 _mousePos;
	bool _shutdown;
	void initWindow();
	void startLoop();

	void update();
	void render();
	static void windowResized(GLFWwindow* window, int width, int height);
	mainCore(int GLMAJOR = 2, int GLMINOR = 1);


public:
	static std::shared_ptr<mainCore> get(int GLMajor=2,int GLMinor = 1);
	static GamePtr getGame();
	glm::vec2 getMousePosition();
	void setMousePosition(float x, float y);
	~mainCore();
	bool init(unsigned int width = 800, unsigned int height = 600, std::string title = "Window");
	void start(GamePtr game);
	glm::vec2 getWindowSize();
	void shutdown();


	GLFWwindow* getwindow();
};
typedef std::shared_ptr<mainCore> mainCorePtr;