#include "gtk-blitz-lineplot.hpp"
#include <boost/foreach.hpp>
#include <iostream> // FIXME

/// Plot1D ////////////////////////////////////////

Plot1D::Plot1D() :
	x_auto(true), y_auto(true),
	xmin(0), xmax(1), ymin(0), ymax(1)
{ }

PlotTracePtr Plot1D::addTrace() {
	PlotTracePtr trace(new PlotTrace(this));
	trace->setSelfRef(trace);
	traces.push_back(trace);
	return trace;
}

void Plot1D::setXAutoRange() {
	x_auto = true;
	recalcAutoRange();
}

void Plot1D::setYAutoRange() {
	y_auto = true;
	recalcAutoRange();
}

void Plot1D::setXRange(double min, double max) {
	xmin = min;
	xmax = max;
	x_auto = false;
	BOOST_FOREACH(PlotTracePtr t, traces) {
		t->setXRange(xmin, xmax);
	}
}

void Plot1D::setYRange(double min, double max) {
	ymin = min;
	ymax = max;
	y_auto = false;
	BOOST_FOREACH(PlotTracePtr t, traces) {
		t->setYRange(ymin, ymax);
	}
}

bool Plot1D::on_expose_event(GdkEventExpose* event) {
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window) {
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
		cr->rectangle(event->area.x, event->area.y,
			event->area.width, event->area.height);
		cr->clip();
		BOOST_FOREACH(PlotTracePtr t, traces) {
			t->draw(cr);
		}
	}
	return true;
}

void Plot1D::dataChanged() {
	recalcAutoRange();
	queue_draw();
}

void Plot1D::recalcAutoRange() {
// FIXME
//	if(x_auto) {
//		setXRange(
//			*std::min_element(xpts.begin(), xpts.end()),
//			*std::max_element(xpts.begin(), xpts.end())
//		);
//	}
//	if(y_auto) {
//		setYRange(
//			*std::min_element(ypts.begin(), ypts.end()),
//			*std::max_element(ypts.begin(), ypts.end())
//		);
//	}
}

/// PlotTrace ////////////////////////////////////////

PlotTrace::PlotTrace(Plot1D *_parent) : 
	parent(_parent)
{ 
	setColor(1, 0, 0);
	setXRange(parent->xmin, parent->xmax);
	setYRange(parent->ymin, parent->ymax);
}

PlotTrace::~PlotTrace() {
	std::cout << "PlotTrace dtor" << std::endl;
}

void PlotTrace::setSelfRef(PlotTracePtr ref) {
	selfref = ref;
}

PlotTracePtr PlotTrace::setXRange(double min, double max) {
	xmin = min;
	xmax = max;
	return selfref.lock();
}

PlotTracePtr PlotTrace::setYRange(double min, double max) {
	ymin = min;
	ymax = max;
	return selfref.lock();
}

PlotTracePtr PlotTrace::setColor(double r, double g, double b) {
	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
	return selfref.lock();
}

void PlotTrace::dataChanged() {
	parent->dataChanged();
}

PlotTracePtr PlotTrace::setYData(double *_ypts, size_t _npts) {
	return setYData(_ypts, _ypts+_npts);
}

PlotTracePtr PlotTrace::setXYData(double *_xpts, double *_ypts, size_t _npts) {
	if(_xpts) {
		return setXYData(_xpts, _xpts+_npts, _ypts, _ypts+_npts);
	} else {
		return setYData(_ypts, _ypts+_npts);
	}
}

void PlotTrace::draw(Cairo::RefPtr<Cairo::Context> cr) {
	cr->set_line_width(1);
	cr->set_source_rgb(rgb[0], rgb[1], rgb[2]);

	const int w = parent->get_allocation().get_width();
	const int h = parent->get_allocation().get_height();

	size_t npts = xpts.size();
	//std::cout << "npts=" << npts << std::endl;
	for(size_t i=0; i<npts; ++i) {
		double x = (xpts[i]-xmin)/(xmax-xmin)*(w-1);
		double y = (ymax-ypts[i])/(ymax-ymin)*(h-1);
		//if(i<5) {
		//	std::cout << "xpts=" << xpts[i] << std::endl;
		//	std::cout << "ypts=" << ypts[i] << std::endl;
		//	std::cout << "xy=" << x << "," << y << std::endl;
		//}
		if(!i) {
			cr->move_to(x, y);
		} else {
			cr->line_to(x, y);
		}
	}
	cr->stroke();
}
