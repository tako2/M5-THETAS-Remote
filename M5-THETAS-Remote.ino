#include <M5Stack.h>
#include <Preferences.h>
#include "ThetaS.h"

Preferences gPrefs;

String gThetaSSID;
ThetaS gThetaS;
OSC_Session gSession;

enum {
  THETA_STATE_NONE,
  THETA_STATE_FOUND,
  THETA_STATE_CONNECTED
} gThetaState;

#define MSG_ROW_Y0 16
#define MSG_ROW_Y1 32
#define MSG_ROW_Y2 48
#define MSG_ROW_Y3 64

#define MSG_ROW_Y4 96

#define INFO_ROW_Y1 128
#define INFO_ROW_Y2 144

//=============================================================================
void draw_buttons()
{
  switch (gThetaState) {
  case THETA_STATE_NONE:
    // ----------------------------------------------------- No THETA Found ---
    M5.Lcd.fillRoundRect(16, 200, 80, 24, 4, CYAN);

    M5.Lcd.fillRoundRect(120, 200, 80, 24, 4, CYAN);
    M5.Lcd.setTextColor(BLACK, CYAN);
    M5.Lcd.setCursor(120 + 4 + 6 * 2, 200 + 4);
    M5.Lcd.print(F("FIND"));

    M5.Lcd.fillRoundRect(320 - 80 - 16, 200, 80, 24, 4, CYAN);
    M5.Lcd.setCursor(320 - 80 - 16 + 4, 200 + 4);
    M5.Lcd.setTextColor(BLACK, CYAN);
    M5.Lcd.print(F("PW OFF"));
    break;

  case THETA_STATE_FOUND:
    // ----------------------------------------- THETA Found (Disconnected) ---
    M5.Lcd.fillRoundRect(16, 200, 80, 24, 4, CYAN);
    M5.Lcd.setTextColor(BLACK, CYAN);
    M5.Lcd.setCursor(16 + 4 + 3 * 2, 200 + 4);
    M5.Lcd.print(F("CLEAR"));

    M5.Lcd.fillRoundRect(120, 200, 80, 24, 4, CYAN);
    M5.Lcd.setTextColor(BLACK, CYAN);
    M5.Lcd.setCursor(120 + 4 + 3 * 2, 200 + 4);
    M5.Lcd.print(F("CONN."));

    M5.Lcd.fillRoundRect(320 - 80 - 16, 200, 80, 24, 4, CYAN);
    M5.Lcd.setCursor(320 - 80 - 16 + 4, 200 + 4);
    M5.Lcd.setTextColor(BLACK, CYAN);
    M5.Lcd.print(F("PW OFF"));
    break;

  case THETA_STATE_CONNECTED:
    // ---------------------------------------------------- THETA Connected ---
    M5.Lcd.fillRoundRect(16, 200, 80, 24, 4, CYAN);
    M5.Lcd.setTextColor(BLACK, CYAN);
    M5.Lcd.setCursor(16 + 4, 200 + 4);
    M5.Lcd.print(F("DISCN."));

    M5.Lcd.fillRoundRect(120, 200, 80, 24, 4, CYAN);
    M5.Lcd.setTextColor(BLACK, CYAN);
    M5.Lcd.setCursor(120 + 4 + 3 * 2, 200 + 4);
    M5.Lcd.print(F("THETA"));

    M5.Lcd.fillRoundRect(320 - 80 - 16, 200, 80, 24, 4, CYAN);
    break;
  }
}

//=============================================================================
void setup()
{
  M5.begin();
  gPrefs.begin("m5-theta");

  M5.Lcd.setTextSize(2);
  M5.Lcd.setBrightness(80);

  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.fillRoundRect(4, 8, 320 - 8, 240 - 16, 8, BLACK);

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(12, MSG_ROW_Y0);
  M5.Lcd.println("M5 THETA Remote");

  delay(10);
  gThetaSSID = gPrefs.getString("ssid");

  if (ThetaS::check_ssid(gThetaSSID)) {
    gThetaS.set_ssid(gThetaSSID);
    gThetaState = THETA_STATE_FOUND;

    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setCursor(12, MSG_ROW_Y2);
    M5.Lcd.println(gThetaSSID);
  } else {
    gThetaState = THETA_STATE_NONE;
  }
  draw_buttons();
}

