#pragma once
#include "I2C_24LC1025.h"
#include "istorage.hpp"
#include <vector>

#define WIFI_CREDENTIALS_ADDRESS 0x0000
#define WIFI_SSID_SIZE 64
#define WIFI_PASSWORD_SIZE 64
#define WIFI_CREDENTIALS_SIZE (WIFI_SSID_SIZE + WIFI_PASSWORD_SIZE)
#define SIGNALS_ADDRESS (WIFI_CREDENTIALS_ADDRESS + WIFI_CREDENTIALS_SIZE)
#define MAX_SIGNAL_NAME_LENGTH 32

class EEPROM : public IStorage {
public:
  EEPROM(uint8_t address);
  bool begin();
  uint16_t saveSignal(Signal signal) override;
  uint16_t getSignalCount() override;
  Signal getSignal(int index) override;
  void removeSignal(int index) override;
  void resetSignals();
  void saveWifiCredentials(const char *ssid, const char *password);
  void getWifiCredentials(char *ssid, char *password);

private:
  I2C_24LC1025 eeprom;
  std::vector<uint16_t> signalAddresses;

  void incrementSignalCount();
  void decrementSignalCount();

  union SignalCounter {
    uint16_t value;
    uint8_t bytes[sizeof(uint16_t)];
  };

  union WifiCredentials {
#pragma pack(push, 1)
    struct {
      char ssid[WIFI_SSID_SIZE];
      char password[WIFI_PASSWORD_SIZE];
    } data;
#pragma pack(pop)
    uint8_t bytes[WIFI_CREDENTIALS_SIZE];
  };

  union SignalData {
#pragma pack(push, 1)
    struct {
      uint8_t batch;
      long code;
      uint16_t pulseLength;
    } data;
#pragma pack(pop)
    uint8_t bytes[sizeof(Signal)];
  };
};
