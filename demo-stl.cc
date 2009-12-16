#include "scopemm.h"

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <iostream>

class Sinewave {
public:
	Sinewave(Plot1D &_plot) : 
		plot(_plot)
	{
		plot.setXAutoRange();
		plot.setYAutoRange();
		//plot.setSwapAxes();
		plot.setDrawAxes();
		plot.setDrawGrids();

		(t1 = plot.addTrace())->setColor(1, 0, 0);
		(t2 = plot.addTrace())->setColor(0, 1, 0);
		(t3 = plot.addTrace())->setColor(0, 0, 1);

		alpha = 0;

		Glib::signal_idle().connect(
			sigc::mem_fun(*this, &Sinewave::on_timeout));
	}

	virtual bool on_timeout() {
		alpha += 10.0;

		double scale = exp(-alpha/1000.0);

		int npts = 1000;

		std::vector<double> xpts;
		std::vector<double> ypts;
		std::vector<double> zpts;
		std::vector<std::pair<double, double> > xypts;

		for(int i=0; i<npts; i++) {
			double x = cos(10.0 * 2.0 * 3.14159 * (i-npts/2) / npts + alpha / 200.0) *
				exp(-(i-npts/2)*(i-npts/2)/alpha) * scale;

			double y = sin(10.0 * 2.0 * 3.14159 * (i-npts/2) / npts + alpha / 200.0) *
				exp(-(i-npts/2)*(i-npts/2)/alpha) * scale;

			double z = 2.0 * (i-npts/2) / (double)npts * scale;

			xpts.push_back(x);
			ypts.push_back(y);
			zpts.push_back(z);
			xypts.push_back(std::pair<double, double>(x, y));
		}

		t1->setXYData(xypts.begin(), xypts.end());
		t2->setXYData(xpts.begin(), xpts.end(), zpts.begin(), zpts.end());
		t3->setXYData(zpts.begin(), zpts.end(), ypts.begin(), ypts.end());

		plot.setXRange(-scale, scale);
		plot.setYRange(-scale, scale);

		return true;
	}

	double alpha;
	Plot1D &plot;
	PlotTracePtr t1;
	PlotTracePtr t2;
	PlotTracePtr t3;
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
