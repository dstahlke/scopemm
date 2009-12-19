#ifndef SCOPEMM_MOUSEADAPTER_H
#define SCOPEMM_MOUSEADAPTER_H

namespace scopemm {

class MouseAdapter {
public:
	MouseAdapter(PlotCanvas *_plot);

	virtual ~MouseAdapter() { }

	virtual void mouse_motion();
	virtual void mouse_clicked(int button);

	bool mouseIn()  const { return mouse_in; }
	double mouseX() const {
		double plot_x, plot_y;
		plot->screenToCoord(mouse_x, mouse_y, plot_x, plot_y);
		return plot_x;
	}
	double mouseY() const {
		double plot_x, plot_y;
		plot->screenToCoord(mouse_x, mouse_y, plot_x, plot_y);
		return plot_y;
	}
	bool button1()  const { return button_state & 1; }
	bool button2()  const { return button_state & 2; }
	bool button3()  const { return button_state & 4; }

	sigc::signal<void, int> signal_clicked;
	sigc::signal<void> signal_motion;

private:
	void updateState(gdouble evt_x, gdouble evt_y, guint evt_state);
	bool on_motion_notify_event(GdkEventMotion* event);
	bool on_enter_notify_event(GdkEventCrossing* event);
	bool on_leave_notify_event(GdkEventCrossing* event);
	bool on_button_press_event(GdkEventButton* event);

	PlotCanvas *plot;
	bool mouse_in;
	double mouse_x, mouse_y;
	int button_state;
};

} // namespace scopemm

#endif // SCOPEMM_MOUSEADAPTER_H
