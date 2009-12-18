#include "scopemm.h"

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <iostream>

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
		data_buf.resize(w, h);

		for(int i=0; i<w; i++) {
			for(int j=0; j<h; j++) {
				double x =  (double(i)/(w-1)*2.0-1.0);
				double y = -(double(j)/(h-1)*2.0-1.0);
				double vb = sin(sqrt((x*x+y*y)*200.0) + alpha);
				double vg = cos(sqrt((x*x+y*y)*200.0) + alpha);
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
