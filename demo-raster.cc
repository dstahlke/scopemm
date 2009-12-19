#include "scopemm.h"

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <iostream>

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

		scopemm::RawRGB &data_buf = raster.getDataBuf();
		data_buf.resize(w, h);

		for(int i=0; i<w; i++) {
			for(int j=0; j<h; j++) {
				// FIXME - add a RasterArea::dataIdxToCoord function
				double x = (    double(i)/(w-1))*(xmax-xmin)+xmin;
				double y = (1.0-double(j)/(h-1))*(ymax-ymin)+ymin;
				double vb = sin(sqrt((x*x*4.0+y*y)*200.0) + alpha);
				double vg = cos(sqrt((x*x*4.0+y*y)*200.0) + alpha);
				x -= mouse.mouseX();
				y -= mouse.mouseY();
				double vr;
				if(mouse.mouseIn()) {
					vr = exp(-(x*x+y*y)*10.0);
				} else {
					vr = 0;
				}
				data_buf(i, j, 0) = uint8_t(vr * 255.0);
				data_buf(i, j, 1) = uint8_t((vg+1.0)/2.0 * 255.0);
				data_buf(i, j, 2) = uint8_t((vb+1.0)/2.0 * 255.0);
			}
		}

		fireChangeEvent();

		return true;
	}

	double alpha;
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
