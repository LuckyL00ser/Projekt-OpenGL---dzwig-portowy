#pragma once
#include "AntTweakBar/include/AntTweakBar.h"
#include "Camera.h"


extern float lightColor[];
extern float lightPos[];

class WorldBar
{
public:
	WorldBar();
	~WorldBar();
	void refresh();
	void windowResize(int width, int height);
	void init(TwButtonCallback cameraRun, void *camera);
	TwBar* swiatBar;
private:
	
	void createBar(TwButtonCallback cameraRun, void *camera);
};

