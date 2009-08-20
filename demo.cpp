#include <gtkmm/main.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/drawingarea.h>
#include <boost/foreach.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

class Plot1D;
class PlotTrace;

typedef boost::shared_ptr<PlotTrace> PlotTracePtr;

class Plot1D : public Gtk::HBox {
public:
	Plot1D();
	virtual ~Plot1D() { }

	virtual PlotTracePtr addTrace();
	virtual void setXRange(double min, double max);
	virtual void setYRange(double min, double max);
	virtual bool on_expose_event(GdkEventExpose* event);

	std::vector<PlotTracePtr> traces;
	double xmin, xmax, ymin, ymax;
};

class PlotTrace : private boost::noncopyable {
public:
	PlotTrace(Plot1D *_parent);

	~PlotTrace();

	virtual void setXRange(double min, double max);
	virtual void setYRange(double min, double max);
	virtual void setColor(double r, double g, double b);
	virtual void setData(double *_ypts, size_t _npts);
	virtual void setData(double *_xpts, double *_ypts, size_t _npts);

	virtual void draw(Cairo::RefPtr<Cairo::Context>);
	
	Plot1D *parent;
	std::vector<double> xpts;
	std::vector<double> ypts;
	double xmin, xmax, ymin, ymax;
	double rgb[3];
};

///////////////////////////////////////////

Plot1D::Plot1D() :
	xmin(0), xmax(1), ymin(0), ymax(1)
{ }

void Plot1D::setXRange(double min, double max) {
	xmin = min;
	xmax = max;
	BOOST_FOREACH(PlotTracePtr t, traces) {
		t->setXRange(xmin, xmax);
	}
}

void Plot1D::setYRange(double min, double max) {
	ymin = min;
	ymax = max;
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

///////////////////////////////////////////

PlotTrace::PlotTrace(Plot1D *_parent) : 
	parent(_parent),
	xmin(0), xmax(1), ymin(0), ymax(1)
{ 
	setColor(1, 0, 0);
	setXRange(parent->xmin, parent->xmax);
	setYRange(parent->ymin, parent->ymax);
}

PlotTrace::~PlotTrace() {
	std::cout << "PlotTrace dtor" << std::endl;
}

PlotTracePtr Plot1D::addTrace() {
	PlotTracePtr trace(new PlotTrace(this));
	traces.push_back(trace);
	return trace;
}

void PlotTrace::setXRange(double min, double max) {
	xmin = min;
	xmax = max;
}

void PlotTrace::setYRange(double min, double max) {
	ymin = min;
	ymax = max;
}

void PlotTrace::setColor(double r, double g, double b) {
	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}

void PlotTrace::setData(double *_ypts, size_t _npts) {
	setData(0, _ypts, _npts);
}

void PlotTrace::setData(double *_xpts, double *_ypts, size_t _npts) {
	xpts.clear();
	ypts.clear();

	if(_xpts) {
		for(size_t i=0; i<_npts; ++i) {
			xpts.push_back(_xpts[i]);
		}
	} else {
		for(size_t i=0; i<_npts; ++i) {
			xpts.push_back(i);
		}
	}
	for(size_t i=0; i<_npts; ++i) {
		//if(i<5) {
		//std::cout << "ypts=" << _ypts[i] << std::endl;
		//}
		ypts.push_back(_ypts[i]);
	}
}

void PlotTrace::draw(Cairo::RefPtr<Cairo::Context> cr) {
	cr->set_line_width(1);
	cr->set_source_rgb(rgb[0], rgb[1], rgb[2]);

	const int w = parent->get_allocation().get_width();
	const int h = parent->get_allocation().get_height();

	double xmin = parent->xmin;
	double xmax = parent->xmax;
	double ymin = parent->ymin;
	double ymax = parent->ymax;

	size_t npts = xpts.size();
	//std::cout << "npts=" << npts << std::endl;
	for(size_t i=0; i<npts; ++i) {
		//if(i<5) {
		//std::cout << "xpts=" << xpts[i] << std::endl;
		//std::cout << "ypts=" << ypts[i] << std::endl;
		//}
		double x = (xpts[i]-xmin)/(xmax-xmin)*(w-1);
		double y = (ymax-ypts[i])/(ymax-ymin)*(h-1);
		if(!i) {
			cr->move_to(x, y);
		} else {
			cr->line_to(x, y);
		}
	}
	cr->stroke();
}

class Sinewave {
public:
	Sinewave(Plot1D &_plot) : plot(_plot) {
		npts = 1000;
		ypts = new double[npts];

		plot.setXRange(0, npts-1);
		plot.setYRange(-1, 1);

		t1 = plot.addTrace();
		t2 = plot.addTrace();
		t1->setColor(1, 0, 0);
		t2->setColor(0, 0, 1);
		alpha = 0;

		Glib::signal_idle().connect(
			sigc::mem_fun(*this, &Sinewave::on_timeout));
	}

	virtual bool on_timeout() {
		alpha += 10.0;
		for(int i=0; i<npts; ++i) {
			ypts[i] = cos(10.0 * 2.0 * 3.14159 * (double)(i-npts/2) / (double)npts +
				alpha / 200.0) *
				exp(-(i-npts/2)*(i-npts/2)/alpha);
		}
		t1->setData(ypts, npts);
		for(int i=0; i<npts; ++i) {
			ypts[i] = sin(10.0 * 2.0 * 3.14159 * (double)(i-npts/2) / (double)npts +
				alpha / 200.0) *
				exp(-(i-npts/2)*(i-npts/2)/alpha);
		}
		t2->setData(ypts, npts);
		plot.queue_draw();
		return true;
	}

	double alpha;
	int npts;
	double *ypts;
	Plot1D &plot;
	PlotTracePtr t1;
	PlotTracePtr t2;
};

int main(int argc, char *argv[]) {
	Gtk::Main kit(argc, argv);

	Gtk::Window win;
	win.set_title("Test");

	Plot1D plot;
	win.add(plot);
	plot.show();

	Sinewave sw(plot);

	Gtk::Main::run(win);

	return 0;
}
