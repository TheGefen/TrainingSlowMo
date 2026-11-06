#include "pch.h"
#include "TrainingSlowMo.h"


BAKKESMOD_PLUGIN(TrainingSlowMo, "Adapt game speed while training", plugin_version, PLUGINTYPE_CUSTOM_TRAINING)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void TrainingSlowMo::onLoad()
{
	_globalCvarManager = cvarManager;
	//cvarManager->log("Plugin loaded!");

	cvarManager->registerCvar("slowMoEnabled", "0", "Enable slow motion plugin", true, false, 0, false, 0, true).bindTo(pluginEnabled);
	cvarManager->registerCvar("onTouchEnabled", "0", "Count touching the ball as success", true, false, 0, false, 0, true).bindTo(onTouchEnabled);
	cvarManager->registerCvar("currMode", "0", "currMode", true, true, 0, false, 1, true);


	cvarManager->registerCvar("idealAccuracy", "0.7", "Accuracy value to aim for ", true, true, 0, true, 1, true).bindTo(idealAccuracy);
	cvarManager->registerCvar("slowMoSpeedIncrement", "0.05", "Speed increase/decrease increment.", true, true, 0.001, true, 0.999, true).bindTo(speedIncrement);
	cvarManager->registerCvar("slowMoMaxSpeed", "150", "Maximum speed limit.", true, true, 5, true, 200, true).bindTo(maxSpeed);
	cvarManager->registerCvar("slowMoMinSpeed", "50", "Minimum speed limit", true, true, 5, true, 200, true).bindTo(minSpeed);	
	cvarManager->registerCvar("slowMoIntervalTime", "60", "Minimum speed limit", true, true, 30, true, 600, true).bindTo(intervalTime);

	cvarManager->registerCvar("showStatus", "1", "Show status bar", true, false, 0, false, 0, true).bindTo(showStatus);
	cvarManager->registerCvar("showGameSpeed", "1", "Show game speed", true, false, 0, false, 0, true).bindTo(showGameSpeed);
	cvarManager->registerCvar("showPrevAccu", "1", "Show previous accuracy", true, false, 0, false, 0, true).bindTo(showPrevAccu);
	cvarManager->registerCvar("showCurrStreak", "1", "Show curr streak", true, false, 0, false, 0, true).bindTo(showCurrStreak);
	cvarManager->registerCvar("showTotalCount", "1", "Show show total shots count", true, false, 0, false, 0, true).bindTo(showTotalCount);
	cvarManager->registerCvar("showtotalAccu", "1", "Show show total accuracy ", true, false, 0, false, 0, true).bindTo(showtotalAccu);
	cvarManager->registerCvar("showLastShotsAccu", "1", "Show show last 100 shots accuracy ", true, false, 0, false, 0, true).bindTo(showLastShotsAccu);


	gameWrapper->HookEventWithCaller<ActorWrapper>("Function TAGame.GameEvent_TrainingEditor_TA.LoadRound", [this](ActorWrapper cw, void* params, std::string eventName) {
		//cvarManager->log("customVarianceEnabled:" + *customVarianceEnabled);
		//LOG("LoadRound");
		if (!*pluginEnabled)
			return;
		loadTrainingPack();
		});
	/*
	gameWrapper->HookEventWithCaller<ActorWrapper>("Function ProjectX.EngineShare_X.EventPostLoadMap", [this](ActorWrapper cw, void* params, std::string eventName) {
		//cvarManager->log("customVarianceEnabled:" + *customVarianceEnabled);
		gameWrapper->HookEventPost("Function Engine.Controller.Restart",
			[this](std::string eventName) {
				//LOG("Restart DETECTED");
			});
		if (!*pluginEnabled)
			return;
		if (gameWrapper->IsInFreeplay()) {
			//LOG("Freeplay detected");
			//LOG(fmt::format("Workshop:{0} ", gameWrapper->GetCurrentMap()));
			ServerWrapper sw = gameWrapper->GetCurrentGameState();
			if (!sw) return;
			GameSettingPlaylistWrapper playlist = sw.GetPlaylist();
			if (!playlist) return;

			if (playlist.GetPlaylistId() == 19) {//workshop;
				loadTrainingPack();
			}
			

		}

		
		});
		*/
	gameWrapper->HookEventWithCaller<ActorWrapper>("Function TAGame.GameEvent_TrainingEditor_TA.Destroyed", [this](ActorWrapper cw, void* params, std::string eventName) {
		if (*pluginEnabled && loaded) {
			onUnload();
		}

		});

	//cvarManager->registerNotifier("my_aweseome_notifier", [&](std::vector<std::string> args) {
	//	cvarManager->log("Hello notifier!");
	//}, "", 0);

	//auto cvar = cvarManager->registerCvar("template_cvar", "hello-cvar", "just a example of a cvar");
	//auto cvar2 = cvarManager->registerCvar("template_cvar2", "0", "just a example of a cvar with more settings", true, true, -10, true, 10 );

	//cvar.addOnValueChanged([this](std::string cvarName, CVarWrapper newCvar) {
	//	cvarManager->log("the cvar with name: " + cvarName + " changed");
	//	cvarManager->log("the new value is:" + newCvar.getStringValue());
	//});

	//cvar2.addOnValueChanged(std::bind(&TrainingSlowMo::YourPluginMethod, this, _1, _2));

	// enabled decleared in the header
	//enabled = std::make_shared<bool>(false);
	//cvarManager->registerCvar("TEMPLATE_Enabled", "0", "Enable the TEMPLATE plugin", true, true, 0, true, 1).bindTo(enabled);

	//cvarManager->registerNotifier("NOTIFIER", [this](std::vector<std::string> params){FUNCTION();}, "DESCRIPTION", PERMISSION_ALL);
	//cvarManager->registerCvar("CVAR", "DEFAULTVALUE", "DESCRIPTION", true, true, MINVAL, true, MAXVAL);//.bindTo(CVARVARIABLE);
	//gameWrapper->HookEvent("FUNCTIONNAME", std::bind(&TEMPLATE::FUNCTION, this));
	//gameWrapper->HookEventWithCallerPost<ActorWrapper>("FUNCTIONNAME", std::bind(&TrainingSlowMo::FUNCTION, this, _1, _2, _3));
	//gameWrapper->RegisterDrawable(bind(&TEMPLATE::Render, this, std::placeholders::_1));


	//gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode", [this](std::string eventName) {
	//	cvarManager->log("Your hook got called and the ball went POOF");
	//});
	// You could also use std::bind here
	//gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode", std::bind(&TrainingSlowMo::YourPluginMethod, this);
}

