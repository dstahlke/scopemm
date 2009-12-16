#ifndef SCOPEMM_GRIDCANVAS_H
#define SCOPEMM_GRIDCANVAS_H

#include <vector>

#include <gtkmm/drawingarea.h>
#include <gtkmm/window.h>
#include <blitz/array.h>

class GridCanvas : public Gtk::DrawingArea {
public:
	GridCanvas();

	virtual ~GridCanvas();

	void setFlipAxes(bool state=true);

	void setData(blitz::Array<double, 2> data);

	void setData(
		blitz::Array<double, 2> data_r,
		blitz::Array<double, 2> data_g,
		blitz::Array<double, 2> data_b
	);

protected:
	virtual bool on_expose_event(GdkEventExpose* event);

	std::vector<guchar> buf;
	int data_h, data_w;
	bool flip_axes;
};

class MouseCanvas : public GridCanvas {
public:
	MouseCanvas();

	virtual ~MouseCanvas();

	void screenToGrid(double sx, double sy, double *tx, double *ty);

	void gridToScreen(double tx, double ty, double *sx, double *sy);

	virtual void mouse_motion();

	virtual void mouse_clicked(int button);

	virtual bool on_motion_notify_event(GdkEventMotion* event);

	virtual bool on_enter_notify_event(GdkEventCrossing* event);

	virtual bool on_leave_notify_event(GdkEventCrossing* event);

	virtual bool on_button_press_event(GdkEventButton* event);

	bool mouse_in;
	double mouse_x, mouse_y;
	bool button1, button2, button3;

private:
	virtual void updateMouseCoords(int evt_x, int evt_y);
};

#endif // SCOPEMM_GRIDCANVAS_H