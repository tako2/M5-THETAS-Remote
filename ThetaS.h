
#ifndef _THETAS_H
#define _THETAS_H

#include <WiFi.h>
#include "OSCAPIv2.h"

class ThetaS : public OSCAPIv2
{
private:
  String gSSID = "THETAXS00000000.OSC";
public:
  ThetaS()
  {
  };

  bool connect()
  {
    if (!check_ssid(gSSID)) {
      return (false);
    }
    WiFi.begin(gSSID.c_str(), get_password(gSSID).c_str());
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
    return (true);
  };

  void disconnect()
  {
    WiFi.disconnect();
  };

  String ssid() {
    return (gSSID);
  };

  bool set_ssid(String ssid)
  {
    if (!check_ssid(ssid)) {
      return (false);
    }
    gSSID = ssid;
    return (true);
  };

  String search_thetas()
  {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    int num_ap = WiFi.scanNetworks();
    if (num_ap == 0) {
      return ("");
    }
    for (int idx = 0; idx < num_ap; idx ++) {
      if (check_ssid(WiFi.SSID(idx))) {
        return (WiFi.SSID(idx));
      }
    }
    return ("");
  };

  static bool check_ssid(String ssid)
  {
    if (ssid.startsWith("THETAXS")
        && ssid.endsWith(".OSC") && ssid.length() == 19) {
      return (true);
    }
    return (false);
  };

  static String get_password(String ssid) {
    return (ssid.substring(7, 15));
  };
};

#endif // !_THETAS_H