void TrainingSlowMo::onUnload()
{
	if (!loaded)
		return;
	loaded = false;

	//LOG("Unhooking");
	gameWrapper->UnhookEvent("Function TAGame.Car_TA.SetVehicleInput");
	gameWrapper->UnhookEvent("Function TAGame.Ball_TA.RecordCarHit");
	gameWrapper->UnhookEvent("Function TAGame.Ball_TA.Explode");
	gameWrapper->UnhookEventPost("Function Engine.Controller.Restart");
	gameWrapper->UnregisterDrawables();
}

void TrainingSlowMo::loadTrainingPack()
{
	//LOG(fmt::format("Loading loaded:{0}", loaded ? 1 : 0));

	if (loaded)
		return;
	loaded = true;
	isFirst = true;
	//LOG("Hooking");
	resetEverything();

	gameWrapper->RegisterDrawable(bind(&TrainingSlowMo::renderSpeed, this, std::placeholders::_1));

	gameWrapper->HookEventWithCaller<ActorWrapper>("Function TAGame.GameEvent_TrainingEditor_TA.EventRoundChanged", [this](ActorWrapper cw, void* params, std::string eventName) {
		//LOG("EventRoundChanged");
		
		TrainingEditorWrapper trainingWrapper = (TrainingEditorWrapper)cw.memory_address;
		if (!trainingWrapper)
			return;
		//LOG(fmt::format("EventRoundChanged, round:{0} currentRound:{1}", trainingWrapper.GetRoundNum(), trainingWrapper.GetShotAttempt()));

		});

	gameWrapper->HookEvent("Function TAGame.Ball_TA.RecordCarHit",
		[this](std::string eventname) {
			if (!*pluginEnabled || !loaded || !gameWrapper->IsInCustomTraining() || !*onTouchEnabled )
				return;

			isGoal= true;
			isExploded = false;

			//LOG("Time to ball: {0:.3f}s after {1} tick", attempt.timeToBall, attempt.ticksToBall);
		});


	gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode",
		[this](std::string eventName) {
			//LOG("Explode DETECTED");
		//	cvarManager->log("Explode detected");
			if (!*pluginEnabled || !loaded || !gameWrapper->IsInCustomTraining())
				return;
		//	cvarManager->log("Explode detected");
			if (!isGoal) {
				isExploded = true;
				isGoal = false;
			}
		});

	gameWrapper->HookEvent("Function TAGame.Ball_TA.OnHitGoal",
		[this](std::string eventName) {
		//	LOG("OnHitGoal DETECTED");
		//	cvarManager->log("Goal detected");
			if (!*pluginEnabled || !loaded || !gameWrapper->IsInCustomTraining())
				return;
		//	cvarManager->log("Goal detected");
			isExploded = false;
			isGoal = true;
		});


	gameWrapper->HookEventPost("Function Engine.Controller.Restart",
		[this](std::string eventName) {			
			if (!*pluginEnabled || !loaded || !gameWrapper->IsInCustomTraining())
				return;
			//LOG(fmt::format("Restart DETECTED isFirst:{0}", isFirst?1:0));
			if (isFirst) {
				isFirst = false;
				return;
			}
			//gameWrapper->GetCurrentGameState().SetGameTimeRemaining(2.14);
		//	cvarManager->log("Restart detected");
			if (isGoal && !isExploded)
			{
			//	cvarManager->log("Hit! - Conecutive" + consecutiveHits);
				//consecutiveHits++;
				consecutiveMiss = 0;
				shotsHit++;
				totalShots++;
				consecutiveHits++;
				updateShot(true);
				if (shotsCount < 10) {
					colorsArray[shotsCount].R = 0;
					colorsArray[shotsCount].G = 255;
					colorsArray[shotsCount].B = 0;
				}
			} else
			{
		//		cvarManager->log("Miss! - Conecutive" + consecutiveMiss);
				//consecutiveHits = 0;
				consecutiveMiss++;
				consecutiveHits = 0;
				shotsMiss++;
				updateShot(false);
				if (shotsCount < 10) {
					colorsArray[shotsCount].R = 255;
					colorsArray[shotsCount].G = 0;
					colorsArray[shotsCount].B = 0;
				}
			}

			shotsCount++;
			totalCount++;
			isGoal = 0;
			isExploded = 0;
			auto speedCvar = _globalCvarManager->getCvar("sv_soccar_gamespeed");
			float speed = speedCvar.getFloatValue();
			int speedModifyer = 0;
		//	LOG("RESTART DETECTED");
			//LOG(fmt::format("ShotsCount:%d accuracy:{0:.3f}", shotsCount, (float)shotsHit/ (shotsCount >0 ? (float)shotsCount : 1.0)));
			
			if (shotsCount == 10) {
				resetColors();
				speedModifyer = calcModifier();
				speed += *speedIncrement * speedModifyer;
				prevAccuracy = (float)shotsHit / 10;
		//		LOG(fmt::format("Speed change:{0:.3f} ", *speedIncrement * speedModifyer));
		//		cvarManager->log("speed:" + to_string(speed) + " maxSpeed:" + to_string(*maxSpeed));
				if (speed * 100 <= *maxSpeed && speed * 100 >= *minSpeed) {
					speedCvar.setValue(speed);
				}
				shotsCount = 0;
				shotsHit = 0;
				shotsMiss = 0;
			}
			
			
			/*
				if (consecutiveHits != 0 && consecutiveHits % (*numHitsChangedSpeed) == 0)
				{
					LOG("RESTART DETECTED");
					gameWrapper->LogToChatbox(to_string(consecutiveHits) + " " + (consecutiveHits > 1 ? "hits" : "hit") + " in a row");
					speed += *speedIncrement;
					LOG(fmt::format("Speed:{0:.3f} %d", speed, *maxSpeed));
					cvarManager->log("speed:" + to_string(speed) + " maxSpeed:" + to_string(*maxSpeed));
					if (speed*100 <= *maxSpeed) {
						speedCvar.setValue(speed);
						string msg = fmt::format("Game speed + {0:.3f} = {1:.3f}", *speedIncrement, speed);
						gameWrapper->LogToChatbox(msg);
					}
				}
				else if (consecutiveMiss != 0 && consecutiveMiss % (*numHitsChangedSpeed) == 0)
				{
					gameWrapper->LogToChatbox(to_string(consecutiveMiss) + " " + (consecutiveMiss > 1 ? "misses" : "miss") + " in a row");
					speed -= *speedIncrement;
					LOG(fmt::format("Speed:{0:.3f} %d", speed, *maxSpeed));
					cvarManager->log("speed:" + to_string(speed) + " minSpeed:" + to_string(*minSpeed));
					if (speed <= 0)
						speed = 0;

					if (speed*100 >= *minSpeed) {
						speedCvar.setValue(speed);
						string msg = fmt::format("Game speed - {0:.3f} = {1:.3f}", *speedIncrement, speed);
						gameWrapper->LogToChatbox(msg);
					}
				}	
				*/
		});
}

