#include "CConfig.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace SocialQuantumTest;

CConfig::CConfig()
	: mResolution(RES_640x480)
	, mFullScreenMode(true)
	, mAntialiasing(MULTISAMPLE_NONE)
	, mViewDistance(500)
	, mGraphicsQuality(NORMAL)
	, mTextureQuality(MEDIUM)
	, mDynamicLighting(false)
	, mShadowsQuality(MEDIUM)
	, mOutputFormatType("")
	, mChangeHistoryIndex(-1){
} // END CConfig

CConfig::~CConfig() {
	mChangeHistory.clear();
} // END ~CConfig

void CConfig::load() {
	if (isConfigFileExists("./config.xml")) {
		loadXMLFile(std::string("./config.xml"));
	}
	else if (isConfigFileExists("./config.bin")) {
		loadBinFile(std::string("./config.bin"));
	}
	else{
		// default settings
		setResolution(std::string(RES_640x480));
	}
} // END load

void CConfig::setResolution(std::string& resolution) {
	if (resolution.compare(RES_640x480) == 0 ||
		resolution.compare(RES_1024x768) == 0 ||
		resolution.compare(RES_1920x1080) == 0)
	{
		// delete forward history
		if (this->mChangeHistory.size()>0 && this->mChangeHistoryIndex<(this->mChangeHistory.size()-1)) {
			int count = this->mChangeHistory.size() - 1 - mChangeHistoryIndex;
			this->mChangeHistory.erase(	this->mChangeHistory.begin() + this->mChangeHistoryIndex + 1, 
										this->mChangeHistory.begin() + this->mChangeHistoryIndex + 1 +count);
		}

		this->mChangeHistory.push_back(resolution);
		this->mChangeHistoryIndex++;
		this->mResolution = resolution;
	}
	else {
		std::string errorMsg;
		errorMsg.resize(_MAX_PATH);
		sprintf(&errorMsg[0], "Error: '%s' unsupported resolutution.", resolution.c_str());
		errorMsg.resize(strlen(errorMsg.data()) + 1);

		throw errorMsg;
	}
} //END setResolution

void CConfig::setFullscreenMode(std::string& isFullscreenMode) {
	std::transform(isFullscreenMode.begin(), isFullscreenMode.end(), isFullscreenMode.begin(), ::tolower);
	if (isFullscreenMode.compare("true") == 0) {
		this->mFullScreenMode = true;
	}
	else if (isFullscreenMode.compare("false") == 0) {
		this->mFullScreenMode = false;
	}
	else {
		throw std::string("Error: unsupported value\n");
	}
} //END setFullscreenMode

void CConfig::setAntialiasing(std::string& antialiasing) {
	std::transform(antialiasing.begin(), antialiasing.end(), antialiasing.begin(), ::toupper);
	if (antialiasing.compare(MULTISAMPLE_NONE) == 0 ||
		antialiasing.compare(MULTISAMPLE_2) == 0 ||
		antialiasing.compare(MULTISAMPLE_4) == 0)
	{
		this->mAntialiasing = antialiasing;
	}
	else {
		throw std::string("Error: unsupported value\n");
	}
} //END setAntialiasing

void CConfig::setViewDistance(int viewDistance) {
	if (viewDistance >= 100 && viewDistance <= 1000) {
		this->mViewDistance = viewDistance;
	}
	else {
		throw std::string("Invalid 'view_distance' setting value. The supported range of numbers is from 100 to 1000.\n");
	}
} //END setViewDistance

void CConfig::setTextureQuality(std::string& textureQuality) {
	std::transform(textureQuality.begin(), textureQuality.end(), textureQuality.begin(), ::tolower);
	if (textureQuality.compare(LOW) == 0 ||
		textureQuality.compare(MEDIUM) == 0 ||
		textureQuality.compare(HIGH) == 0)
	{
		this->mTextureQuality = textureQuality;
		this->mGraphicsQuality = CUSTOM;
	}
	else {
		throw std::string("Error: unsupported value\n");
	}
} //END setTextureQuality

void CConfig::setDynamicLighting(std::string&  isDynamicLighting) {
	std::transform(isDynamicLighting.begin(), isDynamicLighting.end(), isDynamicLighting.begin(), ::tolower);
	if (isDynamicLighting.compare("true") == 0) {
		this->mDynamicLighting = true;
	}
	else if (isDynamicLighting.compare("false") == 0) {
		this->mDynamicLighting = false;
	}
	else {
		throw std::string("Error: unsupported value\n");
	}
} //END setDynamicLighting