//=============================================================================
bool find_theta()
{
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(12, MSG_ROW_Y1);
  M5.Lcd.println("Finding THETA S...");

  String ssid = gThetaS.search_thetas();
  if (ssid.length() == 0) {
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Lcd.setCursor(12, MSG_ROW_Y2);
    M5.Lcd.println("Failed");
  } else {
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setCursor(12, MSG_ROW_Y2);
    M5.Lcd.println(ssid);

    gThetaS.set_ssid(ssid);
    gThetaSSID = ssid;
    gPrefs.putString("ssid", ssid);

    return (true);
  }

  return (false);
}

//=============================================================================
bool connect_theta()
{
  M5.Lcd.setCursor(12, MSG_ROW_Y3);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.print("Connecting...");
  if (gThetaS.connect()) {
    M5.Lcd.print("OK");

    // start session
    gSession = gThetaS.startSession();

    return (true);
  } else {
    M5.Lcd.print("NG");
  }

  return (false);
}

//=============================================================================
void disconnect_theta()
{
  gThetaS.closeSession(gSession.id);
  gThetaS.disconnect();

  M5.Lcd.setCursor(12, MSG_ROW_Y3);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.print("Disconnected    ");
}

//=============================================================================
void loop()
{
  M5.update();

  switch (gThetaState) {
  case THETA_STATE_NONE:
    // ----------------------------------------------------- No THETA Found ---
    if (M5.BtnB.wasPressed()) {
      if (find_theta()) {
        gThetaState = THETA_STATE_FOUND;
	draw_buttons();
      }
    }
    if (M5.BtnC.wasPressed()) {
      M5.powerOFF();
    }
    break;

  case THETA_STATE_FOUND:
    // ----------------------------------------- THETA Found (Disconnected) ---
    if (M5.BtnB.wasPressed()) {
      if (connect_theta()) {
	gThetaState = THETA_STATE_CONNECTED;
	draw_buttons();

	M5.Lcd.setTextColor(WHITE, BLACK);

	int api_lv = gThetaS.get_info();
	M5.Lcd.setCursor(12, INFO_ROW_Y1);
	M5.Lcd.print("API Lv:");
	M5.Lcd.print(api_lv);

	double batt_lv = gThetaS.get_state();
	M5.Lcd.setCursor(12, INFO_ROW_Y2);
	M5.Lcd.print("Batt.:");
	M5.Lcd.print(batt_lv);
      }
    }
    if (M5.BtnA.wasPressed()) {
      // Clear SSID
      gThetaSSID = "";
      gPrefs.remove("ssid");
      gThetaState = THETA_STATE_NONE;
      draw_buttons();
    }
    if (M5.BtnC.wasPressed()) {
      M5.powerOFF();
    }
    break;

  case THETA_STATE_CONNECTED:
    // ---------------------------------------------------- THETA Connected ---
    if (M5.BtnB.wasPressed()) {
      // gThetaS.setGPSInfo(lat, lng, gSession.id);

      M5.Lcd.setTextColor(WHITE, BLACK);
      M5.Lcd.setCursor(12, MSG_ROW_Y4);
      M5.Lcd.print("Shutter! Wait...      ");

      OSC_FileUri uri = gThetaS.takePicture(gSession.id);

      M5.Lcd.setTextColor(WHITE, BLACK);
      M5.Lcd.setCursor(12, MSG_ROW_Y4);
      M5.Lcd.print(uri);
    }
    if (M5.BtnA.wasPressed()) {
      disconnect_theta();
      gThetaState = THETA_STATE_FOUND;
      draw_buttons();
    }
    if (M5.BtnC.wasPressed()) {
    }
    break;
  }

  delay(100);
}
