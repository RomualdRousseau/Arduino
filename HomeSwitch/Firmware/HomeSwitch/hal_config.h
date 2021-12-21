#define ROOM_NAME "christmas"
#define SWITCH_NAME "light"
#define SSR_PIN   2

// Update these with values suitable for your network.

const char* ssid = "AirTies4920_SFYN";
const char* password = ""; // Set the WIFI password here

const char* mqtt_server = "mqtt-server.xanadu";
const char* mqtt_user = "subscriber";
const char* mqtt_password = "subscriber";

const char* status_topic = ROOM_NAME "." SWITCH_NAME ".status";
const char* action_topic = ROOM_NAME "." SWITCH_NAME ".onoff";
