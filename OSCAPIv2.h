
#ifndef _OSCAPIv2_H
#define _OSCAPIv2_H

#include <HTTPClient.h>
#include <ArduinoJson.h>

typedef String OSC_SessionId;
typedef String OSC_FileUri;

class OSC_Session {
public:
  OSC_SessionId id;
  int timeout;
  OSC_Session(OSC_SessionId _id = "dummy", int _timeout = 0) : id(_id), timeout(_timeout)
  {
  };
};

// POST /osc/commands/execute HTTP/1.1
// Host: [camera ip address]:[httpPort]
// Content-Type: application/json;charset=utf-8
// Accept: application/json
// Content-Length: {CONTENT_LENGTH}
// X-XSRF-Protected: 1
//
// {
//     "name": "camera.takePicture"
// }

class OSCAPIv2 {
private:

public:
  OSCAPIv2()
  {
  };

  //===========================================================================
  // GET /osc/info HTTP/1.1
  // Host: [camera ip address]:[httpPort]
  // Accept: application/json
  // X-XSRF-Protected: 1
  int get_info()
  {
    int api_lv = 0;

    HTTPClient http;
    Serial.println("GET /osc/info");
    http.begin("http://192.168.1.1/osc/info");
    int http_code = http.GET();

    Serial.println(http_code);

    if (http_code > 0) {
      String response = http.getString();

      Serial.println(response);

      StaticJsonBuffer<512> json_buf;
      JsonObject &root = json_buf.parseObject(response.c_str());

      if (http_code == HTTP_CODE_OK) {
        if (root.containsKey("apiLevel")) {
          Serial.println(root["apiLevel"].size());
          api_lv = root["apiLevel"][root["apiLevel"].size() - 1];
        } else {
          api_lv = 1;
        }
      } else {
        // error
      }
    } else {
      // failed
    }
    http.end();

    return (api_lv);
  };

  //===========================================================================
  // POST /osc/state HTTP/1.1
  // Host: [camera ip address]:[httpPort]
  // Accept: application/json
  // X-XSRF-Protected: 1
  double get_state()
  {
    double batt_lv = 0;

    HTTPClient http;
    Serial.println("POST /osc/state");
    http.begin("http://192.168.1.1/osc/state");
    int http_code = http.POST("");

    Serial.println(http_code);

    if (http_code > 0) {
      String response = http.getString();

      Serial.println(response);

      StaticJsonBuffer<512> json_buf;
      JsonObject &root = json_buf.parseObject(response.c_str());

      if (http_code == HTTP_CODE_OK) {
        batt_lv = (double)root["state"]["batteryLevel"];
      } else {
        // error
      }
    } else {
      // failed
    }
    http.end();

    return (batt_lv);
  };

  //===========================================================================
  OSC_Session startSession()
  {
    // deprecated in API level 2
    OSC_Session session("dummy");

    HTTPClient http;
    String request = "{\"name\":\"camera.startSession\"}";
    Serial.println("POST: /osc/commands/execute");
    Serial.println(request);
    http.begin("http://192.168.1.1/osc/commands/execute");
    int http_code = http.POST(request);

    Serial.println(http_code);

    if (http_code > 0) {
      String response = http.getString();

      Serial.println(response);

      StaticJsonBuffer<256> json_buf;
      JsonObject &root = json_buf.parseObject(response.c_str());

      if (http_code == HTTP_CODE_OK) {
        session.id = String((const char *)root["results"]["sessionId"]);
        session.timeout = (int)root["results"]["timeout"];
      } else {
        // error
      }
    } else {
      // failed
    }

    http.end();

    return (session);
  };

  //===========================================================================
  OSC_Session updateSession(OSC_SessionId session_id, int timeout = 0)
  {
    // deprecated in API level 2
    OSC_Session session("dummy");

    HTTPClient http;
    String request = "{\"name\":\"camera.updateSession\",\"parameters\":{\"sessionId\":\"" + session_id + "\"";
    if (timeout > 0) {
      request += ",\"timeout\":";
      request += timeout;
      request += "\"}}";
    } else {
      request += "}}";
    }
    Serial.println("POST: /osc/commands/execute");
    Serial.println(request);
    http.begin("http://192.168.1.1/osc/commands/execute");
    int http_code = http.POST(request);

    Serial.println(http_code);

    if (http_code > 0) {
      String response = http.getString();

      Serial.println(response);

      StaticJsonBuffer<256> json_buf;
      JsonObject &root = json_buf.parseObject(response.c_str());

      if (http_code == HTTP_CODE_OK) {
        session.id = String((const char *)root["results"]["sessionId"]);
        session.timeout = (int)root["results"]["timeout"];
      } else {
        // error
      }
    } else {
      // failed
    }

    http.end();

    return (session);
  };

