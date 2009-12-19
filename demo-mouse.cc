#include "scopemm.h"

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <math.h>

class DemoWidget : public scopemm::Plot1D {
public:
	DemoWidget() :
		mouse(this),
		click_x(0),
		click_y(0)
	{
		setXRange(-10, 10);
		setYRange(-10, 10);
		setDrawAxes(true);
		setDrawGrids(true);

		addTrace(t1);
		addTrace(t2);

		mouse.signal_clicked.connect(sigc::mem_fun(
			this, &DemoWidget::mouse_clicked));
		mouse.signal_motion.connect(sigc::mem_fun(
			this, &DemoWidget::mouse_motion));
	}

	void mouse_clicked(int button) {
		click_x = mouse.mouseX();
		click_y = mouse.mouseY();

		double r = 1.0;
		const int nsamps = 100;
		std::vector<double> xpts;
		std::vector<double> ypts;
		for(int i=0; i<nsamps; i++) {
			double theta = double(i) / (nsamps-1) * 2.0 * M_PI;
			xpts.push_back(cos(theta)*r + click_x);
			ypts.push_back(sin(theta)*r + click_y);
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
		if(mouse.mouseIn()) {
			xpts.push_back(click_x);
			xpts.push_back(mouse.mouseX());
			ypts.push_back(click_y);
			ypts.push_back(mouse.mouseY());
		}
		t2.setXYData(xpts.begin(), xpts.end(), ypts.begin(), ypts.end());
		t2.setColor(
			mouse.button1() ? 1 : 0.5,
			mouse.button2() ? 1 : 0.5,
			mouse.button3() ? 1 : 0.5
		);
	}

	scopemm::MouseAdapter mouse;
	scopemm::PlotTrace t1;
	scopemm::PlotTrace t2;
	double click_x;
	double click_y;
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
