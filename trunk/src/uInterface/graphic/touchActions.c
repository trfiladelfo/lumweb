/*
 * touchActions.c
 *
 *  Created on: 18.01.2010
 *      Author: root
 */

#include "graphic/graphicLib.h"
#include "graphic/graphicSettings.h"
#include "graphic/touchActions.h"
#include "graphic/graphicClientBuilder.h"
#include "graphic/graphicWebClient.h"

void increase(tWidget *pWidget) {
	xClientEntity * akt = xClientRoot;

	int divisor = 1;
	char i;

	while (akt != 0) {
		if (akt->increase == (tPushButtonWidget*) pWidget) {
			break;
		}
		akt = akt->next;
	}
	if (akt->increase == (tPushButtonWidget*) pWidget) {
		akt->value += akt->increment;

		if (akt->status & GWC_VALUE_MAX && akt->value > akt->maxValue) {
			akt->value = akt->minValue;
		}

		if (!akt->stringValue) {
			akt->stringValue = (char*) pvPortMalloc((GWC_ROW_VALUE_MAX_LENGTH
					+ 1) * sizeof(char));
		}

		for (i = 0; i < akt->decimal; i++) {
			divisor *= 10;
		}

		if (divisor > 1) {
			snprintf(akt->stringValue, GWC_ROW_VALUE_MAX_LENGTH + 1, "%d,%d",
					akt->value / divisor, akt->value % divisor);
		} else {
			snprintf(akt->stringValue, GWC_ROW_VALUE_MAX_LENGTH + 1, "%d",
					akt->value);
		}
		akt->valueLabel->pcText = akt->stringValue;

		WidgetPaint((tWidget*) akt->valueLabel);
	}
}

void decrease(tWidget *pWidget) {
	xClientEntity * akt = xClientRoot;
	int divisor = 1;
	char i;
	while (akt != 0) {
		if (akt->decrease == (tPushButtonWidget*) pWidget) {
			break;
		}
		akt = akt->next;
	}
	if (akt->decrease == (tPushButtonWidget*) pWidget) {
		akt->value -= akt->increment;

		if (akt->status & GWC_VALUE_MIN && akt->value < akt->minValue) {
			akt->value = akt->maxValue;
		}

		if (!akt->stringValue) {
			akt->stringValue = (char*) pvPortMalloc((GWC_ROW_VALUE_MAX_LENGTH
					+ 1) * sizeof(char));
		}
		for (i = 0; i < akt->decimal; i++) {
			divisor *= 10;
		}

		if (divisor > 1) {
			snprintf(akt->stringValue, GWC_ROW_VALUE_MAX_LENGTH + 1, "%d,%d",
					akt->value / divisor, akt->value % divisor);
		} else {
			snprintf(akt->stringValue, GWC_ROW_VALUE_MAX_LENGTH + 1, "%d",
					akt->value);
		}
		akt->valueLabel->pcText = akt->stringValue;

		WidgetPaint((tWidget*) akt->valueLabel);
	}
}

void backPage(tWidget *pWidget) {
	aktPage--;
	printf("vorherige Seite aufgerufen\n");
	vDrawClientEntity();
}

void continuePage(tWidget *pWidget) {
	aktPage++;
	printf("naechste Seite aufgerufen\n");
	vDrawClientEntity();
}

void onCheckboxClick(tWidget *pWidget, unsigned long bSelected) {
	xClientEntity * akt = xClientRoot;
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
	printf("Change Checkboxvalue");
}

void vLoadNewPage(tWidget *pWidget) {
	xClientEntity * akt = xClientRoot;

	while (akt != 0) {
		if (akt->actionButton == (tPushButtonWidget*) pWidget) {
			break;
		}
		akt = akt->next;
	}
	printf("load page %s\n", akt->stringValue);
	vLoadPage(akt->stringValue);
}
