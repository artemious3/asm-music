#include "MainWindow.h"
#include "sequencer.h"
#include <SFML/Window/Event.hpp>


class InputHandler {
private:
  void close_window();
  // void put_note_on_buffer(uint note_idx);
  // void set_selected_buffer_WT(void* WTptr);
  // void select_buffer();

  void key_pressed(const sf::Event&);
  void key_released(const sf::Event&);
  void special_key(const sf::Event&);

  void play_or_pause();


  int current_octave = 2;
  int sample_inp_counter = 0;
  
   
  MainWindow &window;

public:
  InputHandler(MainWindow &mw);
  void HandleEvent(const sf::Event &ev);
};