void TrainingSlowMo::renderSpeed(CanvasWrapper canvas) {
	bool training = gameWrapper->IsInCustomTraining();

	if (!*pluginEnabled || !loaded || !training) return;

	float screenWidth = canvas.GetSize().X;
	float screenHeight = canvas.GetSize().Y;

	float mid = -1.1;
	int range = 200;
	//int relLocation = (-1 * attempt.positionY) + range;
	int totalUnits = range * 2;



	float opacity = 1.0;
	Vector2 reqSize = Vector2{ (int)(screenWidth * 70 / 100.f), (int)(screenHeight * 4 / 100.f) };
	int unitWidth = reqSize.X / totalUnits;
	Vector2 boxSize = Vector2{ unitWidth * totalUnits, reqSize.Y };
	Vector2 startPos = Vector2{ (int)((screenWidth / 2) - (boxSize.X / 2)), (int)(screenHeight * 10 / 100.f) };


	int positionModifyer = -60;

	string msg = fmt::format("Streak: {0}", (int)(consecutiveHits));
	int currStreakMsgWidth = (msg.length() * 8.5) - 10;
	canvas.SetColor(255, 255, 255, (char)(255 * opacity ));
	canvas.SetPosition(Vector2{ startPos.X + boxSize.X - currStreakMsgWidth, (int)(startPos.Y + positionModifyer) });
	if (*showCurrStreak) 
		canvas.DrawString(msg, 1, 1, true, false);
	positionModifyer +=   (*showCurrStreak ? 20 : 0);
	

	for (int i = 0; i <= 9; i++) {
		canvas.SetColor(colorsArray[i]);
		//canvas.SetColor(255, 255, 255, (char)(255 * opacity));
		canvas.SetPosition(Vector2{ startPos.X + boxSize.X - currStreakMsgWidth + i * 15, (int)(startPos.Y + positionModifyer) });
		if (*showStatus)
			canvas.DrawString("*", 1.5, 1.5, true, false);
	}
	positionModifyer += (*showStatus ? 20 : 0);

	//draw speed label	
	auto speedCvar = _globalCvarManager->getCvar("sv_soccar_gamespeed");
	float speed = speedCvar.getFloatValue();
	string speedMsg = fmt::format("Game speed: {0}%", (int)(speed * 100));
	int width = (msg.length() * 8.5) - 10;
	canvas.SetColor(255, 255, 255, (char)(255 * opacity));
	canvas.SetPosition(Vector2{ startPos.X + boxSize.X - width, (int)(startPos.Y + positionModifyer) });
	if (*showGameSpeed)
		canvas.DrawString(speedMsg, 1, 1, true, false);
	positionModifyer += (*showGameSpeed ? 20 : 0);

	string prevAccuracyMsg = fmt::format("Previous Accuracy: {0}%", (int)(prevAccuracy * 100));
	int prevAccuracyMsgWidth = (msg.length() * 8.5) - 10;
	canvas.SetColor(255, 255, 255, (char)(255 * opacity));
	canvas.SetPosition(Vector2{ startPos.X + boxSize.X - prevAccuracyMsgWidth, (int)(startPos.Y + positionModifyer) });
	if (*showPrevAccu)
		canvas.DrawString(prevAccuracyMsg, 1, 1, true, false);
	positionModifyer += (*showPrevAccu ? 20 : 0);

	string totalCountMsg = fmt::format("Total Shots: {0}", totalCount);
	int totalCountMsgWidth = (msg.length() * 8.5) - 10;
	canvas.SetColor(255, 255, 255, (char)(255 * opacity));
	canvas.SetPosition(Vector2{ startPos.X + boxSize.X - totalCountMsgWidth, (int)(startPos.Y + positionModifyer) });
	if (*showTotalCount)
		canvas.DrawString(totalCountMsg, 1, 1, true, false);
	positionModifyer += (*showTotalCount ? 20 : 0);

	int totalCountAccu = totalCount > 0 ? (totalShots * 100) / totalCount : 0;
	string totalAccuMsg = fmt::format("Total Accuracy: {0}%", totalCountAccu );
	int totalAccuMsgWidth = (msg.length() * 8.5) - 10;
	canvas.SetColor(255, 255, 255, (char)(255 * opacity));
	canvas.SetPosition(Vector2{ startPos.X + boxSize.X - totalAccuMsgWidth, (int)(startPos.Y + positionModifyer) });
	if (*showtotalAccu)
		canvas.DrawString(totalAccuMsg, 1, 1, true, false);
	positionModifyer += (*showtotalAccu ? 20 : 0);

	int lastShotsAccu = totalCount > 100 ? calculateLastsShotsAccuracy() : totalCountAccu;
	string lastShotsAccuMsg = fmt::format("Last 100 Shots Accuracy: {0}%", lastShotsAccu );
	int totalLastShotsAccuMsgWidth = (msg.length() * 8.5) - 10;
	canvas.SetColor(255, 255, 255, (char)(255 * opacity));
	canvas.SetPosition(Vector2{ startPos.X + boxSize.X - totalLastShotsAccuMsgWidth, (int)(startPos.Y + positionModifyer) });
	if (*showLastShotsAccu)
		canvas.DrawString(lastShotsAccuMsg, 1, 1, true, false);
	positionModifyer += (*showLastShotsAccu ? 20 : 0);

}

