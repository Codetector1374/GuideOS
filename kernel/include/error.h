#pragma once


typedef enum os_error {
  OSERR_OK = 0,
  OSERR_INVALID_ARGUMENT = 0x1000,
  OSERR_NO_MEMORY = 0x2000,
} os_error_t;