void CConfig::setShadowsQuality(std::string&  shadowsQuality) {
	std::transform(shadowsQuality.begin(), shadowsQuality.end(), shadowsQuality.begin(), ::tolower);
	if (shadowsQuality.compare(LOW) == 0 ||
		shadowsQuality.compare(MEDIUM) == 0 ||
		shadowsQuality.compare(HIGH) == 0)
	{
		this->mShadowsQuality = shadowsQuality;
		this->mGraphicsQuality = CUSTOM;
	}
	else {
		throw std::string("Error: unsupported value\n");
	}
} //END setShadowsQuality

void CConfig::setGraphicsQuality(std::string&  graphicsQuality) {
	this->mGraphicsQuality = graphicsQuality;
	std::transform(graphicsQuality.begin(), graphicsQuality.end(), graphicsQuality.begin(), ::tolower);
	if (graphicsQuality.compare(LOW) == 0) {
		this->mTextureQuality = LOW;
		this->mDynamicLighting = false;
		this->mShadowsQuality = LOW;
	}
	else if (graphicsQuality.compare(NORMAL) == 0) {
		this->mTextureQuality = MEDIUM;
		this->mDynamicLighting = false;
		this->mShadowsQuality = MEDIUM;
	}
	else if (graphicsQuality.compare(HIGH) == 0) {
		this->mTextureQuality = HIGH;
		this->mDynamicLighting = true;
		this->mShadowsQuality = HIGH;
	}
	else {
		throw std::string("Error: unsupported value\n");
	}
} //END setGraphicsQuality

void CConfig::format(std::string& outputFormatType) {
	std::transform(outputFormatType.begin(), outputFormatType.end(), outputFormatType.begin(), ::tolower);
	if (outputFormatType.compare(XML) == 0) {
		this->mOutputFormatType = XML;
	}
	else if (outputFormatType.compare(BIN) == 0) {
		this->mOutputFormatType = BIN;
	}
	else {
		throw std::string("Error: unsupported output format\n");
	}
} //END format

void CConfig::save() {
	if (this->mOutputFormatType.compare(XML) == 0) {
		// delete BIN config file
		if (remove("./config.bin") != 0) {
			//perror("Error deleting file\n");
		}

		saveXmlFile(std::string("./config.xml"));
	}
	else if (this->mOutputFormatType.compare(BIN) == 0) {
		// delete XML config file
		if (remove("./config.xml") != 0) {
			//perror("Error deleting file\n");
		}

		saveBinFile(std::string("./config.bin"));
	}
	else 
		throw std::string("Error: please specify output file type by calling command format {xml|bin}.\n");
} // END save

void CConfig::undo() {
	if (mChangeHistoryIndex > 0) {
		mChangeHistoryIndex--;
		mResolution = mChangeHistory[mChangeHistoryIndex];
	}
} //END undo

void CConfig::redo() {
	if (mChangeHistoryIndex < (mChangeHistory.size()-1)) {
		mChangeHistoryIndex++;
		mResolution = mChangeHistory[mChangeHistoryIndex];
	}
} //END redo

