#include <algorithm>
#include <vector>

#include "gtk-blitz-gridcanvas.h"

GridCanvas::GridCanvas() : 
	flip_axes(false)
{ }

GridCanvas::~GridCanvas() { }

void GridCanvas::setFlipAxes(bool state) {
	flip_axes = state;
}

void GridCanvas::setData(blitz::Array<double, 2> data) {
	setData(data, data, data);
}

void GridCanvas::setData(
	blitz::Array<double, 2> data_r,
	blitz::Array<double, 2> data_g,
	blitz::Array<double, 2> data_b
) {
	blitz::Array<double, 2> data[3] = { data_r, data_g, data_b };
	const int w = data[0].shape()[flip_axes ? 0 : 1];
	const int h = data[0].shape()[flip_axes ? 1 : 0];
	data_w = w;
	data_h = h;

	if(h>0 && w>0) {
		buf.resize(h*w*3);

		for(int band=0; band<3; band++) {
			const double min = blitz::min(data[band]);
			const double max = blitz::max(data[band]);
			if(flip_axes) {
				for(int y=0; y<data_h; y++) {
					for(int x=0; x<data_w; x++) {
						const int v = int(255.0 * (data[band](x,y)-min) / (max-min));
						buf[(y*data_w + x)*3 + band] = v;
					}
				}
			} else {
				for(int y=0; y<data_h; y++) {
					for(int x=0; x<data_w; x++) {
						const int v = int(255.0 * (data[band](y,x)-min) / (max-min));
						buf[(y*data_w + x)*3 + band] = v;
					}
				}
			}
		}
	}

	queue_draw();
}

bool GridCanvas::on_expose_event(GdkEventExpose* event __attribute__((unused)) ) {
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window && buf.size()) {
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();

		// FIXME - only needed if sizes don't match
		std::vector<guchar> buf2(height*width*3);
		for(int y=0; y<height; y++) {
			const int j = y * (data_h-1) / (height-1);
			for(int x=0; x<width; x++) {
				const int i = x * (data_w-1) / (width-1);
				for(int band=0; band<3; band++) {
					buf2[(y*width + x)*3 + band] =
						buf[(j*data_w + i)*3 + band];
				}
			}
		}

		get_window()->draw_rgb_image(
			get_style()->get_fg_gc(Gtk::STATE_NORMAL),
			0, 0, width, height,
			Gdk::RGB_DITHER_NONE,
			&buf2[0], width*3
		);
	}
	return true;
}

///////////////////////////////////////////////////////////

MouseCanvas::MouseCanvas() {
	set_events(get_events() | Gdk::POINTER_MOTION_MASK);
	set_events(get_events() | Gdk::POINTER_MOTION_HINT_MASK);
	set_events(get_events() | Gdk::ENTER_NOTIFY_MASK);
	set_events(get_events() | Gdk::LEAVE_NOTIFY_MASK);
	set_events(get_events() | Gdk::BUTTON_PRESS_MASK);
	set_events(get_events() | Gdk::BUTTON_RELEASE_MASK);
}

MouseCanvas::~MouseCanvas() { }

void MouseCanvas::screenToGrid(double sx, double sy, double *tx, double *ty) {
	Gtk::Allocation allocation = get_allocation();
	const int aw = allocation.get_width();
	const int ah = allocation.get_height();
	*tx = sx * (data_w-1) / (aw-1);
	*ty = sy * (data_h-1) / (ah-1);
}

void MouseCanvas::gridToScreen(double tx, double ty, double *sx, double *sy) {
	Gtk::Allocation allocation = get_allocation();
	const int aw = allocation.get_width();
	const int ah = allocation.get_height();
	*sx = tx * (aw-1) / (data_w-1);
	*sy = ty * (ah-1) / (data_h-1);
}

void MouseCanvas::updateMouseCoords(int evt_x, int evt_y) {
	screenToGrid(evt_x, evt_y, &mouse_x, &mouse_y);
}

bool MouseCanvas::on_motion_notify_event(GdkEventMotion* event) {
	//std::cout << "Motion " << event->state << std::endl;
	updateMouseCoords(event->x, event->y);
	button1 = (event->state & Gdk::BUTTON1_MASK);
	button2 = (event->state & Gdk::BUTTON2_MASK);
	button3 = (event->state & Gdk::BUTTON3_MASK);
	mouse_motion();
	return true;
}

bool MouseCanvas::on_enter_notify_event(GdkEventCrossing* event) {
	//std::cout << "enter_notify" << event->state << std::endl;
	mouse_in = true;
	updateMouseCoords(event->x, event->y);
	button1 = (event->state & Gdk::BUTTON1_MASK);
	button2 = (event->state & Gdk::BUTTON2_MASK);
	button3 = (event->state & Gdk::BUTTON3_MASK);
	mouse_motion();
	return true;
}

bool MouseCanvas::on_leave_notify_event(GdkEventCrossing* event) {
	//std::cout << "leave_notify" << event->state << std::endl;
	mouse_in = false;
	button1 = (event->state & Gdk::BUTTON1_MASK);
	button2 = (event->state & Gdk::BUTTON2_MASK);
	button3 = (event->state & Gdk::BUTTON3_MASK);
	mouse_motion();
	return true;
}

bool MouseCanvas::on_button_press_event(GdkEventButton* event) {
	updateMouseCoords(event->x, event->y);
	//std::cout << "state " << event->state << "," << event->button << std::endl;
	mouse_clicked(event->button);
	return true;
}

void MouseCanvas::mouse_motion() {
//	std::cout
//		<< "mouse: " << mouse_in << "," << mouse_x << "," << mouse_y
//		<< " / " << button1 << "," << button2 << "," << button3
//		<< std::endl;
}

void MouseCanvas::mouse_clicked(int button) {
	std::cout << "click: " << mouse_x << "," << mouse_y << "," << button << std::endl;
}
