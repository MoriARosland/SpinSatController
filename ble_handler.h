// ble_handler.h
#ifndef BLE_HANDLER_H
#define BLE_HANDLER_H

// BLE state variable: 0 = idle, 1 = scanning, 2 = connected
extern int bleState;

void initBLE(void);
void startScanning(void);

#endif