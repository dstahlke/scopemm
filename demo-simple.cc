#include "scopemm.h"

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <math.h>

int main(int argc, char *argv[]) {
	Gtk::Main kit(argc, argv);

	Gtk::Window win;
	win.set_title("Test");

	Plot1D plot;
	plot.setXAutoRange();
	plot.setYAutoRange();
	plot.setDrawAxes(true);
	plot.setDrawGrids(true);

	PlotTracePtr t1 = plot.addTrace();
	PlotTracePtr t2 = plot.addTrace();
	t1->setColor(1, 0, 0);
	t2->setColor(0, 1, 0);

	const int nsamps = 100;

	std::vector<double> ypts;
	for(int i=0; i<nsamps; i++) {
		double theta = double(i) / nsamps * 2.0 * M_PI;
		ypts.push_back(sin(theta));
	}
	t1->setYData(ypts.begin(), ypts.end());

	std::vector<float> xpts;
	ypts.clear();
	for(int i=0; i<nsamps; i++) {
		double theta = double(i) / nsamps * 2.0 * M_PI;
		// The call to setYData above implicitly used an X range
		// of [0, nsamps-1].  We multiply X by nsamps here in order
		// to occupy the same range.
		xpts.push_back(cos(theta) * nsamps);
		ypts.push_back(sin(theta));
	}
	t2->setXYData(xpts.begin(), xpts.end(), ypts.begin(), ypts.end());

	win.add(plot);
	plot.show();

	Gtk::Main::run(win);

	return 0;
}
