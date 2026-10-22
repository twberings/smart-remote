#include "eeprom.hpp"

void debugReadWrite(uint32_t address, uint8_t *data, size_t length,
                    bool isWrite) {
  if (isWrite) {
    Serial.print("Writing to EEPROM at address: ");
  } else {
    Serial.print("Reading from EEPROM at address: ");
  }
  Serial.print(address, HEX);
  Serial.print(", length: ");
  Serial.println(length);
  for (size_t i = 0; i < length; i++) {
    Serial.print(data[i], HEX);
  }
  Serial.println();
}

EEPROM::EEPROM(uint8_t address) : eeprom(address) {}

bool EEPROM::begin() { return eeprom.begin(); }

void EEPROM::incrementSignalCount() {
  SignalCounter count;
  count.value = getSignalCount();
  count.value++;
  eeprom.writeBlock(SIGNALS_ADDRESS, count.bytes, sizeof(count));
}

void EEPROM::decrementSignalCount() {
  SignalCounter count;
  count.value = getSignalCount();
  if (count.value > 0) {
    count.value--;
    eeprom.writeBlock(SIGNALS_ADDRESS, count.bytes, sizeof(count));
  }
}

void EEPROM::saveWifiCredentials(const char *ssid, const char *password) {
  WifiCredentials credentials;
  strncpy(credentials.data.ssid, ssid, WIFI_SSID_SIZE);
  strncpy(credentials.data.password, password, WIFI_PASSWORD_SIZE);
  auto rv = eeprom.writeBlock(WIFI_CREDENTIALS_ADDRESS, credentials.bytes,
                    sizeof(credentials));
  debugReadWrite(WIFI_CREDENTIALS_ADDRESS, credentials.bytes,
                 sizeof(credentials), true);
  Serial.print("EEPROM writeBlock return value: ");
  Serial.println(rv);
}

void EEPROM::getWifiCredentials(char *ssid, char *password) {
  WifiCredentials credentials;
  eeprom.readBlock(WIFI_CREDENTIALS_ADDRESS, credentials.bytes,
                   sizeof(credentials));
  strncpy(ssid, credentials.data.ssid, WIFI_SSID_SIZE);
  strncpy(password, credentials.data.password, WIFI_PASSWORD_SIZE);
}

uint16_t EEPROM::getSignalCount() {
  SignalCounter count;
  eeprom.readBlock(SIGNALS_ADDRESS, count.bytes, sizeof(count));
  return count.value;
}

void EEPROM::saveSignal(Signal signal) {
  uint16_t count = getSignalCount();
  uint16_t address =
      SIGNALS_ADDRESS + sizeof(SignalCounter) + count * sizeof(SignalData);
  SignalData signalData;
  strncpy(signalData.data.name, signal.name.c_str(), MAX_SIGNAL_NAME_LENGTH);
  signalData.data.code = signal.code;
  signalData.data.pulseLength = signal.pulseLength;
  eeprom.writeBlock(address, signalData.bytes, sizeof(signalData));
  incrementSignalCount();
}

Signal EEPROM::getSignal(int index) {
  Signal signal;
  if (index < 0 || index >= getSignalCount()) {
    return signal;
  }
  uint16_t address =
      SIGNALS_ADDRESS + sizeof(SignalCounter) + index * sizeof(SignalData);
  SignalData signalData;
  eeprom.readBlock(address, signalData.bytes, sizeof(signalData));
  signal.name = std::string(signalData.data.name);
  signal.code = signalData.data.code;
  signal.pulseLength = signalData.data.pulseLength;
  return signal;
}
