/*
 * touchActions.c
 *
 *  Created on: 18.01.2010
 *      Author: root
 */

#include "graphic/graphicLib.h"
#include "graphic/graphicSettings.h"
#include "graphic/touchActions.h"
#include "graphic/graphicWebClient.h"

void increase(tWidget *pWidget) {
	gwcRow * akt = pgwcRoot;
	while (akt != 0) {
		if (akt->increase == (tPushButtonWidget*) pWidget) {
			break;
		}
		akt = akt->next;
	}
	if (akt->increase == (tPushButtonWidget*) pWidget) {
		akt->value++;

		if (!akt->stringValue) {
			akt->stringValue = (char*) pvPortMalloc((GWC_ROW_VALUE_MAX_LENGTH
					+ 1) * sizeof(char));
		}
		snprintf(akt->stringValue, GWC_ROW_VALUE_MAX_LENGTH + 1, "%d",
				akt->value);
		akt->valueLabel->pcText = akt->stringValue;

		WidgetPaint((tWidget*) akt->valueLabel);
	}
}

void decrease(tWidget *pWidget) {
	gwcRow * akt = pgwcRoot;
	while (akt != 0) {
		if (akt->decrease == (tPushButtonWidget*) pWidget) {
			break;
		}
		akt = akt->next;
	}
	if (akt->decrease == (tPushButtonWidget*) pWidget) {
		akt->value--;

		if (!akt->stringValue) {
			akt->stringValue = (char*) pvPortMalloc((GWC_ROW_VALUE_MAX_LENGTH
					+ 1) * sizeof(char));
		}
		snprintf(akt->stringValue, (GWC_ROW_VALUE_MAX_LENGTH + 1), "%d",
				akt->value);
		akt->valueLabel->pcText = akt->stringValue;

		WidgetPaint((tWidget*) akt->valueLabel);
	}
}

void backPage(tWidget *pWidget) {
	aktPage--;
	printf("vorherige Seite aufgerufen\n");
	drawGWC((aktPage - 1) * GWC_ROWS_PER_VIEW);
}

void continuePage(tWidget *pWidget) {
	aktPage++;
	printf("naechste Seite aufgerufen\n");
	drawGWC((aktPage - 1) * GWC_ROWS_PER_VIEW);
}

void onCheckboxClick(tWidget *pWidget, unsigned long bSelected) {
	gwcRow * akt = pgwcRoot;
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

void loadWeb(tWidget *pWidget) {
	loadPage("times_p.ssi");
}
