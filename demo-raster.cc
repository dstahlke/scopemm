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

		// changing these will affect the resolution and the
		// position of the pattern, but the red dot should
		// always line up with the mouse cursor
		//setBbox(scopemm::Bbox(-2, 3, -2, 3));
		raster.setBbox(scopemm::Bbox(-1, 1, -1, 1));
		raster.setSwapAxes(false);
		setSwapAxes(false);

		// bilinear interpolation, makes it look nicer
		raster.setBilinear(true);

		Glib::signal_idle().connect(
			sigc::mem_fun(*this, &Sinewave::on_timeout));
	}

	virtual bool on_timeout() {
		alpha += 0.1;

		int w = 100;
		int h = 100;

		scopemm::RawRGB &data_buf = raster.getDataBuf();
		data_buf.resize(w, h);
		scopemm::AffineTransform affine = raster.getAffine(); // FIXME - docs (requires resize)

		for(int i=0; i<w; i++) {
			for(int j=0; j<h; j++) {
				double x, y;
				affine.fwd(i+0.5, j+0.5, x, y); // FIXME - docs for 0.5
				double vb = sin(sqrt((x*x*4.0+y*y)*200.0) + alpha);
				double vg = cos(sqrt((x*x*4.0+y*y)*200.0) + alpha);
				x -= mouse.mouseX();
				y -= mouse.mouseY();
				double vr;
				if(mouse.mouseIn()) {
					vr = exp(-(x*x+y*y)*50.0);
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
