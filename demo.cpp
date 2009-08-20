#include <gtkmm/main.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/drawingarea.h>
#include <iostream>

class Plot1D;

class PlotTrace {
public:
	PlotTrace() : 
		npts(0), xpts(0), ypts(0),
		xmin(0), xmax(1), ymin(0), ymax(1)
	{ }

	// FIXME - destructor, copy, etc

	virtual void setXRange(double min, double max);
	virtual void setYRange(double min, double max);
	virtual void setData(double *_ypts, int _npts);

	virtual void draw(Cairo::RefPtr<Cairo::Context>, Plot1D *parent);
	
	int npts;
	double *xpts;
	double *ypts;
	double xmin, xmax, ymin, ymax;
};

class Plot1D : public Gtk::HBox {
public:
	Plot1D();
	virtual ~Plot1D() { }

	virtual void addTrace();
	virtual void setXRange(double min, double max);
	virtual void setYRange(double min, double max);
	virtual bool on_expose_event(GdkEventExpose* event);

	PlotTrace *trace;
	double xmin, xmax, ymin, ymax;
};

Plot1D::Plot1D() :
	trace(0),
	xmin(0), xmax(1), ymin(0), ymax(1)
{ }

void Plot1D::addTrace() {
	trace = new PlotTrace();
	trace->setXRange(xmin, xmax);
	trace->setYRange(ymin, ymax);
}

void Plot1D::setXRange(double min, double max) {
	xmin = min;
	xmax = max;
	if(trace) trace->setXRange(xmin, xmax);
}

void Plot1D::setYRange(double min, double max) {
	ymin = min;
	ymax = max;
	if(trace) trace->setYRange(ymin, ymax);
}

bool Plot1D::on_expose_event(GdkEventExpose* event) {
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window) {
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
		cr->rectangle(event->area.x, event->area.y,
			event->area.width, event->area.height);
		cr->clip();
		if(trace) trace->draw(cr, this);
	}
	return true;
}

void PlotTrace::setXRange(double min, double max) {
	xmin = min;
	xmax = max;
}

void PlotTrace::setYRange(double min, double max) {
	ymin = min;
	ymax = max;
}

void PlotTrace::setData(double *_ypts, int _npts) {
	if(_npts != npts) {
		npts = _npts;
		if(xpts) delete[] xpts;
		if(ypts) delete[] ypts;
		xpts = new double[npts];
		ypts = new double[npts];
	}
	for(int i=0; i<npts; i++) {
		xpts[i] = i;
		ypts[i] = _ypts[i];
	}
}

void PlotTrace::draw(Cairo::RefPtr<Cairo::Context> cr, Plot1D *parent) {
	cr->set_line_width(1);
	cr->set_source_rgb(0.8, 0.0, 0.0);

	const int w = parent->get_allocation().get_width();
	const int h = parent->get_allocation().get_height();

	double xmin = parent->xmin;
	double xmax = parent->xmax;
	double ymin = parent->ymin;
	double ymax = parent->ymax;

	for(int i=0; i<npts; ++i) {
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
	Sinewave(Plot1D &_hw) : plot(_hw) {
		npts = 1000;
		ypts = new double[npts];

		plot.setXRange(0, npts-1);
		plot.setYRange(-1, 1);

		plot.addTrace();
		alpha = 0;

		Glib::signal_idle().connect(
			sigc::mem_fun(*this, &Sinewave::on_timeout));
	}

	virtual bool on_timeout() {
		alpha += 10.0;
		for(int i=0; i<npts; i++) {
			ypts[i] = cos(10.0 * 2.0 * 3.14159 * (double)(i-npts/2) / (double)npts +
				alpha / 200.0) *
				exp(-(i-npts/2)*(i-npts/2)/alpha);
		}
		plot.trace->setData(ypts, npts);
		plot.queue_draw();
		return true;
	}

	double alpha;
	int npts;
	double *ypts;
	Plot1D &plot;
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
