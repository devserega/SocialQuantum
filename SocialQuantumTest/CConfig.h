#ifndef _CCONFIG_
#define _CCONFIG_ 

#include <string>
#include <vector>
#include "tinyxml/tinyxml.h"

namespace SocialQuantumTest {
	#define RES_640x480 "640*480"
	#define RES_1024x768 "1024*768"
	#define RES_1920x1080 "1920*1080"
	#define LOW "low" 
	#define NORMAL "normal" 
	#define MEDIUM "medium" 
	#define HIGH "high"
	#define CUSTOM "custom" 
	#define MULTISAMPLE_NONE "MULTISAMPLE_NONE" 
	#define MULTISAMPLE_2 "MULTISAMPLE_2"
	#define MULTISAMPLE_4 "MULTISAMPLE_4"
	#define BIN "bin"
	#define XML "xml" 

	#define RESOLUTION "resolution" 
	#define FULLSCREEN_MODE "full-screen_mode" 
	#define ANTIALIASING "antialiasing" 
	#define VIEW_DISTANCE "view_distance" 
	#define TEXTURE_QUALITY "texture_quality" 
	#define DYNAMIC_LIGHTING "dynamic_lighting" 
	#define SHADOWS_QUALITY "shadows_quality" 
	#define GRAPHICS_QUALITY "graphics_quality" 

	#define GET "get" 
	#define CHANGE "change" 
	#define PRINT "print" 
	#define FORMAT "format" 
	#define SAVE "save" 
	#define EXIT "exit" 
	#define UNDO "undo" 
	#define REDO "redo" 

	class CConfig {
		public:
			CConfig();
			virtual ~CConfig();
			void setResolution(std::string& resolution);
			void setFullscreenMode(std::string& isFullscreenMode);
			void setAntialiasing(std::string& antialiasing);
			void setViewDistance(int viewDistance);
			void setTextureQuality(std::string& textureQuality);
			void setDynamicLighting(std::string& isDynamicLighting);
			void setShadowsQuality(std::string& shadowsQuality);
			void setGraphicsQuality(std::string& graphicsQuality);

			std::string getResolution();
			bool getFullscreenMode();
			std::string getAntialiasing();
			int getViewDistance();
			std::string getTextureQuality();
			bool getDynamicLighting();
			std::string getShadowsQuality();
			std::string getGraphicsQuality();

			void format(std::string& outputFormatType);
			void save();
			void undo();
			void redo();
			void load();

		protected:
			std::string mResolution;
			bool mFullScreenMode;
			std::string mAntialiasing;
			int mViewDistance;
			std::string mTextureQuality;
			bool mDynamicLighting;
			std::string mShadowsQuality;
			std::string mGraphicsQuality;
			std::string mOutputFormatType;
			std::vector<std::string> mChangeHistory;
			int mChangeHistoryIndex;

			bool isConfigFileExists(const std::string& filename);
			void loadBinFile(std::string& filename);
			void loadXMLFile(std::string& filename);
			void saveBinFile(std::string& filename);
			void saveXmlFile(std::string& filename);
	};

};

#endif 