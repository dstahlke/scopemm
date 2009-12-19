#define SCOPEMM_ENABLE_BLITZ
#include "scopemm.h"

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <iostream>
#include <blitz/array.h>

class Sinewave : public scopemm::PlotCanvas {
public:
	Sinewave() : 
		alpha(0),
		mouse(this)
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
		double xmin = -1;
		double xmax =  1;
		double ymin = -1;
		double ymax =  1;

		raster.setXRange(xmin, xmax);
		raster.setYRange(ymin, ymax);
		raster.setSwapAxes(swap_axes);

		if(data_r.shape()[0] != w || data_r.shape()[1] != h) {
			data_r.resize(w, h);
			data_g.resize(w, h);
			data_b.resize(w, h);
		}

		for(int i=0; i<w; i++) {
			for(int j=0; j<h; j++) {
				double x = (    double(i)/(w-1))*(xmax-xmin)+xmin;
				double y = (1.0-double(j)/(h-1))*(ymax-ymin)+ymin;
				data_b(i, j) = sin(sqrt((x*x*4.0+y*y)*200.0) + alpha);
				data_g(i, j) = cos(sqrt((x*x*4.0+y*y)*200.0) + alpha);
				x -= mouse.mouseX();
				y -= mouse.mouseY();
				if(mouse.mouseIn()) {
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
	scopemm::MouseAdapter mouse;
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