bool CConfig::isConfigFileExists(const std::string& filename){
	if (FILE *file = fopen(filename.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
} // END isConfigFileExists

std::string CConfig::getResolution() {
	return this->mResolution;
} // END getResolution

bool CConfig::getFullscreenMode() {
	return this->mFullScreenMode;
} // END getFullscreenMode

std::string CConfig::getAntialiasing() {
	return this->mAntialiasing;
} // END getAntialiasing

int CConfig::getViewDistance() {
	return this->mViewDistance;
} // END getAntialiasing

std::string CConfig::getTextureQuality() {
	return this->mTextureQuality;
} // END getTextureQuality

bool CConfig::getDynamicLighting() {
	return this->mDynamicLighting;
} // END getDynamicLighting

std::string CConfig::getShadowsQuality() {
	return this->mShadowsQuality;
} // END getShadowsQuality

std::string CConfig::getGraphicsQuality() {
	return this->mGraphicsQuality;
} //END getGraphicsQuality

void CConfig::loadBinFile(std::string& filename) {
	std::ifstream in(filename, std::ios::binary | std::ios::in);
	if (in.is_open()){
		std::string resolution;
		std::string::size_type sz = resolution.size();
		in.read((char*) (&sz), sizeof(std::string::size_type));
		resolution.resize(sz);
		in.read(&resolution[0], sz);
		setResolution(resolution);

		sz = mAntialiasing.size();
		in.read((char*) (&sz), sizeof(std::string::size_type));
		mAntialiasing.resize(sz);
		in.read(&mAntialiasing[0], sz);

		in.read((char*) &this->mFullScreenMode, sizeof(bool));
		in.read((char*) &this->mViewDistance, sizeof(int));

		sz = mTextureQuality.size();
		in.read((char*) (&sz), sizeof(std::string::size_type));
		mTextureQuality.resize(sz);
		in.read(&mTextureQuality[0], sz);

		in.read((char*) &this->mDynamicLighting, sizeof(bool));

		sz = mShadowsQuality.size();
		in.read((char*) (&sz), sizeof(std::string::size_type));
		mShadowsQuality.resize(sz);
		in.read(&mShadowsQuality[0], sz);

		sz = mGraphicsQuality.size();
		in.read((char*) (&sz), sizeof(std::string::size_type));
		mGraphicsQuality.resize(sz);
		in.read(&mGraphicsQuality[0], sz);

		in.close();

		mOutputFormatType = BIN;
	}
} //END loadBinFile

void CConfig::saveBinFile(std::string& filename) {
	std::ofstream out(filename.c_str(), std::ios::binary | std::ios::out);
	if (out.is_open()) {
		std::string::size_type sz = this->mResolution.size();
		out.write((char*) (&sz), sizeof(std::string::size_type));
		out.write(this->mResolution.data(), sz);

		sz = this->mAntialiasing.size();
		out.write((char*) (&sz), sizeof(std::string::size_type));
		out.write(this->mAntialiasing.data(), sz);

		out.write((char*) &this->mFullScreenMode, sizeof(bool));
		out.write((char*) &this->mViewDistance, sizeof(int));

		sz = this->mTextureQuality.size();
		out.write((char*) (&sz), sizeof(std::string::size_type));
		out.write(this->mTextureQuality.data(), sz);

		out.write((char*) &this->mDynamicLighting, sizeof(bool));

		sz = this->mShadowsQuality.size();
		out.write((char*) (&sz), sizeof(std::string::size_type));
		out.write(this->mShadowsQuality.data(), sz);

		sz = this->mGraphicsQuality.size();
		out.write((char*) (&sz), sizeof(std::string::size_type));
		out.write(this->mGraphicsQuality.data(), sz);

		out.close();
	}
} //END saveBinFile

void CConfig::loadXMLFile(std::string& filename) {
	TiXmlDocument doc(filename.c_str());
	bool loadOkay = doc.LoadFile();
	if (loadOkay){
		TiXmlElement* root = doc.FirstChildElement("SocialQuantumTest");
		if (root){
			std::string resolution = root->Attribute("resolution");
			setResolution(resolution);

			root->QueryBoolAttribute("full-screen_mode", &mFullScreenMode);

			mAntialiasing = root->Attribute("antialiasing");
			std::transform(mAntialiasing.begin(), mAntialiasing.end(), mAntialiasing.begin(), ::toupper);

			root->QueryIntAttribute("view_distance", &mViewDistance);

			mTextureQuality = root->Attribute("texture_quality");
			std::transform(mTextureQuality.begin(), mTextureQuality.end(), mTextureQuality.begin(), ::tolower);

			root->QueryBoolAttribute("dynamic_lighting", &mDynamicLighting);

			mShadowsQuality = root->Attribute("shadows_quality");
			std::transform(mShadowsQuality.begin(), mShadowsQuality.end(), mShadowsQuality.begin(), ::tolower);

			mGraphicsQuality = root->Attribute("graphics_quality");
			std::transform(mGraphicsQuality.begin(), mGraphicsQuality.end(), mGraphicsQuality.begin(), ::tolower);

			mOutputFormatType = XML;
		}
	}
} //END loadXMLFile

void CConfig::saveXmlFile(std::string& filename) {
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement* root = new TiXmlElement("SocialQuantumTest");
	doc.LinkEndChild(root);

	root->SetAttribute("resolution", mResolution.c_str());
	root->SetAttribute("full-screen_mode", mFullScreenMode == true ? "true" : "false");
	root->SetAttribute("antialiasing", mAntialiasing.c_str());
	root->SetAttribute("view_distance", mViewDistance);
	root->SetAttribute("texture_quality", mTextureQuality.c_str());
	root->SetAttribute("dynamic_lighting", mDynamicLighting == true ? "true" : "false");
	root->SetAttribute("shadows_quality", mShadowsQuality.c_str());
	root->SetAttribute("graphics_quality", mGraphicsQuality.c_str());

	doc.SaveFile(filename.c_str());
} //END saveXmlFile