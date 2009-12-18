#define SCOPEMM_ENABLE_BLITZ
#include "scopemm.h"

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <iostream>
#include <blitz/array.h>

class Sinewave : public scopemm::GridCanvas {
public:
	Sinewave() : alpha(0) {
		Glib::signal_idle().connect(
			sigc::mem_fun(*this, &Sinewave::on_timeout));
	}

	virtual bool on_timeout() {
		alpha += 0.1;

		int w = 100;
		int h = 100;
		blitz::Array<double, 2> data(w, h);

		for(int i=0; i<w; i++) {
			double x = double(i)/(w-1)*2.0-1.0;
			for(int j=0; j<h; j++) {
				double y = double(j)/(h-1)*2.0-1.0;
				data(i, j) = sin(exp((x*x+y*y)*2.0) + alpha);
			}
		}

		setData(data);

		return true;
	}

	double alpha;
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
