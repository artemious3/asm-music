#include "MainWindow.h"
#include "sequencer.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <cstdint>
#include <cstdlib>
#include <unordered_map>

struct NoteInfo {
  int octave;
  int note_idx;
  bool is_sharped;
};



static const uint TRACK_HEIGHT = 100;
static const uint TRACK_CONTENT_LENGTH = 1100;
static const uint TRACK_INFO_LENGTH = 200;
static const uint TRACK_GAP = 4.0;
static const uint LINE_GAP = TRACK_HEIGHT / 6;

static const uint NOTE_HEIGHT = LINE_GAP * 0.8;
static const float NOTE_SECTOR_WIDTH =
    (float)TRACK_CONTENT_LENGTH / BUFFER_SIZE_SECTORS;

static const uint TRACK_NUMBER = 6;

static const uint DOCS_HEIGHT = 250;

static const long WINDOW_SIZE_X =
    2 * TRACK_GAP + TRACK_INFO_LENGTH + TRACK_CONTENT_LENGTH;
const long WINDOW_SIZE_Y =
    2 * TRACK_GAP + (TRACK_HEIGHT + TRACK_GAP) * TRACK_NUMBER + DOCS_HEIGHT;
static const sf::String TITLE = "ASM MUSIC :3333333333333";

static const sf::String DOCS = R"""(
    |~~~~~~~~~~~~~~~|     1-9 :         select track
    |               |     AWSEDFTGYHJ : play note      C: clear 
    |               |     ZX :          change octave
/~~\|           /~~\|     Space :       play/stop
\__/            \__/
)""";

static const std::unordered_map<int8_t *, sf::String> WT_NAMES = {
    {SineTblWT, "SINE"},
    {SawtoothTblWT, "SAWTOOTH"},
    {SquareTblWT, "SQUARE"},
    {KickTblWT, "KICK"},
    {SnareTblWT, "SNARE"}};

static NoteInfo GetNoteInfo(uint note) {
  NoteInfo info;
  if (note < 2) {
    std::abort();
  }
  info.note_idx = (note - 2) % 12;
  info.octave = (note - 2) / 12;
  switch (info.note_idx) {
  case 1:
  case 3:
  case 6:
  case 8:
  case 10:
    info.is_sharped = true;
    break;
  default:
    info.is_sharped = false;
    break;
  }
  return info;
}

static int GetNoteLineIdx(uint note_idx) {
  // 0 -----
  // 2 -----      C is under this
  // 4 -----
  // 6 -----
  // 8 -----      E

  NoteInfo info;

  switch (note_idx) {
  // C, C#
  case 0:
  case 1:
    return 3;
  // D, D#
  case 2:
  case 3:
    return 2;

  // E
  case 4:
    return 8;
  // F , F#
  case 5:
  case 6:
    return 7;
  // G, G#
  case 7:
  case 8:
    return 6;
  // A, A#
  case 9:
  case 10:
    return 5;
  // H
  case 11:
    return 4;
  default:
    std::abort();
  }
}

MainWindow::MainWindow()
    : sf::RenderWindow(sf::VideoMode({WINDOW_SIZE_X, WINDOW_SIZE_Y}), TITLE,
                       sf::Style::Titlebar | sf::Style::Close) {
  this->setSize(sf::Vector2u(WINDOW_SIZE_X, WINDOW_SIZE_Y));
  font.openFromFile("PixeloidMono.ttf");
  sharp_texture.loadFromFile("sharp.png");
}

void MainWindow::drawDocs() {
  sf::Text docText{font};
  docText.setString(DOCS);
  docText.setPosition({1, TRACK_GAP * 2 +
                             (TRACK_HEIGHT + TRACK_GAP) * TRACK_NUMBER});
  this->draw(docText);
}

