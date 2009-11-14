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

	// XXX nur ein Test, soll nachher aus Config geladen werden
	todo = pvPortMalloc(4 * sizeof (char*));

	todo[0] = (char*) pvPortMalloc(9 * sizeof (char));
	strncpy(todo[0], "day_hour\0", 9);

	todo[1] = (char*) pvPortMalloc(11 * sizeof (char));
	strncpy(todo[1], "day_minute\0", 11);

	todo[2] = (char*) pvPortMalloc(11 * sizeof (char));
	strncpy(todo[2], "night_hour\0", 11);

	todo[3] = (char*) pvPortMalloc(13* sizeof (char));
	strncpy(todo[3], "night_minute\0", 13);

	// Ende Test

	goInit();

	render((char**) todo, 4);

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
