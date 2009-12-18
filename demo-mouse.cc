#include "scopemm.h"

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <math.h>

class DemoWidget : public Plot1D {
public:
	DemoWidget() :
		mouse(this)
	{
		setXRange(-10, 10);
		setYRange(-10, 10);
		setDrawAxes(true);
		setDrawGrids(true);

		t1 = addTrace();
		t2 = addTrace();

		mouse.signal_clicked.connect(sigc::mem_fun(
			this, &DemoWidget::mouse_clicked));
		mouse.signal_motion.connect(sigc::mem_fun(
			this, &DemoWidget::mouse_motion));
	}

	void mouse_clicked(int button) {
		double r = 1.0;
		const int nsamps = 100;
		std::vector<double> xpts;
		std::vector<double> ypts;
		for(int i=0; i<nsamps; i++) {
			double theta = double(i) / (nsamps-1) * 2.0 * M_PI;
			xpts.push_back(cos(theta)*r + mouse.mouseX());
			ypts.push_back(sin(theta)*r + mouse.mouseY());
		}
		t1.setXYData(xpts.begin(), xpts.end(), ypts.begin(), ypts.end());
		t1.setColor(
			button==1 ? 1 : 0,
			button==2 ? 1 : 0,
			button==3 ? 1 : 0
		);
	}

	void mouse_motion() {
		std::vector<double> xpts;
		std::vector<double> ypts;
		xpts.push_back(0);
		xpts.push_back(mouse.mouseX());
		ypts.push_back(0);
		ypts.push_back(mouse.mouseY());
		t2.setXYData(xpts.begin(), xpts.end(), ypts.begin(), ypts.end());
		t2.setColor(
			mouse.button1() ? 1 : 0.5,
			mouse.button2() ? 1 : 0.5,
			mouse.button3() ? 1 : 0.5
		);
	}

	MouseAdapter mouse;
	PlotTrace t1;
	PlotTrace t2;
};

int main(int argc, char *argv[]) {
	Gtk::Main kit(argc, argv);

	Gtk::Window win;
	win.set_title("Test");

	DemoWidget demo;
	win.add(demo);
	demo.show();

	Gtk::Main::run(win);

	return 0;
}
