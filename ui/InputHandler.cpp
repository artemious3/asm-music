#include "InputHandler.h"
#include "MainWindow.h"
#include <map>
#include "sequencer.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <portaudio.h>

__attribute__((aligned(1))) volatile uint64_t INP_InputNote = 0;
__attribute__((aligned(1))) volatile uint64_t INP_WasInput = 0;
__attribute__((aligned(1))) volatile Buffer* INP_SelectedBuf = &Buffers[0];

static const int SECTOR_MOVE_STEP = 4;

enum KeyInfo{
	KeyNone,
	KeyNote,
	KeyTrack,
	KeySpecial
};

static const std::map<sf::Keyboard::Key, void*> WT_BUFFERS {
	{sf::Keyboard::Key::Num1, SineTblWT},
	{sf::Keyboard::Key::Num2, SawtoothTblWT},
	{sf::Keyboard::Key::Num3, SquareTblWT},
	{sf::Keyboard::Key::Num4, KickTblWT},
	{sf::Keyboard::Key::Num5, SnareTblWT},
};

static const std::map<sf::Keyboard::Key, KeyInfo > KEY_MASK = {
	{sf::Keyboard::Key::A, KeyNote},
	{sf::Keyboard::Key::W, KeyNote},
	{sf::Keyboard::Key::S, KeyNote},
	{sf::Keyboard::Key::E, KeyNote},
	{sf::Keyboard::Key::D, KeyNote},
	{sf::Keyboard::Key::F, KeyNote},
	{sf::Keyboard::Key::T, KeyNote},
	{sf::Keyboard::Key::G, KeyNote},
	{sf::Keyboard::Key::Y, KeyNote},
	{sf::Keyboard::Key::H, KeyNote},
	{sf::Keyboard::Key::U, KeyNote},
	{sf::Keyboard::Key::J, KeyNote},

	{sf::Keyboard::Key::Num1, KeyTrack},
	{sf::Keyboard::Key::Num2, KeyTrack},
	{sf::Keyboard::Key::Num3, KeyTrack},
	{sf::Keyboard::Key::Num4, KeyTrack},
	{sf::Keyboard::Key::Num5, KeyTrack},
	{sf::Keyboard::Key::Num6, KeyTrack},
	{sf::Keyboard::Key::Num7, KeyTrack},
	{sf::Keyboard::Key::Num8, KeyTrack},

	{sf::Keyboard::Key::Space, KeySpecial},
	{sf::Keyboard::Key::Z, KeySpecial},
	{sf::Keyboard::Key::X, KeySpecial},
	{sf::Keyboard::Key::Left, KeySpecial},
	{sf::Keyboard::Key::Right, KeySpecial},
	{sf::Keyboard::Key::C, KeySpecial},

};

static const std::map<sf::Keyboard::Key, uint> NOTE_IDX = {	
	{sf::Keyboard::Key::A, 0},
	{sf::Keyboard::Key::W, 1},
	{sf::Keyboard::Key::S, 2},
	{sf::Keyboard::Key::E, 3},
	{sf::Keyboard::Key::D, 4},
	{sf::Keyboard::Key::F, 5},
	{sf::Keyboard::Key::T, 6},
	{sf::Keyboard::Key::G, 7},
	{sf::Keyboard::Key::Y, 8},
	{sf::Keyboard::Key::H, 9},
	{sf::Keyboard::Key::U, 10},
	{sf::Keyboard::Key::J, 11},
};


KeyInfo GET_KEY_INFO(sf::Keyboard::Key key){
	auto maybe_info = KEY_MASK.find(key);
	if(maybe_info != KEY_MASK.end()){
		return maybe_info->second;
	}
	return KeyNone;
}

InputHandler::InputHandler(MainWindow& mw)
	: window(mw){
}

void InputHandler::key_pressed(const sf::Event &event) {
  // assuming that event is KeyPressEvent

auto ev = event.getIf<sf::Event::KeyPressed>();

  switch (GET_KEY_INFO(ev->code)) {
  case KeyNote:
    if (INP_SelectedBuf->is_sample) {
      INP_InputNote = 1;
    } else {
      INP_InputNote = 2 + current_octave * 12 + NOTE_IDX.at(ev->code);
    }
    INP_WasInput = 1;
    break;

  case KeyTrack: {
    int track_idx = static_cast<int>(ev->code) -
                    static_cast<int>(sf::Keyboard::Key::Num0) - 1;
    INP_SelectedBuf = &Buffers[track_idx];
    break;
  }
  case KeySpecial:
    special_key(*ev);
    break;
  }
}

void InputHandler::play_or_pause() {
  if (Pa_IsStreamStopped(Stream_ptr)) {
    SequencerPlay();
  } else {
    SequencerStop();
  }
}

void InputHandler::special_key(const sf::Event &event) {
auto ev = event.getIf<sf::Event::KeyPressed>();
  switch (ev->code) {
  case sf::Keyboard::Key::Space:
    play_or_pause();
    break;
  case sf::Keyboard::Key::Left:
    if (Pa_IsStreamStopped(Stream_ptr) && Current_sector >= SECTOR_MOVE_STEP) {
      Current_sector -= SECTOR_MOVE_STEP;
    }
    break;
  case sf::Keyboard::Key::Right:
    if (Pa_IsStreamStopped(Stream_ptr) &&
        Current_sector <= BUFFER_SIZE_SECTORS - SECTOR_MOVE_STEP) {
      Current_sector += SECTOR_MOVE_STEP;
    }
    break;
  case sf::Keyboard::Key::C: {
    auto notes = INP_SelectedBuf->note_ptr;
    for (int i = 0; i < BUFFER_SIZE_SECTORS; ++i) {
      notes[i] = 0;
    }
  } break;
  case sf::Keyboard::Key::Z:
    if (current_octave > 0) {
      current_octave--;
    }
    break;
  case sf::Keyboard::Key::X:
    if (current_octave < 4) {
      current_octave++;
    }
  default:
    return;
  }
}

void InputHandler::key_released(const sf::Event &event) {
auto ev = event.getIf<sf::Event::KeyReleased>();
  if (GET_KEY_INFO(ev->code) == KeyNote) {
    INP_WasInput = 0;
  }
}

void InputHandler::HandleEvent(const sf::Event &event) {
	if(event.is<sf::Event::Closed>()) {
		window.close();
	} else if (event.is<sf::Event::KeyPressed>()) {
		key_pressed(event);
	} else if (event.is<sf::Event::KeyReleased>()) {
		key_released(event);
	}
}
