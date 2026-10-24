#include "eeprom.hpp"
#include <cstdint>
#define DELETED_SIGNAL_BATCH 0xFF

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

bool EEPROM::begin() {
  return eeprom.begin();
  for (uint16_t i = 0; i < getSignalCount(); i++) {
    uint16_t address =
        SIGNALS_ADDRESS + sizeof(SignalCounter) + i * sizeof(SignalData);
    if (eeprom.readByte(address) != DELETED_SIGNAL_BATCH) {
      signalAddresses.push_back(address);
    }
  }
}

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

void EEPROM::resetSignals() {
  SignalCounter count;
  count.value = 0;
  eeprom.writeBlock(SIGNALS_ADDRESS, count.bytes, sizeof(count));
}

void EEPROM::saveWifiCredentials(const char *ssid, const char *password) {
  WifiCredentials credentials;
  strncpy(credentials.data.ssid, ssid, WIFI_SSID_SIZE);
  strncpy(credentials.data.password, password, WIFI_PASSWORD_SIZE);
  eeprom.writeBlock(WIFI_CREDENTIALS_ADDRESS, credentials.bytes,
                    sizeof(credentials));
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

uint16_t EEPROM::saveSignal(Signal signal) {
  uint16_t address = SIGNALS_ADDRESS + sizeof(SignalCounter);
  for (auto &addr : signalAddresses) {
    if (addr != address) {
      break;
    }
    address += sizeof(SignalData);
  }
  signalAddresses.push_back(address);

  SignalData signalData;
  signalData.data.batch = signal.batch;
  signalData.data.code = signal.code;
  signalData.data.pulseLength = signal.pulseLength;
  eeprom.writeBlock(address, signalData.bytes, sizeof(signalData));
  incrementSignalCount();
  return address;
}

Signal EEPROM::getSignal(int index) {
  Signal signal;
  if (index < 0 || index >= getSignalCount()) {
    return signal;
  }
  uint16_t address = signalAddresses[index];
  SignalData signalData;
  eeprom.readBlock(address, signalData.bytes, sizeof(signalData));
  signal.id = address;
  signal.batch = signalData.data.batch;
  signal.code = signalData.data.code;
  signal.pulseLength = signalData.data.pulseLength;
  return signal;
}

void EEPROM::removeSignal(int index) {
  if (index < 0 || index >= getSignalCount()) {
    return;
  }
  uint16_t address = signalAddresses[index];
  signalAddresses.erase(signalAddresses.begin() + index);
  decrementSignalCount();

  eeprom.writeByte(address, DELETED_SIGNAL_BATCH);
}
