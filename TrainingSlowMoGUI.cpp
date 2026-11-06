#include "pch.h"
#include "TrainingSlowMo.h"

 //Plugin Settings Window code here
std::string TrainingSlowMo::GetPluginName() {
	return "TrainingSlowMo";
}

void TrainingSlowMo::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Render the plugin settings here
// This will show up in bakkesmod when the plugin is loaded at
//  f2 -> plugins -> TrainingSlowMo
void TrainingSlowMo::RenderSettings() {
	ImGui::TextUnformatted("TrainingSlowMo plugin settings");

	CVarWrapper enableCvar = cvarManager->getCvar("slowMoEnabled");
	if (!enableCvar) return;
	bool enabled = enableCvar.getBoolValue();
	if (ImGui::Checkbox("Enable plugin", &enabled))
		enableCvar.setValue(enabled);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Enable/Disable slow motion plugin");
	ImGui::Separator();

	if (ImGui::RadioButton("Custome Training Accuracy", &modeTypeRadio, 0)) {
		cvarManager->getCvar("currMode").setValue(modeTypeRadio);
	}
	if (ImGui::RadioButton("Increase Speed over time", &modeTypeRadio, 1)) {
		cvarManager->getCvar("currMode").setValue(modeTypeRadio);
	}



	//-------------------------Defense Options-----------------------------------
	CVarWrapper onTouchCvar = cvarManager->getCvar("onTouchEnabled");
	if (!onTouchCvar) return;
	bool onTouch = onTouchCvar.getBoolValue();

	if (ImGui::Checkbox("Touching the ball counts as success", &onTouch))
		onTouchCvar.setValue(onTouch);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Touching the ball counts as success (useful for defense training)");
	ImGui::Separator();
	// ------------------------ SPEED SETTINGS ----------------------------------
	
	/*
	CVarWrapper hitsCvar = cvarManager->getCvar("slowMoNumHits");
	
	if (!hitsCvar) return;

	int hit = hitsCvar.getIntValue();
	if (ImGui::SliderInt("Number hits/misses before speed change", &hit, 1, 30)) 
		hitsCvar.setValue(hit);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Number of consecutive hits/misses before speed change.");
	
	*/

	CVarWrapper idealAccuracycVar = cvarManager->getCvar("idealAccuracy");

	if (!idealAccuracycVar) return;

	float idealAcc = idealAccuracycVar.getFloatValue();
	if (ImGui::SliderFloat("Accuracy threshold at last 10 shots to change speed.", &idealAcc, 0.05, 0.999))
		idealAccuracycVar.setValue(idealAcc);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Accuracy threshold at last 10 shots to change speed.");



	CVarWrapper speedIncCvar = cvarManager->getCvar("slowMoSpeedIncrement");

	
	if (!speedIncCvar) return;

	float speedInc = speedIncCvar.getFloatValue();
	if (ImGui::SliderFloat("Value of speed increment", &speedInc, 0.001, 0.999))
		speedIncCvar.setValue(speedInc);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("The value to add or subtract from game speed.");


	CVarWrapper maxSpeedCvar = cvarManager->getCvar("slowMoMaxSpeed");
	if (!maxSpeedCvar) return;
	
	CVarWrapper minSpeedCvar = cvarManager->getCvar("slowMoMinSpeed");
	if (!minSpeedCvar) return;

	int maxSpeed = maxSpeedCvar.getIntValue();
	int  minSpeed = minSpeedCvar.getIntValue();

	if (ImGui::SliderInt("Maximum speed limit", &maxSpeed, 5, 150)) {
			maxSpeedCvar.setValue(maxSpeed);
	}
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Maximum speed limit.");

	
	if (ImGui::SliderInt("Minimum speed limit.", &minSpeed, 5, 150)) {
			minSpeedCvar.setValue(minSpeed);
	}		
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Minimum speed limit.");

	CVarWrapper intervalTimeCvar = cvarManager->getCvar("slowMoIntervalTime");
	if (!intervalTimeCvar) return;
	int intervalSeconds = intervalTimeCvar.getIntValue();
	if (ImGui::SliderInt("Interval increase (seconds).", &intervalSeconds, 30, 600)) {
		intervalTimeCvar.setValue(intervalSeconds);
	}
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Seconds between speed increases (\"Increase speed over time\" mode).");

	// ------------------------ MESSAGES SETTINGS ---------------------------------
	
	CVarWrapper showCurrStreakCvar = cvarManager->getCvar("showCurrStreak");
	if (!showCurrStreakCvar) return;
	bool showCurrStreakB = showCurrStreakCvar.getBoolValue();
	if (ImGui::Checkbox("Show curr streak", &showCurrStreakB))
		showCurrStreakCvar.setValue(showCurrStreakB);
	ImGui::Separator();

	CVarWrapper showStatusCvar = cvarManager->getCvar("showStatus");
	if (!showStatusCvar) return;
	bool showStatusB = showStatusCvar.getBoolValue();
	if (ImGui::Checkbox("Show status bar", &showStatusB))
		showStatusCvar.setValue(showStatusB);
	ImGui::Separator();

	CVarWrapper showGameSpeedCvar = cvarManager->getCvar("showGameSpeed");
	if (!showGameSpeedCvar) return;
	bool showGameSpeedB = showGameSpeedCvar.getBoolValue();
	if (ImGui::Checkbox("Show game speed", &showGameSpeedB)) 
		showGameSpeedCvar.setValue(showGameSpeedB);
	ImGui::Separator();

	CVarWrapper showPrevAccuCvar = cvarManager->getCvar("showPrevAccu");
	if (!showPrevAccuCvar) return;
	bool showPrevAccuB = showPrevAccuCvar.getBoolValue();
	if (ImGui::Checkbox("Show previous accuracy", &showPrevAccuB))
		showPrevAccuCvar.setValue(showPrevAccuB);
	ImGui::Separator();

	CVarWrapper showTotalCountCvar = cvarManager->getCvar("showTotalCount");
	if (!showTotalCountCvar) return;
	bool showTotalCountB = showTotalCountCvar.getBoolValue();
	if (ImGui::Checkbox("Show show total shots count", &showTotalCountB))
		showTotalCountCvar.setValue(showTotalCountB);
	ImGui::Separator();

	CVarWrapper showtotalAccuCvar = cvarManager->getCvar("showtotalAccu");
	if (!showtotalAccuCvar) return;
	bool showtotalAccuB = showtotalAccuCvar.getBoolValue();
	if (ImGui::Checkbox("Show show total accuracy", &showtotalAccuB))
		showtotalAccuCvar.setValue(showtotalAccuB);
	ImGui::Separator();

	CVarWrapper showLastShotsAccuCvar = cvarManager->getCvar("showLastShotsAccu");
	if (!showLastShotsAccuCvar) return;
	bool showLastShotsAccuB = showLastShotsAccuCvar.getBoolValue();
	if (ImGui::Checkbox("Show show last 100 shots accuracy", &showLastShotsAccuB))
		showLastShotsAccuCvar.setValue(showLastShotsAccuB);
	ImGui::Separator();
}



// Do ImGui rendering here
void TrainingSlowMo::Render()
{
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

// Name of the menu that is used to toggle the window.
std::string TrainingSlowMo::GetMenuName()
{
	return "TrainingSlowMo";
}

// Title to give the menu
std::string TrainingSlowMo::GetMenuTitle()
{
	return menuTitle_;
}


// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool TrainingSlowMo::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool TrainingSlowMo::IsActiveOverlay()
{
	return true;
}

// Called when window is opened
void TrainingSlowMo::OnOpen()
{
	isWindowOpen_ = true;
}

// Called when window is closed
void TrainingSlowMo::OnClose()
{
	isWindowOpen_ = false;
}

