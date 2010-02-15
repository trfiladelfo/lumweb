/**
 * \addtogroup Graphic
 * @{
 *
 * \file graphicClientBuilder.c
 * \author Anziner, Hahn
 * \brief
 *
 */

#include <string.h>
#include "setup.h"
#include "graphic/graphicWebClient.h"
#include "graphic/graphicLib.h"
#include "graphic/graphicSettings.h"
#include "graphic/graphicClientBuilder.h"
#include "graphic/touchActions.h"

#include "configuration/configloader.h"

#include "ethernet/LWIPStack.h"

#include "lwip/netbuf.h"
#include "lwip/api.h"

#include "utils.h"

struct netconn *conn;
struct netbuf *inBuf;
char *pageData;

void drawClientEntity(void);
void vInsertClientEntity(xClientEntity* toInsert);

int aktPage = 1;
char* aktWebPage = NULL;

xClientEntity *xClientRoot = NULL;

void vLoadMenu(void) {
	char* configLoad = loadFromConfig(IP_CONFIG_FILE, "DEFAULT_MENU_PAGE");
	if (configLoad != NULL) {
		vLoadPage(configLoad);
		vPortFree(configLoad);
		configLoad = NULL;
	} else {
		vLoadPage("");
	}
}

void vAddRow(char* id, char* name, long value, long min, long max, long inc,
		long decimal, char status) {

	xClientEntity *newRow =
			(xClientEntity*) pvPortMalloc(sizeof(xClientEntity));
	newRow->id = id;
	newRow->name = name;
	newRow->value = value;
	newRow->status = status;
	newRow->minValue = min;
	newRow->maxValue = max;
	newRow->increment = inc;
	newRow->decimal = decimal;
	newRow->stringValue = NULL;
	newRow->checkbox = NULL;
	newRow->decrease = NULL;
	newRow->increase = NULL;
	newRow->nameLabel = NULL;
	newRow->actionButton = NULL;
	newRow->valueLabel = NULL;

	newRow->next = NULL;

	vInsertClientEntity(newRow);
}

void vAddActionButton(char* name, char* link, char status) {

	xClientEntity *newRow =
			(xClientEntity*) pvPortMalloc(sizeof(xClientEntity));
	newRow->id = NULL;
	newRow->name = name;
	newRow->value = 0;
	newRow->status = status;
	newRow->minValue = 0;
	newRow->maxValue = 0;
	newRow->increment = 0;
	newRow->decimal = 0;
	newRow->stringValue = link;
	newRow->checkbox = NULL;
	newRow->decrease = NULL;
	newRow->increase = NULL;
	newRow->nameLabel = NULL;
	newRow->actionButton = NULL;
	newRow->valueLabel = NULL;

	newRow->next = NULL;

	vInsertClientEntity(newRow);
}

void vInsertClientEntity(xClientEntity* toInsert) {
	xClientEntity* akt = xClientRoot;
	if (xClientRoot == NULL) {
		xClientRoot = toInsert;
	} else {
		while (akt != NULL && akt->next != NULL) {
			akt = akt->next;
		}
		akt->next = toInsert;
	}
}

