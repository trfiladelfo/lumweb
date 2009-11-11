/*
 * runGraphics.h
 *
 *  Created on: 19.10.2009
 *      Author: root
 */

#ifndef RUNGRAPHICS_H_
#define RUNGRAPHICS_H_

xTaskHandle xGraphicTaskHandler;

void vGraphicObjectsTask(void *pvParameters);

#endif /* RUNGRAPHICS_H_ */
