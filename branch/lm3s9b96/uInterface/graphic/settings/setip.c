/**
 * Menuitem for setting the IP-Address
 */

#include "../graphicTask.h"
#include "hw_types.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/checkbox.h"
#include "grlib/container.h"
#include "grlib/pushbutton.h"
#include "grlib/radiobutton.h"
#include "grlib/slider.h"
#include "graphic/settings/settings.h"
#include "kitronix320x240x16_ssd2119_8bit.h"
#include "lwip/ip_addr.h"

// IP Parsen ((addr.addr) & 0xFF), ((addr.addr >> 8) & 0xFF), ((addr.addr >> 16) & 0xFF), ((addr.addr >> 24) & 0xFF)
// IP Setzten netif_set_ipaddr(struct netif *netif, struct ip_addr *ipaddr)
long ip[4];
void updateIp1(tWidget *widget, long lValue);
void updateIp2(tWidget *widget, long lValue);
void updateIp3(tWidget *widget, long lValue);
void updateIp4(tWidget *widget, long lValue);
void updateIP(long lValue, int pos);

struct ip_addr addr;

extern tCanvasWidget g_psSliderPanels[];
Canvas(g_sIntroduction, g_psSliderPanels, 0, 0, &g_sKitronix320x240x16_SSD2119, 0, 24,
		320, 166, CANVAS_STYLE_APP_DRAWN, 0, 0, 0, 0, 0, 0, 0);

tSliderWidget
		g_psSliders[] = {

								SliderStruct( g_psSliderPanels, g_psSliders + 1, 0, &g_sKitronix320x240x16_SSD2119,
										20, 70, 26, 110, 0, 255, 192, (SL_STYLE_FILL | SL_STYLE_BACKG_FILL
												| SL_STYLE_VERTICAL | SL_STYLE_OUTLINE | SL_STYLE_LOCKED),
										ClrDarkGreen, ClrDarkGray, ClrSilver, 0, 0, 0, 0, 0, 0, updateIp1),

								SliderStruct( g_psSliderPanels, g_psSliders + 2, 0, &g_sKitronix320x240x16_SSD2119,
										90, 70, 26, 110, 0, 255, 168, (SL_STYLE_FILL | SL_STYLE_BACKG_FILL
												| SL_STYLE_VERTICAL | SL_STYLE_OUTLINE | SL_STYLE_LOCKED),
										ClrDarkGreen, ClrDarkGray, ClrSilver, 0, 0, 0, 0, 0, 0, updateIp2),

								SliderStruct( g_psSliderPanels, g_psSliders + 3, 0, &g_sKitronix320x240x16_SSD2119,
										160, 70, 26, 110, 0, 255, 0, (SL_STYLE_FILL | SL_STYLE_BACKG_FILL
												| SL_STYLE_VERTICAL | SL_STYLE_OUTLINE | SL_STYLE_LOCKED),
										ClrDarkGreen, ClrDarkGray, ClrSilver, 0, 0, 0, 0, 0, 0, updateIp3),

								SliderStruct( g_psSliderPanels, 0, 0, &g_sKitronix320x240x16_SSD2119, 240, 70, 26,
										230, 0, 255, 100, (SL_STYLE_FILL | SL_STYLE_BACKG_FILL
												| SL_STYLE_VERTICAL | SL_STYLE_OUTLINE | SL_STYLE_LOCKED),
										ClrDarkGreen, ClrDarkGray, ClrSilver, 0, 0, 0, 0, 0, 0, updateIp4)

		};

tCanvasWidget g_psSliderPanels[] = {

CanvasStruct(0, 0, &g_sIntroduction,
		&g_sKitronix320x240x16_SSD2119, 0, 30, 320, 166, CANVAS_STYLE_FILL,
		ClrBlack, 0, 0, 0, 0, 0, 0)

};

void setIP(tWidget *pWidget) {

	//
	// Remove the current panel.
	//
	WidgetRemove(pWidget);

	//
	// Add and draw the new panel.
	//
	WidgetAdd(WIDGET_ROOT, (tWidget *) (&g_psSliderPanels));

	//
	// Issue the initial paint request to the widgets.
	//
	WidgetPaint(WIDGET_ROOT);
	printf("Setze IP aufgerufen\n");

}

void setGW(tWidget *pWidget) {
	//
	// Remove the current panel.
	//
	WidgetRemove(pWidget);

	//
	// Add and draw the new panel.
	//
	WidgetAdd(WIDGET_ROOT, (tWidget *) (&g_psSliderPanels));

	//
	// Issue the initial paint request to the widgets.
	//
	WidgetPaint(WIDGET_ROOT);
	printf("Setze GW aufgerufen\n");
}

void setNETMASK(tWidget *pWidget) {
	//
	// Remove the current panel.
	//
	WidgetRemove(pWidget);

	//
	// Add and draw the new panel.
	//
	WidgetAdd(WIDGET_ROOT, (tWidget *) (&g_psSliderPanels));

	//
	// Issue the initial paint request to the widgets.
	//
	WidgetPaint(WIDGET_ROOT);
	printf("Setze NETMASK aufgerufen\n");
}

void SetIPAddress(void* target(void)) {
	target();
}

void updateIp1(tWidget *widget, long lValue) {
	updateIP(lValue, 0);
}
void updateIp2(tWidget *widget, long lValue) {
	updateIP(lValue, 1);
}
void updateIp3(tWidget *widget, long lValue) {
	updateIP(lValue, 2);
}
void updateIp4(tWidget *widget, long lValue) {
	updateIP(lValue, 3);
}

void updateIP(long lValue, int pos) {
	if (pos >= 0 && pos < 4) {
		ip[pos] = lValue;
		static char pcSliderText[5];

		sprintf(pcSliderText, "%3d", lValue);
		SliderTextSet(&g_psSliders[pos], pcSliderText);
		WidgetPaint((tWidget *) &g_psSliders[pos]);
	}
}
