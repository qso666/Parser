#include <iostream>
#include <any>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <exception>

class JSON {
	std::map<std::string, std::any> object;
	std::vector<std::any> array;
public:
	JSON(); // Default constructor

	// Constructor with parameters
	JSON(const std::string& s, int& position, bool isFile);

	bool is_array();

	bool is_object();

	// Value by key in object
	std::any& operator[](const std::string& key);

	// Value by index in array
	std::any& operator[](int index);

	// String value processing
	static std::string parseString(const std::string& s, int& position);

	// Bool value processing
	static bool parseBool(const std::string& s, int& position);

	// Number value processing
	static double parseNumber(const std::string& s, int& position);

	// Object processing
	static std::map<std::string, std::any> parseObject(const std::string& s, int& position);

	// Array processing
	static std::vector<std::any> parseArray(const std::string& s, int& position);

	// Input line processing
	void parse(const std::string& s, int& position);

	// Input file processing
	void parseFile(const std::string& path);
};

