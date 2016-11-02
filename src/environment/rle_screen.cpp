/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details.
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * ******************************************************************************/

#include "../rle_interface.hpp"
#include "retro_environment.hpp"

namespace rle {

RLEScreen::RLEScreen(int h, int w):m_rows(h), m_columns(w), m_pixels(m_rows * m_columns){

	m_pixelFormat = new  pixelFormat();
}

RLEScreen::~RLEScreen(){
	delete m_pixelFormat;
}

RLEScreen::RLEScreen(const RLEScreen &rhs): m_rows(rhs.m_rows), m_columns(rhs.m_columns), m_pixels(rhs.m_pixels){

	m_pixelFormat = new  pixelFormat();
	m_pixelFormat->Bpp    = rhs.m_pixelFormat->Bpp   ;
	m_pixelFormat->rmask  = rhs.m_pixelFormat->rmask ;
	m_pixelFormat->gmask  = rhs.m_pixelFormat->gmask ;
	m_pixelFormat->bmask  = rhs.m_pixelFormat->bmask ;
	m_pixelFormat->amask  = rhs.m_pixelFormat->amask ;
    m_pixelFormat->rShift = rhs.m_pixelFormat->rShift;
 	m_pixelFormat->gShift = rhs.m_pixelFormat->gShift;
	m_pixelFormat->bShift = rhs.m_pixelFormat->bShift;
	m_pixelFormat->aShift = rhs.m_pixelFormat->aShift;

}

RLEScreen& RLEScreen::operator=(const RLEScreen &rhs) {



  m_rows = rhs.m_rows;
  m_columns = rhs.m_columns;
  m_pixels = rhs.m_pixels;
  m_pixelFormat->Bpp    = rhs.m_pixelFormat->Bpp   ;
  m_pixelFormat->rmask  = rhs.m_pixelFormat->rmask ;
  m_pixelFormat->gmask  = rhs.m_pixelFormat->gmask ;
  m_pixelFormat->bmask  = rhs.m_pixelFormat->bmask ;
  m_pixelFormat->amask  = rhs.m_pixelFormat->amask ;
  m_pixelFormat->rShift = rhs.m_pixelFormat->rShift;
  m_pixelFormat->gShift = rhs.m_pixelFormat->gShift;
  m_pixelFormat->bShift = rhs.m_pixelFormat->bShift;
  m_pixelFormat->aShift = rhs.m_pixelFormat->aShift;

  return *this;
}

bool RLEScreen::equals(const RLEScreen &rhs) const {
  return (m_rows == rhs.m_rows &&
          m_columns == rhs.m_columns &&
          (memcmp(&m_pixels[0], &rhs.m_pixels[0], arraySize()) == 0) );
}

int RLEScreen::getBpp()const{
	return m_pixelFormat->Bpp;
}

pixel_t RLEScreen::getRGBPixel(const uint32_t &pixel)const{

	struct pixelFormat* m = m_pixelFormat;
	uint32_t red   = (pixel & m->rmask) >> m->rShift;
	uint32_t green = (pixel & m->gmask) >> m->gShift;
	uint32_t blue  = (pixel & m->bmask) >> m->bShift;

	red   = red   << (m->rFill + 16); 	//solution commented out since previous agents were not using it
	green = green << (m->gFill+8);
	blue  = blue  <<  m->bFill;

	return red | green | blue;

}

void RLEScreen::getRGB(const uint32_t &pixel, uint8_t &red, uint8_t &green, uint8_t &blue)const{

	struct pixelFormat* m = m_pixelFormat;
	red   = (pixel & m->rmask) >> m->rShift;
	green = (pixel & m->gmask) >> m->gShift;
	blue  = (pixel & m->bmask) >> m->bShift;

	red   = red   << m->rFill; 	//solution commented out since previous agents were not using it
	green = green << m->gFill;
	blue  = blue  << m->bFill;
}

// pixel accessors, (row, column)-ordered
inline pixel_t RLEScreen::get(int r, int c) const {
  // Perform some bounds-checking
  assert (r >= 0 && r < m_rows && c >= 0 && c < m_columns);
  return m_pixels[r * m_columns + c];
}

inline pixel_t* RLEScreen::pixel(int r, int c) {
  // Perform some bounds-checking
  assert (r >= 0 && r < m_rows && c >= 0 && c < m_columns);
  return &m_pixels[r * m_columns + c];
}

// Access a whole row
inline pixel_t* RLEScreen::getRow(int r) const {
  assert (r >= 0 && r < m_rows);
  return const_cast<pixel_t*>(&m_pixels[r * m_columns]);
}

} // namespace rle
