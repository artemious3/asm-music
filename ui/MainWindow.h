#ifndef MAINWINDOW_H_Q7MVEE9O
#define MAINWINDOW_H_Q7MVEE9O

#include <SFML/Graphics.hpp>
#include "sequencer.h"
#include "config.h"

class MainWindow : public sf::RenderWindow {

public:
	MainWindow();


	void drawEverything();

private:
	void drawTrackAt(float x, float y, bool is_sample);
	void drawWTInfo();
	void drawBufferNotes(Buffer* buf,float bufX, float bufY);
	void drawPlayhead();
	void drawDocs();


	sf::Font font;
	sf::Texture sharp_texture;
	bool refreshNotes = false;
	
};

#endif /* end of include guard: MAINWINDOW_H_Q7MVEE9O */
