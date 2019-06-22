#include "WorldBar.h"
#include <iostream>

WorldBar::WorldBar()
{
}


WorldBar::~WorldBar()
{
}

void WorldBar::refresh()
{
	TwRefreshBar(swiatBar);
}

void WorldBar::windowResize(int width, int height)
{
	int pos[2], size[2];
	TwGetParam(swiatBar, NULL, "size", TW_PARAM_INT32, 2, size);
	pos[0] = width - size[0] - 16;
	pos[1] = 16;
	TwSetParam(swiatBar, NULL, "position", TW_PARAM_INT32, 2, pos);
	//std::cout << "\nSzer: " << width << "\nWys: " << height << std::endl;
}

void WorldBar::init(TwButtonCallback callback, void *camera)
{
	createBar(callback, camera);
}

void WorldBar::createBar(TwButtonCallback callback, void *camera)
{
	//std::cout << lightPos[0] << " " << lightPos[1] << " " << lightPos[2];
	swiatBar = TwNewBar("otoczenie");
	lightPos[0] = 50;
	TwDefine(" otoczenie label='Parametry otoczenia' position='1384 16' ");
	TwAddVarRW(swiatBar, "lightPos", TW_TYPE_DIR3F, &lightPos, " label='Kierunek oswietlenia' ");
	TwAddVarRW(swiatBar, "lightColor", TW_TYPE_COLOR3F, &lightColor, " label='Kolor swiatla' ");
	TwAddButton(swiatBar, "Kamera", callback, camera, " label='Wlaczenie kamery' key=C ");
}