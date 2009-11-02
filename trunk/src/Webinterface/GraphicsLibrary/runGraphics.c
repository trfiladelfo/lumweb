/**
 * Start Library for the Graphics Task
 * @author anzinger
 * @version 1.0/14-10-09
 */

#include "GraphicsLibrary/graphicObjects.h"
#include "GraphicsLibrary/graphicTextbox.h"
#include "GraphicsLibrary/graphicButton.h"
#include "GraphicsLibrary/runGraphics.h"
#include "GraphicsLibrary/renderGraphics.h"

#include "string.h"

void vGraphicObjectsTask(void *pvParameters)
{

	char **todo;

	todo = pvPortMalloc(8 * sizeof (char*));

	todo[0] = (char*) pvPortMalloc(9 * sizeof (char));
	strncpy(todo[0], "day_hour\0", 9);

	todo[1] = (char*) pvPortMalloc(11 * sizeof (char));
	strncpy(todo[1], "day_minute\0", 11);

	todo[2] = (char*) pvPortMalloc(11 * sizeof (char));
	strncpy(todo[2], "night_hour\0", 11);

	todo[3] = (char*) pvPortMalloc(13* sizeof (char));
	strncpy(todo[3], "night_minute\0", 13);

	todo[4] = (char*) pvPortMalloc(9 * sizeof (char));
	strncpy(todo[4], "day_hour\0", 9);

	todo[5] = (char*) pvPortMalloc(11 * sizeof (char));
	strncpy(todo[5], "day_minute\0", 11);

	todo[6] = (char*) pvPortMalloc(11 * sizeof (char));
	strncpy(todo[6], "night_hour\0", 11);

	todo[7] = (char*) pvPortMalloc(13 * sizeof (char));
	strncpy(todo[7], "night_minutx\0", 13);

	goInit();

	render((char**) todo, (int)8);

	//goNewTextBox(7, 5, 10, "day_hour");
	//goNewTextBox(7, 5, 24, "day_minute");
	//goNewTextBox(7, 5, 38, "night_hour");
	//goNewTextBox(7, 5, 52, "night_minute");
	//goNewTextBox(7, 5, 66, "day_hour");
	//goNewTextBox(7, 5, 80, "day_minute");
	//goNewTextBox(7, 5, 94, "night_hour");
	//goNewTextBox(7, 5, 108, "night_minute");

	//b = goNewButton(10, 10, 114, 10, goButtonUp, pucBorderNormal,
	//		vTextBoxIncrement);
	//b1 = goNewButton(10, 10, 114, 24, goButtonDown, pucBorderNormal,
	//		vTextBoxDecrement);

	goObjectsListener(xGraphicTaskHandler);

	while (1)
	{
		// ENDLESS LOOP!
	}
}

void vScreenSaver(void)
{
	goDisplaySleep();
}
