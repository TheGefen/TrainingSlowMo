#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "version.h"

constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);
using namespace std;


class TrainingSlowMo: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow, public BakkesMod::Plugin::PluginWindow
{

	//std::shared_ptr<bool> enabled;

	//Boilerplate
	virtual void onLoad();
	virtual void onUnload();

	// Inherited via PluginSettingsWindow
	
	void RenderSettings() override;
	std::string GetPluginName() override;
	void SetImGuiContext(uintptr_t ctx) override;
	
	//Plugin Enabled
	shared_ptr<bool> pluginEnabled = make_shared<bool>(true);
	shared_ptr<bool> onTouchEnabled = make_shared<bool>(true);

	bool loaded = false;
	bool isFirst = true;
	float prevAccuracy;

	shared_ptr<bool> changeSpeed = make_shared<bool>(false);
	
	// Number of hits/misses before the game speed is changed
	shared_ptr<int> numHitsChangedSpeed = make_shared<int>(4);
	shared_ptr<float> idealAccuracy = make_shared<float>(0.7);
	

	// Amount to change the game speed by
	shared_ptr<float> speedIncrement = make_shared<float>(0.1);

	//Max/Min speed
	shared_ptr<int> maxSpeed = make_shared<int>(120);
	shared_ptr<int> minSpeed = make_shared<int>(70);
	shared_ptr<int> intervalTime = make_shared<int>(60);
	
	// Consecutive hits and misses
	int consecutiveHits = 0;
	int consecutiveMiss = 0;

	int shotsHit, shotsMiss , shotsCount, totalCount, totalShots = 0;
	int calcModifier();

	int modeTypeRadio;
	shared_ptr<bool> showCurrStreak	= make_shared<bool>(true);
	shared_ptr<bool> showStatus = make_shared<bool>(true);
	shared_ptr<bool> showGameSpeed = make_shared<bool>(true);
	shared_ptr<bool> showPrevAccu = make_shared<bool>(true);
	shared_ptr<bool> showTotalCount = make_shared<bool>(true);
	shared_ptr<bool> showtotalAccu = make_shared<bool>(true);
	shared_ptr<bool> showLastShotsAccu = make_shared<bool>(true);

	bool lastShotsArray[100];
	int lastShotsArrayIndex;

	//Attempt
	bool isGoal;
	bool isExploded;

	//Colors
	LinearColor colorsArray[10];

	void resetEverything();
	void resetColors();
	void loadTrainingPack();
	//void unloadTrainingPack();
	void renderSpeed(CanvasWrapper canvas);
	// Inherited via PluginWindow
	void updateShot(bool newShot);
	int calculateLastsShotsAccuracy();

	bool isWindowOpen_ = false;
	bool isMinimized_ = false;
	std::string menuTitle_ = "TrainingSlowMo";

	virtual void Render() override;
	virtual std::string GetMenuName() override;
	virtual std::string GetMenuTitle() override;
	virtual bool ShouldBlockInput() override;
	virtual bool IsActiveOverlay() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;
	
	
};

