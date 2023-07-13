#pragma once

#include <string>

class TextMessage {

	// A message which contains all the state needed to output a line of text
	// We use the propertied object pattern

	protected:

		std::string text;

	public:

		TextMessage(const std::string & t) {
		    text = t;	
		}

		const std::string & get_text() { return text; }

};
