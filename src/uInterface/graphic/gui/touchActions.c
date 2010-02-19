/**
 * \addtogroup Graphic
 * @{
 *
 * \file touchAction.c
 * \author Anziner, Hahn
 * \brief
 *
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "setup.h"
#include "touchActions.h"
#include "dislpayBasics.h"
#include "displayDraw.h"

#include "graphic/httpc/webClient.h"

#include "configuration/configloader.h"

void vTouchLoadMenu(tWidget *pWidget) {
	printf("vTouchLoadMenu: lade Menu\n");
	vLoadMenu();
	vInitializeMenuButton();
	printf("vTouchLoadMenu: menu loaded\n");
}

void vTouchStoreValues(tWidget *pWidget) {
	vInitDisplay();
	char* configLoad = loadFromConfig(IP_CONFIG_FILE, "DEFAULT_SET_PAGE");
	vLoadWebPage(configLoad, xDisplayRoot.entities);
	vPortFree(configLoad);

	vDrawElementsOnDisplay();
}

void vPageUp(tWidget *pWidget) {
	if (elementOffset > 0) {
		elementOffset--;
		vInitDisplay();
		vDrawElementsOnDisplay();
		vInitializeUpButton();
	}
}

void vPageDown(tWidget *pWidget) {
	elementOffset++;
	vInitDisplay();
	vDrawElementsOnDisplay();
	vInitializeDownButton();
}

void vHyperlinkAction(tWidget *pWidget) {
	basicDisplayLine *root = xDisplayRoot.entities;
	while (root != NULL) {
		if (root->valueWidget == pWidget) {
			xDisplayRoot.menue = true;
			vInitDisplay();
			vLoadWebPage(root->strValue, NULL);
			vDrawElementsOnDisplay();
			break;
		}
		root = root->next;
	}
}
