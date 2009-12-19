#define SCOPEMM_ENABLE_BLITZ
#include "scopemm.h"

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <iostream>
#include <blitz/array.h>

class Sinewave : public scopemm::PlotCanvas {
public:
	Sinewave() {
		setXAutoRange();
		setYAutoRange();
		//setSwapAxes();
		setDrawAxes();
		setDrawGrids();

		addLayer(t1);
		addLayer(t2);
		addLayer(t3);

		t1.setColor(1, 0, 0);
		t2.setColor(0, 1, 0);
		t3.setColor(0, 0, 1);

		alpha = 0;

		Glib::signal_idle().connect(
			sigc::mem_fun(*this, &Sinewave::on_timeout));
	}

	virtual bool on_timeout() {
		alpha += 10.0;

		double scale = exp(-alpha/1000.0);

		int npts = 1000;
		//setXRange(0, npts-1);
		blitz::Array<double, 1> xpts(npts);
		blitz::Array<double, 1> ypts(npts);
		blitz::Array<double, 1> zpts(npts);
		blitz::Array<blitz::TinyVector<double, 2>, 1> xypts(npts);
		blitz::firstIndex i;

		xpts = cos(10.0 * 2.0 * 3.14159 * (i-npts/2) / npts + alpha / 200.0) *
			exp(-(i-npts/2)*(i-npts/2)/alpha) * scale;

		ypts = sin(10.0 * 2.0 * 3.14159 * (i-npts/2) / npts + alpha / 200.0) *
			exp(-(i-npts/2)*(i-npts/2)/alpha) * scale;

		zpts = 2.0 * (i-npts/2) / (double)npts * scale;

		xypts[0] = xpts;
		xypts[1] = ypts;

		t1.setXYData(xypts);
		t2.setXYData(xpts.begin(), xpts.end(), zpts.begin(), zpts.end());
		t2.setXYData(zpts.begin(), zpts.end(), ypts.begin(), ypts.end());

		// autoscale would work too, but this is smoother and keeps the origin
		// right in the middle
		setXRange(-scale, scale);
		setYRange(-scale, scale);

		return true;
	}

	double alpha;
	scopemm::PlotTrace t1;
	scopemm::PlotTrace t2;
	scopemm::PlotTrace t3;
};

int main(int argc, char *argv[]) {
	Gtk::Main kit(argc, argv);

	Gtk::Window win;
	win.set_title("Test");

	Sinewave sw;

	win.add(sw);
	sw.show();

	Gtk::Main::run(win);

	return 0;
}