int TrainingSlowMo::calcModifier() {
	int speedModifyer = 0;

	float shotsAcc = (float)shotsHit / 10;
	float accDiff = shotsAcc - *idealAccuracy;
	if (accDiff < -0.3) speedModifyer = -2;
	if (accDiff > -0.3 && accDiff < -0.05) speedModifyer = -1;
	if (accDiff < 0.3 && accDiff > 0.05) speedModifyer = 1;
	if (accDiff > 0.3) speedModifyer = 2;

	return speedModifyer;
}

void TrainingSlowMo::resetEverything() {
	resetColors();
	shotsCount = 0;
	shotsHit = 0;
	shotsMiss = 0;
	totalShots = 0;
	totalCount = 0;
	lastShotsArrayIndex = 0;
}

void TrainingSlowMo::resetColors() {
	for (int i = 0; i <= 9; i++) {
		colorsArray[i].R = 255;
		colorsArray[i].G = 255;
		colorsArray[i].B = 255;
		colorsArray[i].A = 255;
	}
}

void TrainingSlowMo::updateShot(bool newShot) {
	lastShotsArray[lastShotsArrayIndex] = newShot;
	// Move to the next index
	lastShotsArrayIndex = (lastShotsArrayIndex + 1) % 100;
}

int TrainingSlowMo::calculateLastsShotsAccuracy() {
	// Count the number of successful shots
	int successfulShots = 0;
	for (int i = 0; i < 100; ++i) {
		if (lastShotsArray[i]) {
			successfulShots++;
		}
	}

	// Calculate the accuracy percentage
	int totalShots = 100; // Assuming 100 shots in the array
	int accuracy = (totalShots == 0) ? 100 : (successfulShots * 100) / totalShots;
	return accuracy;
}
