#ifndef _Common_Process_hpp
#define _Common_Process_hpp

#include "Defines_Libraries.hpp"

class CommonTools
{
	public:
		static std::vector<std::string> divide_by();
		static std::string convert_key_value_map_to_string(KeyValueMap key_values);
		static KeyValueMap convert_string_to_key_value_map(std::string data);
};

#endif