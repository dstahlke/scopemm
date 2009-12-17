#include "scopemm-lineplot.h"
#include <boost/foreach.hpp>
#include <algorithm>

/// Plot1D ////////////////////////////////////////

Plot1D::Plot1D() :
	x_auto(true), y_auto(true),
	xmin(0), xmax(1), ymin(0), ymax(1),
	swap_axes(false), 
	draw_x_axis(false), draw_y_axis(false),
	draw_x_grid(false), draw_y_grid(false)
{ }

Plot1D::~Plot1D() {
	// now that our object no longer exists, it must not receive
	// any more events
	BOOST_FOREACH(PlotTrace &t, traces) {
		t.setChangeListener(NULL);
	}
}

PlotTrace Plot1D::addTrace() {
	PlotTrace trace;
	trace.setChangeListener(this);
	traces.push_back(trace);
	fireChangeEvent();
	return trace;
}

void Plot1D::setXAutoRange() {
	x_auto = true;
	recalcAutoRange();
	fireChangeEvent();
}

void Plot1D::setYAutoRange() {
	y_auto = true;
	recalcAutoRange();
	fireChangeEvent();
}

void Plot1D::setXRange(double min, double max) {
	assert(min < max);
	xmin = min;
	xmax = max;
	x_auto = false;
	fireChangeEvent();
}

void Plot1D::setYRange(double min, double max) {
	assert(min < max);
	ymin = min;
	ymax = max;
	y_auto = false;
	fireChangeEvent();
}

void Plot1D::setSwapAxes(bool state) {
	swap_axes = state;
	fireChangeEvent();
}

void Plot1D::setDrawAxes(bool state) {
	draw_x_axis = state;
	draw_y_axis = state;
	fireChangeEvent();
}

void Plot1D::setDrawXAxis(bool state) {
	draw_x_axis = state;
	fireChangeEvent();
}

void Plot1D::setDrawYAxis(bool state) {
	draw_y_axis = state;
	fireChangeEvent();
}

void Plot1D::setDrawGrids(bool state) {
	draw_x_grid = state;
	draw_y_grid = state;
	fireChangeEvent();
}

void Plot1D::setDrawXGrid(bool state) {
	draw_x_grid = state;
	fireChangeEvent();
}

void Plot1D::setDrawYGrid(bool state) {
	draw_y_grid = state;
	fireChangeEvent();
}

void Plot1D::drawStripes(
	const Cairo::RefPtr<Cairo::Context> &cr,
	double from, double to, double step,
	bool horiz
) const {
	if(from > to) std::swap(from, to);

	const int w = get_allocation().get_width();
	const int h = get_allocation().get_height();

	for(double p=from; p<=to; p+=step) {
		double x1, y1, x2, y2;
		coordToScreen(w, h, horiz ? p : xmin, horiz ? ymin : p, x1, y1);
		coordToScreen(w, h, horiz ? p : xmax, horiz ? ymax : p, x2, y2);
		cr->move_to(x1, y1);
		cr->line_to(x2, y2);
	}

	cr->stroke();
}

static double rangeMagnitude(double span) {
	return log(fabs(span) * 1.0) / log(10);
}

void Plot1D::drawGrid(const Cairo::RefPtr<Cairo::Context> &cr) const {
	double grid_fg = 0.7;
	double canvas_bg = 1.0;

	cr->save();
	cr->set_line_width(1);
	cr->set_antialias(Cairo::ANTIALIAS_NONE);

	if(draw_x_grid) {
		double mag = rangeMagnitude(xmax-xmin);
		double frac = ceil(mag) - mag;
		frac = frac*2.0 - 1.0;
		if(frac > 0) {
			double color = grid_fg*frac + canvas_bg*(1.0-frac);
			cr->set_source_rgb(color, color, color);
			double step = pow(10, floor(mag)-1.0);
			drawStripes(cr, step*ceil(xmin/step), xmax, step, true);
		}
	}

	if(draw_y_grid) {
		double mag = rangeMagnitude(ymax-ymin);
		double frac = ceil(mag) - mag;
		frac = frac*2.0 - 1.0;
		if(frac > 0) {
			double color = grid_fg*frac + canvas_bg*(1.0-frac);
			cr->set_source_rgb(color, color, color);
			double step = pow(10, floor(mag)-1.0);
			drawStripes(cr, step*ceil(ymin/step), ymax, step, false);
		}
	}

	if(draw_x_grid) {
		double mag = rangeMagnitude(xmax-xmin);

		cr->set_source_rgb(grid_fg, grid_fg, grid_fg);
		double step = pow(10, floor(mag));
		drawStripes(cr, step*ceil(xmin/step), xmax, step, true);
	}

	if(draw_y_grid) {
		double mag = rangeMagnitude(ymax-ymin);

		cr->set_source_rgb(grid_fg, grid_fg, grid_fg);
		double step = pow(10, floor(mag));
		drawStripes(cr, step*ceil(ymin/step), ymax, step, false);
	}

	cr->restore();
}

