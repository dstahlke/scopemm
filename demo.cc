#include "scopemm.h"
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <iostream>
#include <blitz/array.h>

class Sinewave {
public:
	Sinewave(Plot1D &_plot) : 
		plot(_plot)
	{
		//plot.setXRange(-1, 1);
		//plot.setYRange(-1, 1);
		plot.setXAutoRange();
		plot.setYAutoRange();
		//plot.setSwapAxes();
		plot.setDrawAxes();
		plot.setDrawGrids();

		(t1 = plot.addTrace())->
			setColor(1, 0, 0);
		(t2 = plot.addTrace())->
			setColor(0, 0, 1);
		alpha = 0;

		Glib::signal_idle().connect(
			sigc::mem_fun(*this, &Sinewave::on_timeout));
	}

	virtual bool on_timeout() {
		alpha += 10.0;

		int npts = 1000;
		//plot.setXRange(0, npts-1);
		blitz::Array<double, 1> xpts(npts);
		blitz::Array<double, 1> ypts(npts);
		blitz::firstIndex i;

		xpts = cos(10.0 * 2.0 * 3.14159 * (i-npts/2) / npts + alpha / 200.0) *
			exp(-(i-npts/2)*(i-npts/2)/alpha);

		ypts = sin(10.0 * 2.0 * 3.14159 * (i-npts/2) / npts + alpha / 200.0) *
			exp(-(i-npts/2)*(i-npts/2)/alpha);

		t1->setXYData(xpts, ypts);

		xpts = 2.0 * (i-npts/2) / (double)npts;

		t2->setXYData(xpts, ypts);

		plot.setXRange(-exp(alpha/1000.0), exp(alpha/1000.0));
		plot.setYRange(-exp(alpha/1000.0), exp(alpha/1000.0));

		return true;
	}

	double alpha;
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
