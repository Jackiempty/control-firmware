#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct {
  void *handler;
  uint8_t buffer[512];
} gnss_hw_t;

typedef struct {
  uint64_t latitude;
  uint64_t longitude;
  float altitude;
  float speed;
  uint32_t timestamp;
  uint8_t valid;
  gnss_hw_t hw;
} gnss_t;

gnss_t *open_gnss_instance(uint32_t id);

void gnss_init(gnss_t *instance);
void gnss_bsp_init(gnss_t *instance);
void gnss_process(gnss_t *instance, uint8_t *data, size_t len);
void gnss_bsp_interrupt(gnss_t *instance, uint8_t *data, size_t len);
