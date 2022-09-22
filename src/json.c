#include <cJSON.h>

#include "json.h"

extern char *mac_address;

json json_to_message(char const *json_string)
{
   cJSON *response = cJSON_Parse(json_string);
   json _json;

   _json.method = cJSON_GetObjectItem(response, "method")->valuestring;
   _json.params = cJSON_GetObjectItem(response, "params")->valuestring;

   return _json;
}