void vDrawClientEntity(void) {

	int offset = (aktPage - 1) * GWC_ROWS_PER_VIEW;
	xClientEntity *akt = xClientRoot;

	int aktRow = 0;
	int top = GWC_FIRST_ROW_TOP;
	int i;

	if (offset > 0) {
		for (i = 0; akt->next != NULL && i < offset; i++) {
			akt = akt->next;
		}
	} else if (offset < 0) {
		offset = 0;
	}

	for (i = 0; akt != NULL && i < GWC_ROWS_PER_VIEW; i++) {

		if ((akt->status & GWC_LABEL) == GWC_LABEL) {

			akt->nameLabel = addLabel(GWC_ROW_LABEL_LEFT, top,
					GWC_ROW_LABEL_WIDTH, GWC_ROW_HEIGHT, akt->name);
#ifdef DEBUG_GRAPHIC
	printf("vDrawClientEntity: add Label %s\n", akt->name);
#endif
		}

		if ((akt->status & GWC_NUMERIC) == GWC_NUMERIC) {

			if (!akt->stringValue) {
				akt->stringValue = (char*) pvPortMalloc(
						(GWC_ROW_VALUE_MAX_LENGTH + 1) * sizeof(char));
			}
			akt->decrease = addButton(GWC_ROW_DECREASE_BUTTON_LEFT, top,
					GWC_ROW_DECREASE_BUTTON_WIDTH, GWC_ROW_HEIGHT,
					GWC_ROW_DECREASE_BUTTON_SYMBOL,
					GWC_ROW_DECREASE_AUTOREPEAT, decrease);
#ifdef DEBUG_GRAPHIC
	printf("vDrawClientEntity: add Decrease Button\n");
#endif

			/*if (akt->decimal == 0) {
				snprintf(akt->stringValue, (GWC_ROW_VALUE_MAX_LENGTH + 1),
						"%d", akt->value / 10);
			} else {
				snprintf(akt->stringValue, (GWC_ROW_VALUE_MAX_LENGTH + 1),
						"%d,%d", akt->value / 10, akt->value % 10);
			}*/
			snprintf(akt->stringValue, GWC_ROW_VALUE_MAX_LENGTH, "%d",	akt->value);
			akt->valueLabel = addLabel(GWC_ROW_VALUE_LEFT, top,
					GWC_ROW_VALUE_WIDTH, GWC_ROW_HEIGHT, akt->stringValue);
#ifdef DEBUG_GRAPHIC
	printf("vDrawClientEntity: add Value %s\n", akt->stringValue);
#endif
			akt->increase = addButton(GWC_ROW_INCREASE_BUTTON_LEFT, top,
					GWC_ROW_INCREASE_BUTTON_WIDTH, GWC_ROW_HEIGHT,
					GWC_ROW_INCREASE_BUTTON_SYMBOL,
					GWC_ROW_INCREASE_AUTOREPEAT, increase);
#ifdef DEBUG_GRAPHIC
	printf("vDrawClientEntity: add Increase Button\n");
#endif

		} else if ((akt->status & GWC_BOOLEAN) == GWC_BOOLEAN) {
			akt->checkbox = addCheckbox(GWC_ROW_CHECKBOX_LEFT, top,
					GWC_ROW_CHECKBOX_WIDTH, GWC_ROW_HEIGHT,
					(unsigned long) akt->value, onCheckboxClick);
#ifdef DEBUG_GRAPHIC
	printf("vDrawClientEntity: add Checkbox %d\n", akt->status);
#endif

		} else if ((akt->status & GWC_SUBMIT) == GWC_SUBMIT) {
			akt->actionButton = addButton(GWC_ACTION_BUTTON_LEFT, top,
					GWC_ACTION_BUTTON_WIDTH, GWC_ROW_HEIGHT, akt->name, 0,
					vSendData);
#ifdef DEBUG_GRAPHIC
	printf("vDrawClientEntity: add Submit %s\n", akt->name);
#endif

		} else if ((akt->status & GWC_HYPERLINK) == GWC_HYPERLINK) {
			akt->actionButton = addButton(GWC_ACTION_BUTTON_LEFT, top,
					GWC_ACTION_BUTTON_WIDTH, GWC_ROW_HEIGHT, akt->name, 0,
					vLoadNewPage);
#ifdef DEBUG_GRAPHIC
	printf("vDrawClientEntity: add Hyperlink %s\n", akt->name);
#endif
		}

		akt = akt->next;
		top += GWC_ROW_HEIGHT + GWC_ROW_MARGIN_TOP;
	}

	if (offset > 0) {
#ifdef DEBUG_GRAPHIC
	printf("vDrawClientEntity: add Back\n");
#endif
		addButton(5, 205, 80, 30, "Back", 0, backPage);
	}

	if (akt != NULL) {
#ifdef DEBUG_GRAPHIC
	printf("vDrawClientEntity: add Continue\n");
#endif
		addButton(235, 205, 80, 30, "Continue", 0, continuePage);
	}

	vDrawPanel();
#ifdef DEBUG_GRAPHIC
	printf("vDrawClientEntity: Output erfolgreich\n");
#endif
}

void vDestroyClientEntities(void) {
	xClientEntity *akt = xClientRoot;
	xClientEntity *toDelete = NULL;
#ifdef DEBUG_GRAPHIC
	printf("vDestroyClientEntities start ...\n");
#endif
	vCleanDisplay();
#ifdef DEBUG_GRAPHIC
	printf("vDestroyClientEntities destroy structures ...\n");
#endif
	while (akt != NULL) {
		if (akt->id != NULL) {
#ifdef DEBUG_GRAPHIC
			printf("vDestroyClientEntities free id\n");
#endif
			vPortFree(akt->id);
			akt->id = NULL;
		}

		if (akt->name != NULL) {
#ifdef DEBUG_GRAPHIC
			printf("vDestroyClientEntities free name\n");
#endif
			vPortFree(akt->name);
			akt->name = NULL;
		}

		if (akt->stringValue != NULL) {
#ifdef DEBUG_GRAPHIC
			printf("vDestroyClientEntities free stringValue\n");
#endif
			vPortFree(akt->stringValue);
			akt->stringValue = NULL;
		}

		toDelete = akt;
		akt = akt->next;
#ifdef DEBUG_GRAPHIC
		printf("vDestroyClientEntities free structure\n");
#endif
		vPortFree(toDelete);
		toDelete = NULL;
	}
#ifdef DEBUG_GRAPHIC
	printf("vDestroyClientEntities Entities destroyed\n");
#endif
	xClientRoot = NULL;
}

xClientEntity *getClientRoot(void) {
	return xClientRoot;
}
