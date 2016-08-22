#include "ale_c_wrapper.h"

#include <cstring>
#include <string>
#include <stdexcept>

//void encodeState(ALEState *state, char *buf, int buf_len) {
//	std::string str = state->serialize();
//
//	if (buf_len < int(str.length())) {
//		throw new std::runtime_error("Buffer is not big enough to hold serialized ALEState. Please use encodeStateLen to determine the correct buffer size");
//	}
//
//	memcpy(buf, str.data(), str.length());
//}
//
//int encodeStateLen(ALEState *state) {
//	return state->serialize().length();
//}
//
//ALEState *decodeState(const char *serialized, int len) {
//	std::string str(serialized, len);
//
//	return new ALEState(str);
//}


static void ale_rearrangeRgb(uint8_t *dst_buffer, const uint32_t *src_buffer, size_t src_size , const ALEInterface *ale) {

  uint8_t *q = dst_buffer;
  uint8_t r, g, b;
    for(size_t i = 0; i < src_size; ++i, ++q){
    	ale->getScreen().getRGB(((uint16_t*)src_buffer)[i], r, g, b);
        *q = (unsigned char) (r);  q++;    // r
        *q = (unsigned char) (g);  q++;    // g
        *q = (unsigned char) (b);  q++;    // b
    }
}

static void ale_rearrangeGrayscale(uint8_t *dst_buffer, const uint32_t *src_buffer, size_t src_size , const ALEInterface *ale) {

  uint8_t *q = dst_buffer;
  uint8_t r, g, b;
    for(size_t i = 0; i < src_size; ++i, ++q){
    	ale->getScreen().getRGB(((uint16_t*)src_buffer)[i], r, g, b);
        *q = (unsigned char) (0.3*r + 0.5*g+0.2*b);
    }
}

void getScreenRGB(ALEInterface *ale, unsigned char *output_buffer){
	  const ale::ALEScreen& screen = ale->getScreen();
  size_t w = screen.width();
  size_t h = screen.height();
  size_t screen_size = w*h;
  size_t size_in_pixels = screen_size / screen.getBpp();
  ale_rearrangeRgb(output_buffer, (uint32_t*)screen.getArray(), size_in_pixels, ale);
}

//This method should receive an empty vector to fill it with
//the grayscale colours
void getScreenGrayscale(ALEInterface *ale, unsigned char *output_buffer){
  const ale::ALEScreen& screen = ale->getScreen();
  size_t w = ale->getScreen().width();
  size_t h = ale->getScreen().height();
  size_t screen_size = w*h;
  size_t size_in_pixels = screen_size ;
  ale_rearrangeGrayscale(output_buffer, (uint32_t*)screen.getArray(), size_in_pixels, ale);
}
