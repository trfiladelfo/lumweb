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
#include "displayBasics.h"
#include "displayDraw.h"

#include "graphic/httpc/webClient.h"
#include "graphic/gui/valueEditor.h"

#include "configuration/configloader.h"

void vTouchLoadMenu(tWidget *pWidget) {
#if DEBUG_GRAPHIC
	printf("vTouchLoadMenu: lade Menu\n");
#endif

	vLoadMenu();
	vInitializeMenuButton();

#if DEBUG_GRAPHIC
	printf("vTouchLoadMenu: menu loaded\n");
#endif
}

void vTouchStoreValues(tWidget *pWidget) {
	char* configLoad = loadFromConfig(IP_CONFIG_FILE, "DEFAULT_SET_PAGE");

	vInitDisplay();
	vLoadWebPage(configLoad, xDisplayRoot.entities);
	vPortFree(configLoad);
	vDrawElementsOnDisplay();
	vInitializeSaveButton();
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

void vCheckboxAction(tWidget* pWidget, unsigned long status) {
	basicDisplayLine *root = xDisplayRoot.entities;
	while (root != NULL) {
		if (root->valueWidget == pWidget) {
			root->value = (int) status;
			break;
		}
		root = root->next;
	}
}

void vOpenEditorAction(tWidget *pWidget) {
	basicDisplayLine *root = xDisplayRoot.entities;
	while (root != NULL) {
		if (root->valueWidget == pWidget) {
			printf("vTimeEditorAction: open Editor\n");
			((tPushButtonWidget*) pWidget)->ulStyle &= (~PB_STYLE_PRESSED);
			vOpenEditor(root);
			break;
		}
		root = root->next;
	}
}
