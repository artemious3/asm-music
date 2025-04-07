#include <SFML/Graphics.hpp>
#include "MainWindow.h"
#include "sequencer.h"
#include "InputHandler.h"
using namespace std;


int main() {
 
    SequencerInit();
    MainWindow App{};
    InputHandler handler{App};

    App.setVerticalSyncEnabled(true);


    while (App.isOpen()) {
	App.clear();
        while (const auto event = App.pollEvent()) {
		if(event.has_value())
			handler.HandleEvent(*event);
        }
	App.drawEverything();
        App.display();
    }
    return 0;
}
