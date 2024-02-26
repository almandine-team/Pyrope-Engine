#pragma once

#include "string"
#include "cstring"
#include "map"
#include "sstream"

const std::string sections[]
{
	"Item",
	"Object",
	"Folder",
	"Entities",
	"Properties",
}; // Feel free to implement your own documentation through the customSections array.

const std::string customSections[]{ " " };

const std::string DOCERROR = "This feature was either mispelled, not documented yet, not implemented, or named differently.\nIf this feature does exist, and is not documented or named differently and you believe it should be named differently,\nplease file and issue or pull request at:\nhttps://github.com/Pyrope-Engine/Pyrope-Engine.\nThank you!";

std::string give_sections() {
	std::string strSec = """Sections: """;

	for (size_t index = 0; index < sections->length(); index++) {
		strSec += '\n';
		strSec += '(' + index + ') ' + sections[index].c_str(); // Might change (add fmt string library.. (https://github.com/fmtlib/fmt))
	}

	strSec += "Custom documentation, if any: ";
	strSec += '\n' + "----------------------------";

	for (size_t index = 0; index < customSections->length(); index++) {
		strSec += '\n';
		strSec += sections[index].c_str();
	}

	return strSec;
}

size_t find(std::string section) {
	bool found = false;
	size_t index = 0;
	while (index < sections->size()) {
		index++;
		if (sections[index] == section) {
			found = true;
			break;
		}
	}

	if (!found) {
		return sections->length() - 1; //DOCERROR
	}
	return index;
}

std::string documentation(std::string section) {
	switch (find(section)) {
		case 1: // Item
			return "This is an item!";
			break;
		default:
			return DOCERROR;
			break;
	}
	//return DOCERROR;
}