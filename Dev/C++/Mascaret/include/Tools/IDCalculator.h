/*
 * CERV/ENIB -- Tools
 * ----------------------------------------------------------------------------
 * file        : IDCalculator.h
 * author(s)   : Nicolas MARION <marion@enib.fr>
 * date        : 2010-06-08
 * ----------------------------------------------------------------------------
 * Description : Description
 * ----------------------------------------------------------------------------
 */

#ifndef _v2_Tools_IDCalculator_h_
#define _v2_Tools_IDCalculator_h_

#include "Tools/veha_plateform.h"
#include "Tools/md5.h"
#include <string.h>

class VEHA_API IDCalculator {
public:

	static std::string calculateID(std::string prefix, std::string name) {
		md5_state_t state;
		md5_byte_t digest[16];
		char hex_output[16*2 + 1];
		int di;

		md5_init(&state);
		md5_append(&state, (const md5_byte_t *)name.c_str(), strlen(name.c_str()));
		md5_finish(&state, digest);
		for (di = 0; di < 16; ++di)
			sprintf(hex_output + di * 2, "%02x", digest[di]);
		//   cerr << "Digest : " << hex_output << endl;
		std::string str = prefix+"-"+hex_output;
		return str;
	}
};




#endif // _v2_Tools_IDCalculator_h_
