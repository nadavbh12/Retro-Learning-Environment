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



static void ale_rearrangeRgb(uint8_t *output_buffer, const uint32_t *screenArray, size_t obs_size , const ALEInterface *ale) {

  const int r_offset = 0ul;
  const int g_offset = obs_size;
  const int b_offset = 2ul * obs_size;
  uint8_t r, g, b;
  for (int index = 0ul; index < obs_size; ++index) {
	ale->getScreen().getRGB(((uint16_t*)screenArray)[index], r, g, b);
	output_buffer[r_offset + index] = r;
	output_buffer[g_offset + index] = g;
	output_buffer[b_offset + index] = b;
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
