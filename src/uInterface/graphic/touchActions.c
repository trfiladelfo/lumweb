/**
 * \addtogroup Graphic
 * @{
 *
 * \file touchAction.c
 * \author Anziner, Hahn
 * \brief
 *
 */

#include "setup.h"
#include "graphic/graphicLib.h"
#include "graphic/graphicSettings.h"
#include "graphic/touchActions.h"
#include "graphic/graphicClientBuilder.h"
#include "graphic/graphicWebClient.h"

void increase(tWidget *pWidget) {
	xClientEntity * akt = getClientRoot();

	char i;

	while (akt != 0) {
		if (akt->increase == (tPushButtonWidget*) pWidget) {
			break;
		}
		akt = akt->next;
	}
	if (akt->increase == (tPushButtonWidget*) pWidget) {

		akt->value += akt->increment;

		if ((akt->status & GWC_VALUE_MAX) == GWC_VALUE_MAX && akt->value
				> akt->maxValue) {
			akt->value = akt->minValue;
		}

		if (!akt->stringValue) {
			akt->stringValue = (char*) pvPortMalloc((GWC_ROW_VALUE_MAX_LENGTH
					+ 1) * sizeof(char));
		}

		/*if (akt->decimal == 0) {
			snprintf(akt->stringValue, GWC_ROW_VALUE_MAX_LENGTH + 1, "%d",
					akt->value / 10);
		} else {
			snprintf(akt->stringValue, GWC_ROW_VALUE_MAX_LENGTH + 1, "%d,%d",
					akt->value / 10, akt->value % 10);
		}*/
		snprintf(akt->stringValue, GWC_ROW_VALUE_MAX_LENGTH + 1, "%d",	akt->value);

		akt->valueLabel->pcText = akt->stringValue;

		WidgetPaint((tWidget*) akt->valueLabel);
	}
}

void decrease(tWidget *pWidget) {
	xClientEntity * akt = getClientRoot();
	char i;
	while (akt != 0) {
		if (akt->decrease == (tPushButtonWidget*) pWidget) {
			break;
		}
		akt = akt->next;
	}
	if (akt->decrease == (tPushButtonWidget*) pWidget) {
		akt->value -= akt->increment;

		if ((akt->status & GWC_VALUE_MIN) == GWC_VALUE_MIN && akt->value
				< akt->minValue) {
			akt->value = akt->maxValue;
		}

		if (!akt->stringValue) {
			akt->stringValue = (char*) pvPortMalloc((GWC_ROW_VALUE_MAX_LENGTH
					+ 1) * sizeof(char));
		}

		/*if (akt->decimal == 0) {
			snprintf(akt->stringValue, GWC_ROW_VALUE_MAX_LENGTH + 1, "%d",
					akt->value / 10);
		} else {
			snprintf(akt->stringValue, GWC_ROW_VALUE_MAX_LENGTH + 1, "%d,%d",
					akt->value / 10, akt->value % 10);
		}*/
		snprintf(akt->stringValue, GWC_ROW_VALUE_MAX_LENGTH + 1, "%d",	akt->value);

		akt->valueLabel->pcText = akt->stringValue;

		WidgetPaint((tWidget*) akt->valueLabel);
	}
}

void backPage(tWidget *pWidget) {
	aktPage--;
#ifdef DEBUG_GRAPHIC
	printf("vorherige Seite aufgerufen\n");
#endif
	vDrawClientEntity();
}

void continuePage(tWidget *pWidget) {
	aktPage++;
#ifdef DEBUG_GRAPHIC
	printf("naechste Seite aufgerufen\n");
#endif
	vDrawClientEntity();
}

void onCheckboxClick(tWidget *pWidget, unsigned long bSelected) {
	xClientEntity * akt = getClientRoot();
	while (akt != 0) {
		if (akt->checkbox == (tCheckBoxWidget*) pWidget) {
			break;
		}
		akt = akt->next;
	}
	if (akt->checkbox == (tCheckBoxWidget*) pWidget) {
		akt->value = bSelected;
		if (bSelected) {
			akt->checkbox->usStyle |= CB_STYLE_SELECTED;
		} else {
			akt->checkbox->usStyle &= ~CB_STYLE_SELECTED;
		}
		WidgetPaint((tWidget*) akt->checkbox);
	}
}

void vLoadNewPage(tWidget *pWidget) {
	xClientEntity * akt = getClientRoot();

	while (akt != 0) {
		if (akt->actionButton == (tPushButtonWidget*) pWidget) {
			break;
		}
		akt = akt->next;
	}
#ifdef DEBUG_GRAPHIC
	printf("load page %s\n", akt->stringValue);
#endif
	vLoadPage(akt->stringValue);
}
