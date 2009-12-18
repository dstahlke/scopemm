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
		setYRange(1, -1);

		Glib::signal_idle().connect(
			sigc::mem_fun(*this, &Sinewave::on_timeout));
	}

	virtual bool on_timeout() {
		alpha += 0.1;

		int w = 200;
		int h = 100;
		blitz::Array<double, 2> data(w, h);

		double mx = mouse.mouseX();
		double my = mouse.mouseY();
		std::cout << "mx=" << mx << ", my=" << my << std::endl;

		for(int i=0; i<w; i++) {
			for(int j=0; j<h; j++) {
				double x = double(i)/(w-1)*2.0-1.0;
				double y = double(j)/(h-1)*2.0-1.0;
				data(i, j) = sin(sqrt((x*x+y*y)*50.0) + alpha);
				x -= mx;
				y -= my;
				data(i, j) += exp(-(x*x+y*y)*10.0);
			}
		}

		setData(data);

		return true;
	}

	double alpha;
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
