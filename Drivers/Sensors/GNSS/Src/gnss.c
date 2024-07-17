#include "gnss.h"

#define STR2DIG(x) ((x) - '0')

#define MAX_INSTANCE 1
static gnss_t instance_list[MAX_INSTANCE];

static inline uint32_t gnss_utc_parse(char *utc_string) {
  uint8_t hh = STR2DIG(utc_string[0]) * 10 + STR2DIG(utc_string[1]);
  uint8_t mm = STR2DIG(utc_string[2]) * 10 + STR2DIG(utc_string[3]);
  uint8_t ss = STR2DIG(utc_string[4]) * 10 + STR2DIG(utc_string[5]);
  return hh * 3600 + mm * 60 + ss;
}

static inline void gnss_parser(gnss_t *instance, uint8_t *data, size_t len) {
  char *saveptr;
  char *token = strtok_r(data, ",", &saveptr);
  if (strcmp(token, "$GPRMC") == 0) {
    token = strtok_r(NULL, ",", &saveptr);
    if (token[0] == 0) return;
    instance->timestamp = gnss_utc_parse(token);

    token = strtok_r(NULL, ",", &saveptr);
    if (token[0] == 0) return;
    instance->valid = token[0] == 'A';

    token = strtok_r(NULL, ",", &saveptr);
    if (token[0] == 0) return;
    instance->latitude = ((uint64_t)token[0] << 40) |
                         ((uint64_t)token[1] << 32) | (token[2] << 24) |
                         (token[3] < 16) | (token[4] << 8) | token[5];
    strtok_r(NULL, ",", &saveptr);

    token = strtok_r(NULL, ",", &saveptr);
    if (token[0] == 0) return;
    instance->longitude = ((uint64_t)token[0] << 48) |
                          ((uint64_t)token[1] << 40) |
                          ((uint64_t)token[2] << 32) | (token[3] < 24) |
                          (token[4] << 16) | (token[5] << 8) | token[6];
    strtok_r(NULL, ",", &saveptr);

    token = strtok_r(NULL, ",", &saveptr);
    if (token[0] == 0) return;
    instance->speed = atof(token) * 1.852;

  } else if (strcmp(token, "$GPGGA") == 0) {
    strtok_r(NULL, ",", &saveptr);
    strtok_r(NULL, ",", &saveptr);
    strtok_r(NULL, ",", &saveptr);
    strtok_r(NULL, ",", &saveptr);
    strtok_r(NULL, ",", &saveptr);
    strtok_r(NULL, ",", &saveptr);
    strtok_r(NULL, ",", &saveptr);
    strtok_r(NULL, ",", &saveptr);
    strtok_r(NULL, ",", &saveptr);
    instance->altitude = atof(strtok_r(NULL, ",", &saveptr));
  } else {
    // do nothing
  }
}

gnss_t *open_gnss_instance(uint32_t id) {
  if (id >= MAX_INSTANCE) return NULL;
  return &instance_list[id];
}

void gnss_init(gnss_t *instance) {
  instance->latitude = instance->longitude = 0;
  instance->altitude = 0;
  instance->timestamp = 0;
  instance->valid = 0;
  gnss_bsp_init(instance);
}

void gnss_process(gnss_t *instance, uint8_t *data, size_t len) {
  data[len] = 0;
  char *saveptr;
  char *token = strtok_r(data, "\r\n", &saveptr);
  do {
    int len = saveptr - token;
    if (len > 0) gnss_parser(instance, token, len);
  } while ((token = strtok_r(NULL, "\r\n", &saveptr)));
}
