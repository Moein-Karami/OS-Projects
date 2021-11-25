#include "Common_Tools.hpp"

std::vector<std::string> CommonTools::divide_by(std::string data, char divider)
{
	std::vector<std::string> words;
	std::string word;
	for (auto letter : data)
	{
		if (letter == divider)
		{
			if (word.size())
				words.push_back(word);
			word.clear();
		}
		else
			word += letter;
	}
	if (word.size())
		words.push_back(word);
	return words;
}

std::string CommonTools::convert_key_value_map_to_string(KeyValueMap key_value)
{
	std::string data;
	for (auto element : key_value)
		data += element.key + COMMA + std::to_string(element.value) + COMMA;
	if (data.size())
		data.pop_back();
	return data;
}

KeyValueMap CommonTools::convert_string_to_key_value_map(std::string data)
{
	KeyValueMap key_value;
	std::vector<std::string> words = divide_by(data, COMMA);
	for (int i = 0 ; i < words.size(); i += 2)
		key_value[words[i]] += std::stoi(words[i+1]);

	return key_value;
}

