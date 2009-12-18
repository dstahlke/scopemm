#define SCOPEMM_ENABLE_BLITZ
#include "scopemm.h"

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <iostream>
#include <blitz/array.h>

class Sinewave : public scopemm::GridCanvas {
public:
	Sinewave() : 
		alpha(0),
		mouse(this)
	{
		setXRange(-1, 1);
		setYRange(-1, 1);

		Glib::signal_idle().connect(
			sigc::mem_fun(*this, &Sinewave::on_timeout));
	}

	virtual bool on_timeout() {
		alpha += 0.1;

		int w = 100;
		int h = 100;
		if(data_r.shape()[0] != w || data_r.shape()[1] != h) {
			data_r.resize(w, h);
			data_g.resize(w, h);
			data_b.resize(w, h);
		}

		for(int i=0; i<w; i++) {
			for(int j=0; j<h; j++) {
				double x =  (double(i)/(w-1)*2.0-1.0);
				double y = -(double(j)/(h-1)*2.0-1.0);
				data_b(i, j) = sin(sqrt((x*x+y*y)*200.0) + alpha);
				data_g(i, j) = cos(sqrt((x*x+y*y)*200.0) + alpha);
				x -= mouse.mouseX();
				y -= mouse.mouseY();
				if(mouse.mouseIn()) {
					data_r(i, j) = exp(-(x*x+y*y)*10.0);
				} else {
					data_r(i, j) = 0;
				}
			}
		}

		setData(data_r, data_g, data_b);

		return true;
	}

	double alpha;
	blitz::Array<double, 2> data_r;
	blitz::Array<double, 2> data_g;
	blitz::Array<double, 2> data_b;
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
