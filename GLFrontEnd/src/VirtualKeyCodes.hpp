/*
 * VirtualKeyCodes.hpp
 *
 * Designed and written by
 *	 Shvedov Yury 321 group.
 *
 * Powered by
 * 	 MediaLab, CMC MSU
 *
 * 21.11.2012
 */

#ifndef _GLFrontEnd_VIRTUAL_KEY_CODES_HPP_
#define _GLFrontEnd_VIRTUAL_KEY_CODES_HPP_

#ifdef __linux__

#define _KEY_W 25
#define _KEY_S 39
#define _KEY_A 38
#define _KEY_D 40
#define _KEY_SPACE 65
#define _KEY_CTRL 37

#elif defined _WIN32 || defined _WIN64

#define _KEY_W 87
#define _KEY_S 83
#define _KEY_A 65
#define _KEY_D 68
#define _KEY_SPACE 32
#define _KEY_CTRL 17

#endif

#endif //_GLFrontEnd_VIRTUAL_KEY_CODES_HPP_