  //===========================================================================
  void closeSession(OSC_SessionId session_id = "dummy")
  {
    // deprecated in API level 2

    HTTPClient http;
    String request = "{\"name\":\"camera.closeSession\"";
    if (session_id == "dummy") {
      request += "}";
    } else {
      request += ",\"parameters\":{\"sessionId\":\"";
      request += session_id;
      request += "\"}}";
    }
    Serial.println("POST: /osc/commands/execute");
    Serial.println(request);
    http.begin("http://192.168.1.1/osc/commands/execute");
    int http_code = http.POST(request);

    Serial.println(http_code);

    if (http_code > 0) {
      String response = http.getString();

      Serial.println(response);
    } else {
      // failed
    }

    http.end();
  };

  //===========================================================================
  // {"name":"camera.takePicture","state":"inProgress","id":"1","progress":{"completion":0.00}}
  // {"name":"camera.takePicture","state":"done","results":{"fileUri":"100RICOH/R0010XXX.JPG"}}
  OSC_FileUri takePicture(OSC_SessionId session_id = "dummy")
  {
    // session_id was deprecated in API level 2
    String file_uri = "";

    HTTPClient http;
    http.setReuse(true);
    String request = "{\"name\":\"camera.takePicture\"}";
    if (session_id != "dummy") {
      request = "{\"name\":\"camera.takePicture\", \"parameters\":{\"sessionId\":\"" + session_id + "\"}}";
    }
    Serial.println("POST: /osc/commands/execute");
    Serial.println(request);
    http.begin("http://192.168.1.1/osc/commands/execute");
    int http_code = http.POST(request);

    Serial.println(http_code);

    if (http_code > 0) {
      String response = http.getString();

      Serial.println(response);

      StaticJsonBuffer<256> json_buf;
      JsonObject &root = json_buf.parseObject(response.c_str());

      if (http_code == HTTP_CODE_OK && root.containsKey("state")) {
        if (strcmp("done", (const char *)root["state"]) == 0) {
          file_uri = String((const char *)root["results"]["fileUri"]);
        } else if (strcmp("inProgress", (const char *)root["state"]) == 0) {
          String id = root["id"];
          do {
            delay(100);
	    String request2 = "{\"id\":\"" + id + "\"}";
	    Serial.println("POST: /osc/commands/status");
	    Serial.println(request2);
            http.begin("http://192.168.1.1/osc/commands/status");
            http_code = http.POST(request2);
	    Serial.println(http_code);
            if (http_code < 0) break;

            response = http.getString();

            Serial.println(response);

            StaticJsonBuffer<256> json_buf2;
            JsonObject &root2 = json_buf2.parseObject(response.c_str());

            if (http_code == HTTP_CODE_OK && root2.containsKey("state")) {
              if (strcmp("done", (const char *)root2["state"]) == 0) {
                file_uri = String((const char *)root2["results"]["fileUri"]);
                break;
              } else if (strcmp("inProgress", (const char *)root2["state"]) == 0) {
                // in progress
              } else {
                // error
                break;
              }
            } else {
              break;
            }
          } while (true);
        }
      } else {
        // error
      }
    } else {
      // failed
    }

    http.end();

    return (file_uri);
  };

  //===========================================================================
  void setGPSInfo(double lat, double lng, OSC_SessionId session_id = "dummy")
  {
    HTTPClient http;
    String request = "{\"name\":\"camera.setOptions\",\"parameters\":{";
    if (session_id != "dummy") {
      request += "\"sessionId\":\"";
      request += session_id;
      request += "\",";
    }
    request += "\"options\":{";
    request += "\"gpsInfo\":{\"lat\":";
    char buf[32];
    sprintf(buf, "%.6lf", lat);
    request += buf;
    request += ",\"lng\":";
    sprintf(buf, "%.6lf", lng);
    request += buf;
    request += "}}}}";
    Serial.println("POST: /osc/commands/execute");
    Serial.println(request);
    http.begin("http://192.168.1.1/osc/commands/execute");
    int http_code = http.POST(request);

    Serial.println(http_code);

    if (http_code > 0) {
      String response = http.getString();
      Serial.println(response);
    }

    http.end();
  };
};

#endif // !_OSCAPIv2_H
