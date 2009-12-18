#include <iostream>

#include "scopemm-mouseadapter.h"

MouseAdapter::MouseAdapter(
	PlotBase *_plot
) : 
	plot(_plot)
{
	plot->set_events(
		plot->get_events() | 
		Gdk::POINTER_MOTION_MASK |
		Gdk::POINTER_MOTION_HINT_MASK |
		Gdk::ENTER_NOTIFY_MASK |
		Gdk::LEAVE_NOTIFY_MASK |
		Gdk::BUTTON_PRESS_MASK |
		Gdk::BUTTON_RELEASE_MASK
	);

	plot->signal_motion_notify_event().connect(sigc::mem_fun(
		*this, &MouseAdapter::on_motion_notify_event));
	plot->signal_enter_notify_event().connect(sigc::mem_fun(
		*this, &MouseAdapter::on_enter_notify_event));
	plot->signal_leave_notify_event().connect(sigc::mem_fun(
		*this, &MouseAdapter::on_leave_notify_event));
	plot->signal_button_press_event().connect(sigc::mem_fun(
		*this, &MouseAdapter::on_button_press_event));
}

void MouseAdapter::updateMouseCoords(int evt_x, int evt_y) {
	plot->screenToCoord(evt_x, evt_y, mouse_x, mouse_y);
}

bool MouseAdapter::on_motion_notify_event(GdkEventMotion* event) {
	//std::cout << "Motion " << event->state << std::endl;
	updateMouseCoords(event->x, event->y);
	button1 = (event->state & Gdk::BUTTON1_MASK);
	button2 = (event->state & Gdk::BUTTON2_MASK);
	button3 = (event->state & Gdk::BUTTON3_MASK);
	mouse_motion();
	return true;
}

bool MouseAdapter::on_enter_notify_event(GdkEventCrossing* event) {
	//std::cout << "enter_notify" << event->state << std::endl;
	mouse_in = true;
	updateMouseCoords(event->x, event->y);
	button1 = (event->state & Gdk::BUTTON1_MASK);
	button2 = (event->state & Gdk::BUTTON2_MASK);
	button3 = (event->state & Gdk::BUTTON3_MASK);
	mouse_motion();
	return true;
}

bool MouseAdapter::on_leave_notify_event(GdkEventCrossing* event) {
	//std::cout << "leave_notify" << event->state << std::endl;
	mouse_in = false;
	button1 = (event->state & Gdk::BUTTON1_MASK);
	button2 = (event->state & Gdk::BUTTON2_MASK);
	button3 = (event->state & Gdk::BUTTON3_MASK);
	mouse_motion();
	return true;
}

bool MouseAdapter::on_button_press_event(GdkEventButton* event) {
	updateMouseCoords(event->x, event->y);
	//std::cout << "state " << event->state << "," << event->button << std::endl;
	mouse_clicked(event->button);
	return true;
}

void MouseAdapter::mouse_motion() {
//	std::cout
//		<< "mouse: " << mouse_in << "," << mouse_x << "," << mouse_y
//		<< " / " << button1 << "," << button2 << "," << button3
//		<< std::endl;
}

void MouseAdapter::mouse_clicked(int button) {
	std::cout << "click: " << mouse_x << "," << mouse_y << "," << button << std::endl;
}
