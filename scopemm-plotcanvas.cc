#include <boost/foreach.hpp>
#include <algorithm>

#include "scopemm.h"

using namespace scopemm;

/// PlotCanvas ////////////////////////////////////////

PlotCanvas::PlotCanvas() :
	grid_layer(new GridLayer()),
	x_auto(true), y_auto(true),
	draw_x_axis(false), draw_y_axis(false),
	xmin(0), xmax(1), ymin(0), ymax(1), swap_axes(false)
{ }

PlotCanvas::~PlotCanvas() {
	// now that our object no longer exists, it must not receive
	// any more events
	BOOST_FOREACH(const PlotLayerImplPtr &layer, layers) {
		layer->change_listeners.erase(this);
	}
}

PlotCanvas &PlotCanvas::addTrace(PlotLayerBase &layer) {
	layer.impl_base->change_listeners.insert(this);
	layers.insert(layer.impl_base);
	fireChangeEvent();
	return *this;
}

PlotCanvas &PlotCanvas::removeTrace(PlotLayerBase &layer) {
	layer.impl_base->change_listeners.erase(this);
	layers.erase(layer.impl_base);
	fireChangeEvent();
	return *this;
}

void PlotCanvas::setXAutoRange() {
	x_auto = true;
	recalcAutoRange();
	fireChangeEvent();
}

void PlotCanvas::setYAutoRange() {
	y_auto = true;
	recalcAutoRange();
	fireChangeEvent();
}

void PlotCanvas::setXRange(double min, double max) {
	assert(min < max);
	xmin = min;
	xmax = max;
	x_auto = false;
	fireChangeEvent();
}

void PlotCanvas::setYRange(double min, double max) {
	assert(min < max);
	ymin = min;
	ymax = max;
	y_auto = false;
	fireChangeEvent();
}

void PlotCanvas::setSwapAxes(bool state) {
	swap_axes = state;
	fireChangeEvent();
}

void PlotCanvas::setDrawAxes(bool state) {
	draw_x_axis = state;
	draw_y_axis = state;
	fireChangeEvent();
}

void PlotCanvas::setDrawXAxis(bool state) {
	draw_x_axis = state;
	fireChangeEvent();
}

void PlotCanvas::setDrawYAxis(bool state) {
	draw_y_axis = state;
	fireChangeEvent();
}

void PlotCanvas::setDrawGrids(bool state) {
	if(state) {
		addTrace(*grid_layer);
	} else {
		removeTrace(*grid_layer);
	}
	fireChangeEvent();
}

bool PlotCanvas::on_expose_event(GdkEventExpose* event) {
	Glib::RefPtr<Gdk::Window> window = get_window();

	if(!window) return true;

	screen_w = get_allocation().get_width();
	screen_h = get_allocation().get_height();

	Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
	if(event) {
		cr->rectangle(event->area.x, event->area.y,
			event->area.width, event->area.height);
		cr->clip();
	}

	cr->save();
	cr->set_source_rgb(1, 1, 1);
	cr->paint();
	cr->restore();

	if(draw_x_axis || draw_y_axis) {
		cr->save();
		cr->set_line_width(1);
		cr->set_source_rgb(0.3, 0.3, 0.3);
		cr->set_antialias(Cairo::ANTIALIAS_NONE);
		if(draw_x_axis) {
			double x1, y1, x2, y2;
			coordToScreen(0, ymin, x1, y1);
			coordToScreen(0, ymax, x2, y2);
			cr->move_to(x1, y1);
			cr->line_to(x2, y2);
			cr->stroke();
		}
		if(draw_y_axis) {
			double x1, y1, x2, y2;
			coordToScreen(xmin, 0, x1, y1);
			coordToScreen(xmax, 0, x2, y2);
			cr->move_to(x1, y1);
			cr->line_to(x2, y2);
			cr->stroke();
		}
		cr->restore();
	}

	BOOST_FOREACH(const PlotLayerImplPtr &layer, layers) {
		layer->draw(this, cr);
	}

	return true;
}

void PlotCanvas::fireChangeEvent() {
	recalcAutoRange();
	queue_draw();
}

void PlotCanvas::recalcAutoRange() {
	if(x_auto) {
		bool first = true;
		double min=0, max=0;
		BOOST_FOREACH(const PlotLayerImplPtr &layer, layers) {
			if(!layer->hasMinMax()) continue;
			double sub_min = layer->getMinX();
			double sub_max = layer->getMaxX();
			if(first || sub_min < min) min = sub_min;
			if(first || sub_max > max) max = sub_max;
			first = false;
		}
		double delta = max-min;
		min -= delta * 0.05;
		max += delta * 0.05;
		// if no layers had min/max
		if(first) { min = 0; max = 1; }
		if(min == max) { min -= 1; max += 1; }
		xmin = min;
		xmax = max;
	}
	if(y_auto) {
		bool first = true;
		double min=0, max=0;
		BOOST_FOREACH(const PlotLayerImplPtr &layer, layers) {
			if(!layer->hasMinMax()) continue;
			double sub_min = layer->getMinY();
			double sub_max = layer->getMaxY();
			if(first || sub_min < min) min = sub_min;
			if(first || sub_max > max) max = sub_max;
			first = false;
		}
		double delta = max-min;
		min -= delta * 0.05;
		max += delta * 0.05;
		// if no layers had min/max
		if(first) { min = 0; max = 1; }
		if(min == max) { min -= 1; max += 1; }
		ymin = min;
		ymax = max;
	}
}

/// PlotLayerBase ////////////////////////////////////

void PlotLayerBase::fireChangeEvent() {
	BOOST_FOREACH(PlotCanvas *l, impl_base->change_listeners) {
		l->fireChangeEvent();
	}
}
