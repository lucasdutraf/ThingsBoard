#ifndef JSON_H
#define JSON_H

struct json {
    const char *method;
    const char *params;
} typedef json;

json json_to_message(char const *json_string);

#endif