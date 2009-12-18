#ifndef SCOPEMM_MOUSEADAPTER_H
#define SCOPEMM_MOUSEADAPTER_H

#include "scopemm-base.h"

class MouseAdapter {
public:
	MouseAdapter(PlotBase *_plot);

	virtual void mouse_motion();

	virtual void mouse_clicked(int button);

	bool mouse_in;
	double mouse_x, mouse_y;
	bool button1, button2, button3;

private:
	bool on_motion_notify_event(GdkEventMotion* event);
	bool on_enter_notify_event(GdkEventCrossing* event);
	bool on_leave_notify_event(GdkEventCrossing* event);
	bool on_button_press_event(GdkEventButton* event);
	virtual void updateMouseCoords(int evt_x, int evt_y);

private:
	PlotBase *plot;
};

#endif // SCOPEMM_MOUSEADAPTER_H
