#ifndef SCOPEMM_MOUSEADAPTER_H
#define SCOPEMM_MOUSEADAPTER_H

#include <gtkmm/main.h>
#include <gtkmm/widget.h>

namespace scopemm {

class MouseAdapter {
public:
	MouseAdapter();

	~MouseAdapter() { }

	void attach(Gtk::Widget *w);

	void mouse_motion();
	void mouse_clicked(int button);

	sigc::signal<void, int> signal_clicked;
	sigc::signal<void> signal_motion;

private:
	void updateState(gdouble evt_x, gdouble evt_y, guint evt_state);
	bool on_motion_notify_event(GdkEventMotion* event);
	bool on_enter_notify_event(GdkEventCrossing* event);
	bool on_leave_notify_event(GdkEventCrossing* event);
	bool on_button_press_event(GdkEventButton* event);

public:
	bool mouse_in;
	double mouse_x, mouse_y;
	int button_state;
};

} // namespace scopemm

#endif // SCOPEMM_MOUSEADAPTER_H
