#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <algorithm>
#include "CConfig.h"

using namespace SocialQuantumTest;

std::vector<std::string> split(const std::string& text)
{
	std::vector<std::string> tokens;
	std::size_t start = text.find_first_not_of(" "), end = 0;

	while ((end = text.find_first_of(" ", start)) != std::string::npos)
	{
		tokens.push_back(text.substr(start, end - start));
		start = text.find_first_not_of(" ", end);
	}

	if (start != std::string::npos)
		tokens.push_back(text.substr(start));

	return tokens;
} //END split

void change(CConfig* config, std::string& settingName, std::string& settingValue) {
	std::transform(settingValue.begin(), settingValue.end(), settingValue.begin(), ::tolower);
	try {
		if (settingName.compare(RESOLUTION) == 0)
			config->setResolution(settingValue);
		else if (settingName.compare(FULLSCREEN_MODE) == 0)
			config->setFullscreenMode(settingValue);
		else if (settingName.compare(ANTIALIASING) == 0)
			config->setAntialiasing(settingValue);
		else if (settingName.compare(VIEW_DISTANCE) == 0) {
			std::string::size_type sz;  // alias of size_t
			int viewDistance = std::stoi(settingValue, &sz);
			config->setViewDistance(viewDistance);
		}
		else if (settingName.compare(TEXTURE_QUALITY) == 0)
			config->setTextureQuality(settingValue);
		else if (settingName.compare(DYNAMIC_LIGHTING) == 0)
			config->setDynamicLighting(settingValue);
		else if (settingName.compare(SHADOWS_QUALITY) == 0)
			config->setShadowsQuality(settingValue);
		else if (settingName.compare(GRAPHICS_QUALITY) == 0)
			config->setGraphicsQuality(settingValue);
		else
			printf("Error: unrecognized setting name '%s.'\n", settingName.c_str());
	}
	catch (std::string error){
		printf("%s\n", error.c_str());
	}
} //END change

std::string get(CConfig* config, std::string& settingName) {
	std::transform(settingName.begin(), settingName.end(), settingName.begin(), ::tolower);
	if (settingName.compare(RESOLUTION) == 0)
		return config->getResolution();
	else if (settingName.compare(FULLSCREEN_MODE) == 0)
		return config->getFullscreenMode() == true ? std::string("true") : std::string("false");
	else if (settingName.compare(ANTIALIASING) == 0)
		return config->getAntialiasing();
	else if (settingName.compare(VIEW_DISTANCE) == 0)
		return std::to_string(config->getViewDistance());
	else if (settingName.compare(TEXTURE_QUALITY) == 0)
		return config->getTextureQuality();
	else if (settingName.compare(DYNAMIC_LIGHTING) == 0)
		return config->getDynamicLighting() == true ? std::string("true") : std::string("false");
	else if (settingName.compare(SHADOWS_QUALITY) == 0)
		return config->getShadowsQuality();
	else if (settingName.compare(GRAPHICS_QUALITY) == 0)
		return config->getGraphicsQuality();
	throw std::string("Invalid 'setting name'.\n");
} //END get

void print(CConfig* config) {
	printf("%s=%s\n", RESOLUTION, config->getResolution().c_str());
	printf("%s=%s\n", FULLSCREEN_MODE, config->getFullscreenMode() == true ? "true" : "false");
	printf("%s=%s\n", ANTIALIASING, config->getAntialiasing().c_str());
	printf("%s=%d\n", VIEW_DISTANCE, config->getViewDistance());
	printf("%s=%s\n", TEXTURE_QUALITY, config->getTextureQuality().c_str());
	printf("%s=%s\n", DYNAMIC_LIGHTING, config->getDynamicLighting() == true ? "true" : "false");
	printf("%s=%s\n", SHADOWS_QUALITY, config->getShadowsQuality().c_str());
	printf("%s=%s\n", GRAPHICS_QUALITY, config->getGraphicsQuality().c_str());
} //END print

void main() {
	std::string command;
	bool exitProgram = false;
	CConfig* config = new CConfig();
	config->load();

	while (!exitProgram) {
		std::cout << "Please, enter command: ";
		std::getline(std::cin, command);
		std::vector<std::string> tokens = split(command);
		if (tokens.size()>=1 && tokens.size() <= 3) {
			try {
				std::transform(tokens[0].begin(), tokens[0].end(), tokens[0].begin(), ::tolower);
				if (tokens[0].compare(CHANGE) == 0) {
					if (tokens.size() == 3)
						change(config, tokens[1], tokens[2]);
					else
						printf("Error: Not enough arguments\n");
				}
				else if (tokens[0].compare(GET) == 0) {
					if (tokens.size() == 2) {
						std::string value = get(config, tokens[1]);
						printf("%s", value.c_str());
					}
					else
						printf("Error: Not enough arguments\n");
				}
				else if (tokens[0].compare(PRINT) == 0) {
					if (tokens.size() == 1)
						print(config);
					else
						printf("Error: too many arguments\n");
				}
				else if (tokens[0].compare(FORMAT) == 0) {
					if (tokens.size() == 2)
						config->format(tokens[1]);
					else
						printf("Error: Not enough arguments\n");
				}
				else if (tokens[0].compare(SAVE) == 0) {
					if (tokens.size() == 1)
						config->save();
					else
						printf("Error: too many arguments\n");
				}
				else if (tokens[0].compare(EXIT) == 0) {
					if (tokens.size() == 1)
						exitProgram = true;
					else
						printf("Error: too many arguments\n");
				}
				else if (tokens[0].compare(UNDO) == 0) {
					if (tokens.size() == 1)
						config->undo();
					else
						printf("Error: too many arguments\n");
				}
				else if (tokens[0].compare(REDO) == 0) {
					if (tokens.size() == 1)
						config->redo();
					else
						printf("Error: too many arguments\n");
				}
				else
					printf("Error: '%s' unsupported command.\n", tokens[0].c_str());
				printf("\n");
			}
			catch (std::string error) {
				printf("%s", error.c_str());
			}
		}
		else if (tokens.size() > 3) {
			printf("Error: too many arguments\n");
		}	
		else {
			printf("Error: wrong command\n");
		}
	}

	delete config;
} //END main