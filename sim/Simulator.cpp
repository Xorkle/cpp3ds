#include <iostream>
#include <gtkmm.h>
#include <sim3ds/sim/Simulator.h>

// Should never be executed. Only defined so the linker sees the
// symbol in the absence of the --wrap option.
extern "C" int __real_main();

namespace cpp3ds {

	Simulator::Simulator(Glib::RefPtr<Gtk::Application> app, Glib::RefPtr<Gtk::Builder> builder){
		application = app;
		thread = new sf::Thread(&Simulator::runGame, this);
		builder->get_widget("window", window);
		builder->get_widget("boxSFML", boxSFML);

		builder->get_widget("aboutDialog", aboutDialog);
		// Works in Gtkmm 3.12 unstable
		// Glib::RefPtr<Gdk::Pixbuf> logo = Gdk::Pixbuf::create_from_resource("/org/cpp3ds/ui/logo.png");
		// aboutDialog->set_logo(logo);

		// Make pausedFrame the cpp3ds logo

		pausedFrame.setTexture(pausedFrameTexture);

		builder->get_widget("saveDialog", saveDialog);

		builder->get_widget("menuAbout", menuAbout);
		builder->get_widget("buttonScreenshot", buttonScreenshot);
		builder->get_widget("buttonPlayPause", buttonPlayPause);
		builder->get_widget("buttonStop", buttonStop);
		builder->get_widget("buttonToggle3D", buttonToggle3D);
		builder->get_widget("scale3D", scale3D);

		// Create and add a SFMLWidget
		screen = new SFMLWidget(sf::VideoMode(800, 480));
		boxSFML->pack_start(*screen, true, true);
		screen->show();

		Glib::signal_timeout().connect(sigc::bind_return(sigc::mem_fun(*this,
			&Simulator::checkThreadState ),true),200);

		menuAbout->signal_activate().connect(sigc::mem_fun(*this,
			&Simulator::on_about_clicked ));

		window->signal_show().connect(sigc::mem_fun(*this,
			&Simulator::on_window_show ));

		screen->signal_size_allocate().connect(sigc::mem_fun(*this,
			&Simulator::on_sfml_size_allocate ));

		buttonScreenshot->signal_clicked().connect(sigc::mem_fun(*this,
			&Simulator::saveScreenshot ));
		buttonPlayPause->signal_clicked().connect(sigc::mem_fun(*this,
			&Simulator::on_playpause_clicked ));
		buttonStop->signal_clicked().connect(sigc::mem_fun(*this,
			&Simulator::on_stop_clicked ));
		buttonToggle3D->signal_toggled().connect(sigc::mem_fun(*this,
			&Simulator::on_toggle3d_clicked ));
	}

	Simulator::~Simulator(){
		stop();
		delete thread;
		delete screen;
	}

	void Simulator::run(){
		application->run(*window);
	}

	// Meant to be run in another thread or will block the GUI.
	void Simulator::runGame(){
		isThreadRunning = true;
		std::cout << "Simulation starting..." << std::endl;
		int ret = __real_main();
		std::cout << "Simulation ended." << std::endl;
		screen->renderWindow.setActive(false);
		isThreadRunning = false;
	}

	void Simulator::checkThreadState(){
		// Check to see if thread ended but GUI hasn't updated
		// (Can't easily update GUI inside thread)
		if (!isThreadRunning && buttonStop->get_sensitive())
			on_stop_clicked();
	}

	void Simulator::play() {
		triggerPause = false;
		screen->renderWindow.setActive(false);
		if (!isThreadRunning)
			thread->launch();
	}

	void Simulator::pause() {
		triggerPause = true;
	}

	void Simulator::stop() {
		// Set flag to trigger stop.
		// It's checked in thread on every frame draw.
		triggerStop = true;
		thread->wait();
		triggerStop = false;
		pausedFrameTexture.update(screen->renderWindow);
	}

	float Simulator::get_slider3d(){
		// Mutex lock here
		if (!buttonToggle3D->get_active())
			return 0;
		return 1.0;
	}

	void Simulator::saveScreenshot(){
		saveDialog->run();
		sf::Image screenie = screen->renderWindow.capture();
		screenie.saveToFile("test.png");
	}


	/***********************
	  UI Events
	 ***********************/
	void Simulator::on_window_show(){
		std::cout << "shown" << std::endl;
	}

	void Simulator::on_sfml_size_allocate(Gtk::Allocation& allocation){
		std::cout << "test" << std::endl;
		screen->renderWindow.draw(pausedFrame);
		// If paused, redraw paused frame
		screen->display();
	}

	void Simulator::on_playpause_clicked(){
		buttonStop->set_sensitive(true);
		if (isPaused){
			buttonPlayPause->set_icon_name("media-playback-pause");
			play();
		} else {
			buttonPlayPause->set_icon_name("media-playback-start");
			pause();
		}
	}

	void Simulator::on_stop_clicked(){
		buttonStop->set_sensitive(false);
		buttonPlayPause->set_sensitive(false);
		buttonPlayPause->set_icon_name("media-playback-start");
		stop();
		buttonPlayPause->set_sensitive(true);
	}

	void Simulator::on_toggle3d_clicked(){
		bool active = buttonToggle3D->get_active();
		scale3D->set_sensitive(active);
		scale3D->set_opacity(active ? 1.0 : 0.8);
		screen->show3D();
		screen->set_size_request(active ? 800 : 400, 480);
		// Resize window to smallest size allowed
		window->resize(1,1);
	}

	void Simulator::on_about_clicked(){
		aboutDialog->run();
	}

}