#include "json.h"

JSON::JSON() = default;

JSON::JSON(const std::string& s, int& position, bool isFile) {
	if (isFile) {
		parseFile(s);
	}
	else {
		parse(s, position);
	}
}

bool JSON::is_array() {
	if (array.empty()) {
		return false;
	}
	else return true;
}

bool JSON::is_object() {
	if (array.empty()) {
		return true;
	}
	else return false;
}

std::any& JSON::operator[](const std::string& key) {
	if (!is_object()) {
		throw std::logic_error("\nNot object\n");
	}
	else return object[key];
}

std::any& JSON::operator[](int index) {
	if (!is_array()) {
		throw std::logic_error("\nNot array\n");
	}
	else return array[index];
}

std::string JSON::parseString(const std::string& s, int& position) {
	std::string result;
	position += 1;
	for (; position < s.size(); position++) {
		if (s[position] != '"') {
			result += s[position];
		}
		else {
			return result;
		}
	}
	throw std::logic_error("Invalid input format\n");
}

bool JSON::parseBool(const std::string& s, int& position) {
	bool result = true;
	bool isBool = true;
	if (s[position] == 't') {
		position += 1;
		if (s[position] == 'r') {
			position += 1;
			if (s[position] == 'u') {
				position += 1;
				if (s[position] == 'e') {
					result = true;
				}
				else isBool = false;
			}
			else isBool = false;
		}
		else isBool = false;
	}
	else {
		position += 1;
		if (s[position] == 'a') {
			position += 1;
			if (s[position] == 'l') {
				position += 1;
				if (s[position] == 's') {
					position += 1;
					if (s[position] == 'e') {
						result = false;
					}
					else isBool = false;
				}
				else isBool = false;
			}
			else isBool = false;
		}
		else isBool = false;
	}
	if (isBool) {
		return result;
	}
	else throw std::logic_error("Invalid input format\n");
}

double JSON::parseNumber(const std::string& s, int& position) {
	std::string number;
	number += s[position];
	position += 1;
	for (; position < s.size(); position++) {
		if (iswdigit(s[position]) || s[position] == '.') {
			number += s[position];
		}
		else {
			if (s[position] == ' ' || s[position] == ',' || s[position] == ']' || s[position] == '}') {
				position -= 1;
				return stod(number);
			}
			else throw std::logic_error("Invalid input format\n");
		}
	}
}

std::map<std::string, std::any> JSON::parseObject(const std::string& s, int& position) {
	std::map<std::string, std::any> result;
	std::any value;
	std::string key;
	int stateObject = 0;
	position += 1;
	for (; position < s.size(); position++) {
		if (s[position] == '"') {
			if (stateObject == 0) {
				key = parseString(s, position);
				stateObject = 1;
				continue;
			}
			if (stateObject == 2) {
				value = parseString(s, position);
				result.insert(std::pair<std::string, std::any>(key, value));
				stateObject = 3;
				continue;
			}
			else throw std::logic_error("Invalid input format\n");
		}
		if (s[position] == ':') {
			if (stateObject == 1) {
				stateObject = 2;
				continue;
			}
			else throw std::logic_error("Invalid input format\n");
		}
		if (s[position] == '}') {
			if (stateObject == 0 or stateObject == 3) {
				return result;
			}
			else throw std::logic_error("Invalid input format\n");
		}
		if (s[position] == ',') {
			if (stateObject == 3) {
				stateObject = 0;
				continue;
			}
			else throw std::logic_error("Invalid input format\n");
		}
		if (s[position] == '{') {
			if (stateObject == 2) {
				JSON buf(s, position, false);
				result.insert(std::pair<std::string, std::any>(key, buf));
				stateObject = 3;
				continue;
			}
			else throw std::logic_error("Invalid input format\n");
		}
		if (s[position] == '[') {
			if (stateObject == 2) {
				JSON buf(s, position, false);
				result.insert(std::pair<std::string, std::any>(key, buf));
				stateObject = 3;
				continue;
			}
			else throw std::logic_error("Invalid input format\n");
		}
		if (s[position] == 't' || s[position] == 'f') {
			if (stateObject == 2) {
				value = parseBool(s, position);
				result.insert(std::pair<std::string, std::any>(key, value));
				stateObject = 3;
				continue;
			}
			else throw std::logic_error("Invalid input format\n");
		}
		if (iswdigit(s[position])) {
			if (stateObject == 2) {
				value = parseNumber(s, position);
				result.insert(std::pair<std::string, std::any>(key, value));
				stateObject = 3;
				continue;
			}
			else throw std::logic_error("Invalid input format\n");
		}
	}
	throw std::logic_error("Invalid input format\n");
}

std::vector<std::any> JSON::parseArray(const std::string& s, int& position) {
	std::vector<std::any> result;
	std::any value;
	int stateArray = 0;
	/*
	 state = {
	 0 - Looking for a value or end;
	 1 - Looking for a comma or end;
	 }*/
	position += 1;
	for (; position < s.size(); position++) {
		if (s[position] == '{') {
			if (stateArray == 0) {
				JSON buf(s, position, false);
				result.push_back(buf);
				stateArray = 1;
				continue;
			}
			else throw std::logic_error("Invalid input format\n");
		}
		if (s[position] == '[') {
			if (stateArray == 0) {
				JSON buf(s, position, false);
				result.push_back(buf);
				stateArray = 1;
				continue;
			}
			else throw std::logic_error("Invalid input format\n");
		}
		if (s[position] == '"') {
			if (stateArray == 0) {
				value = parseString(s, position);
				result.push_back(value);
				stateArray = 1;
				continue;
			}
			else throw std::logic_error("Invalid input format\n");
		}
		if (s[position] == ',') {
			if (stateArray == 1) {
				stateArray = 0;
				continue;
			}
			else throw std::logic_error("Invalid input format\n");
		}
		if (iswdigit(s[position])) {
			if (stateArray == 0) {
				value = parseNumber(s, position);
				result.push_back(value);
				stateArray = 1;
				continue;
			}
			else throw std::logic_error("Invalid input format\n");
		}
		if (s[position] == 't' || s[position] == 'f') {
			if (stateArray == 0) {
				value = parseBool(s, position);
				result.push_back(value);
				stateArray = 1;
				continue;
			}
			else throw std::logic_error("Invalid input format\n");
		}
		if (s[position] == ']') {
			if (stateArray == 1 or stateArray == 0) {
				return result;
			}
		}
	}
	throw std::logic_error("Invalid input format\n");
}

void JSON::parse(const std::string& s, int& position) {
	for (; position < s.size(); position++) {
		if (s[position] == '{') {
			object = parseObject(s, position);
			return;
		}
		if (s[position] == '[') {
			array = parseArray(s, position);
			return;
		}
	}
	throw std::logic_error("Invalid input format\n");
}

void JSON::parseFile(const std::string& path) {
	std::ifstream JSON_file(path);
	std::string line, file;
	if (JSON_file.is_open()) {
		while (!JSON_file.eof()) {
			getline(JSON_file, line);
			file += line;
		}
		int position = 0;
		parse(file, position);
		return;
	}
	else throw std::logic_error("File does not exist\n");
}