bool Plot1D::on_expose_event(GdkEventExpose* event) {
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window) {
		const int w = get_allocation().get_width();
		const int h = get_allocation().get_height();

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

		if(draw_x_grid || draw_y_grid) {
			drawGrid(cr);
		}

		if(draw_x_axis || draw_y_axis) {
			cr->save();
			cr->set_line_width(1);
			cr->set_source_rgb(0.3, 0.3, 0.3);
			cr->set_antialias(Cairo::ANTIALIAS_NONE);
			if(draw_x_axis) {
				double x1, y1, x2, y2;
				coordToScreen(w, h, 0, ymin, x1, y1);
				coordToScreen(w, h, 0, ymax, x2, y2);
				cr->move_to(x1, y1);
				cr->line_to(x2, y2);
				cr->stroke();
			}
			if(draw_y_axis) {
				double x1, y1, x2, y2;
				coordToScreen(w, h, xmin, 0, x1, y1);
				coordToScreen(w, h, xmax, 0, x2, y2);
				cr->move_to(x1, y1);
				cr->line_to(x2, y2);
				cr->stroke();
			}
			cr->restore();
		}

		BOOST_FOREACH(PlotTrace &t, traces) {
			t.draw(this, cr);
		}
	}
	return true;
}

const PlotTrace &Plot1D::trace(int idx) const {
	return traces[idx];
}

PlotTrace &Plot1D::trace(int idx) {
	return traces[idx];
}

void Plot1D::fireChangeEvent() {
	recalcAutoRange();
	queue_draw();
}

void Plot1D::recalcAutoRange() {
	if(x_auto) {
		bool first = true;
		double min=0, max=0;
		BOOST_FOREACH(PlotTrace &t, traces) {
			if(t.empty()) continue;
			double sub_min = t.getMinX();
			double sub_max = t.getMaxX();
			if(first || sub_min < min) min = sub_min;
			if(first || sub_max > max) max = sub_max;
			first = false;
		}
		double delta = max-min;
		min -= delta * 0.05;
		max += delta * 0.05;
		// if no traces
		if(first) { min = 0; max = 1; }
		if(min == max) { min -= 1; max += 1; }
		xmin = min;
		xmax = max;
	}
	if(y_auto) {
		bool first = true;
		double min=0, max=0;
		BOOST_FOREACH(PlotTrace &t, traces) {
			if(t.empty()) continue;
			double sub_min = t.getMinY();
			double sub_max = t.getMaxY();
			if(first || sub_min < min) min = sub_min;
			if(first || sub_max > max) max = sub_max;
			first = false;
		}
		double delta = max-min;
		min -= delta * 0.05;
		max += delta * 0.05;
		// if no traces
		if(first) { min = 0; max = 1; }
		if(min == max) { min -= 1; max += 1; }
		ymin = min;
		ymax = max;
	}
}

/// PlotTrace ////////////////////////////////////////

PlotTraceImpl::PlotTraceImpl() :
	parent(NULL)
{ }

PlotTrace::PlotTrace() :
	impl(new PlotTraceImpl())
{ 
	setColor(1, 0, 0);
}

PlotTrace& PlotTrace::setColor(double r, double g, double b) {
	impl->rgb[0] = r;
	impl->rgb[1] = g;
	impl->rgb[2] = b;
	fireChangeEvent();
	return *this;
}

void PlotTrace::draw(Plot1D *parent, Cairo::RefPtr<Cairo::Context> cr) {
	cr->save();
	cr->set_line_width(1);
	cr->set_source_rgb(impl->rgb[0], impl->rgb[1], impl->rgb[2]);

	const int w = parent->get_allocation().get_width();
	const int h = parent->get_allocation().get_height();

	const double xmin = parent->getXMin();
	const double xmax = parent->getXMax();
	const double ymin = parent->getYMin();
	const double ymax = parent->getYMax();
	const bool swap_axes = parent->getSwapAxes();

	size_t npts = impl->xpts.size();
	//std::cout << "npts=" << npts << std::endl;
	for(size_t i=0; i<npts; ++i) {
		double x = (impl->xpts[i]-xmin)/(xmax-xmin);
		double y = (impl->ypts[i]-ymin)/(ymax-ymin);
		if(swap_axes) std::swap(x, y);
		y = 1.0-y;
		x *= (w-1);
		y *= (h-1);
		//if(i<5) {
		//	std::cout << "xpts=" << impl->xpts[i] << std::endl;
		//	std::cout << "ypts=" << impl->ypts[i] << std::endl;
		//	std::cout << "xy=" << x << "," << y << std::endl;
		//}
		if(!i) {
			cr->move_to(x, y);
		} else {
			cr->line_to(x, y);
		}
	}
	cr->stroke();
	cr->restore();
}

double PlotTrace::getMinX() { 
	assert(!empty());
	return *std::min_element(impl->xpts.begin(), impl->xpts.end()); 
}

double PlotTrace::getMaxX() { 
	assert(!empty());
	return *std::max_element(impl->xpts.begin(), impl->xpts.end()); 
}

double PlotTrace::getMinY() { 
	assert(!empty());
	return *std::min_element(impl->ypts.begin(), impl->ypts.end()); 
}

double PlotTrace::getMaxY() { 
	assert(!empty());
	return *std::max_element(impl->ypts.begin(), impl->ypts.end()); 
}
