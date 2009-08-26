#include "gtk-blitz-lineplot.hpp"
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <iostream>
#include <blitz/array.h>

class Sinewave {
public:
	Sinewave(Plot1D &_plot) : 
		npts(1000), 
		xpts(npts),
		ypts(npts),
		plot(_plot)
	{
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
			xpts(i) = cos(10.0 * 2.0 * 3.14159 * (double)(i-npts/2) / (double)npts +
				alpha / 200.0) *
				exp(-(i-npts/2)*(i-npts/2)/alpha);
		}
		xpts = xpts*500.0 + 500.0;
		for(int i=0; i<npts; ++i) {
			ypts(i) = sin(10.0 * 2.0 * 3.14159 * (double)(i-npts/2) / (double)npts +
				alpha / 200.0) *
				exp(-(i-npts/2)*(i-npts/2)/alpha);
		}
		t1->setXYData(xpts, ypts);
		t2->setYData(ypts);
		plot.queue_draw();
		return true;
	}

	double alpha;
	int npts;
	blitz::Array<double, 1> xpts;
	blitz::Array<double, 1> ypts;
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
