// FIXME - sync this with demo-raster.cc
#define SCOPEMM_ENABLE_BLITZ
#include "scopemm.h"

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <iostream>
#include <blitz/array.h>

class Sinewave : public scopemm::PlotCanvas {
public:
	Sinewave() : 
		alpha(0)
	{
		addLayer(raster);

		Glib::signal_idle().connect(
			sigc::mem_fun(*this, &Sinewave::on_timeout));
	}

	virtual bool on_timeout() {
		alpha += 0.1;

		// changing these will affect the resolution and the
		// position of the pattern, but the red dot should
		// always line up with the mouse cursor
		bool swap_axes = false;
		int w = 100;
		int h = 100;
		scopemm::Bbox bbox(-1, 1, -1, 1);

		raster.setBbox(bbox);
		raster.setSwapAxes(swap_axes);

		if(data_r.shape()[0] != w || data_r.shape()[1] != h) {
			data_r.resize(w, h);
			data_g.resize(w, h);
			data_b.resize(w, h);
		}

		for(int i=0; i<w; i++) {
			for(int j=0; j<h; j++) {
				double x = (    double(i)/(w-1))*(bbox.xmax-bbox.xmin)+bbox.xmin;
				double y = (1.0-double(j)/(h-1))*(bbox.ymax-bbox.ymin)+bbox.ymin;
				data_b(i, j) = sin(sqrt((x*x*4.0+y*y)*200.0) + alpha);
				data_g(i, j) = cos(sqrt((x*x*4.0+y*y)*200.0) + alpha);
				x -= mouseX();
				y -= mouseY();
				if(mouseIn()) {
					data_r(i, j) = exp(-(x*x+y*y)*10.0);
				} else {
					data_r(i, j) = 0;
				}
			}
		}

		// Autoscale:
		// setData(data_r, data_g, data_b);
		// Same scale for each band:
		// setData(data_r, data_g, data_b, -1.0, 1.0);
		// Different scale for each band:
		raster.setData(
			data_r,  0.0, 1.0,
			data_g, -1.0, 1.0,
			data_b, -1.0, 1.0
		);
		// Grascale:
		//setData(data_r, 0.0, 1.0);

		return true;
	}

	double alpha;
	blitz::Array<double, 2> data_r;
	blitz::Array<double, 2> data_g;
	blitz::Array<double, 2> data_b;
	scopemm::RasterArea raster;
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