void MainWindow::drawTrackAt(float x, float y, bool is_sample) {
  sf::RectangleShape trackInfo{sf::Vector2f(TRACK_INFO_LENGTH, TRACK_HEIGHT)};
  trackInfo.setFillColor(sf::Color::Black);
  trackInfo.setOutlineThickness(1);
  trackInfo.setOutlineColor(sf::Color::White);
  trackInfo.setPosition({x, y});
  this->draw(trackInfo);

  sf::Vertex noteLine[] = {
      sf::Vertex(sf::Vector2f(x + TRACK_INFO_LENGTH + TRACK_GAP, y)),
      sf::Vertex(sf::Vector2f(
          x + TRACK_INFO_LENGTH + TRACK_GAP + TRACK_CONTENT_LENGTH, y))};

  if (is_sample) {
    noteLine[0].position.y = y + (float)TRACK_HEIGHT / 2;
    noteLine[1].position.y = y + (float)TRACK_HEIGHT / 2;
    this->draw(noteLine, 2, sf::PrimitiveType::Lines);
  } else {
    for (int i = 0; i < 5; ++i) {
      noteLine[0].position.y += LINE_GAP;
      noteLine[1].position.y += LINE_GAP;
      this->draw(noteLine, 2, sf::PrimitiveType::Lines);
    }
  }
}

void MainWindow::drawPlayhead() {
  sf::RectangleShape head(
      sf::Vector2f(NOTE_SECTOR_WIDTH, TRACK_HEIGHT * TRACK_NUMBER));
  head.setFillColor(sf::Color::Green);
  head.setPosition({TRACK_INFO_LENGTH + Current_sector * NOTE_SECTOR_WIDTH, 1});
  this->draw(head);
}

void MainWindow::drawWTInfo() {
  Buffer *current = Buffers;
  float curPos = TRACK_GAP + (float)TRACK_HEIGHT / 2;
  for (int i = 0; i < TRACK_NUMBER; i++) {
    sf::Text WTText{font};
    WTText.setString(WT_NAMES.at(current->WT));
    WTText.setFillColor(sf::Color::White);
    WTText.setPosition({TRACK_GAP, curPos});
    this->draw(WTText);
    current++;
    curPos += TRACK_GAP + TRACK_HEIGHT;
  }
}

void MainWindow::drawEverything() {
  float trackPos = LINE_GAP;
  for (int i = 0; i < TRACK_NUMBER; ++i) {
    drawTrackAt(TRACK_GAP, trackPos, Buffers[i].is_sample);
    trackPos += (float)(TRACK_HEIGHT + TRACK_GAP);
  }

  drawWTInfo();
  drawDocs();
  drawPlayhead();

  Buffer *bufArray = Buffers;

  float bufY = TRACK_GAP;
  for (int i = 0; i < TRACK_NUMBER; ++i) {
    drawBufferNotes(&bufArray[i], TRACK_GAP + TRACK_INFO_LENGTH, bufY);
    bufY += TRACK_HEIGHT + TRACK_GAP;
  }
}

void MainWindow::drawBufferNotes(Buffer *buf, float bufX, float bufY) {
  auto notes = buf->note_ptr;
  std::vector<std::pair<float,float>> sharp_positions;

  float noteX = bufX;

  uint8_t lastNote = -1;
  for (int i = 0; i < BUFFER_SIZE_SECTORS; ++i) {
    if (notes[i] != 0) {
      sf::RectangleShape note(sf::Vector2f(NOTE_SECTOR_WIDTH, NOTE_HEIGHT));
      if (buf->is_sample) {
        note.setPosition({noteX,(float)(bufY + TRACK_HEIGHT / 2.0 + NOTE_HEIGHT / 2.0)});
        this->draw(note);
      } else {
	NoteInfo ni = GetNoteInfo(notes[i]);
	auto noteY = bufY + LINE_GAP * 1.4 +
                                       (LINE_GAP / 2) *
                                        GetNoteLineIdx(ni.note_idx);

        note.setPosition({noteX, (float)noteY});
        this->draw(note);
	if(ni.is_sharped && lastNote != notes[i]){
		sharp_positions.push_back({noteX, noteY});
	}
      }
    }
    lastNote = notes[i];
    noteX += NOTE_SECTOR_WIDTH;
  }


  sf::Sprite sharp{sharp_texture};
  for(auto sharp_pos : sharp_positions){
	  sharp.setPosition({sharp_pos.first, sharp_pos.second});
	  this->draw(sharp);

  }

}
