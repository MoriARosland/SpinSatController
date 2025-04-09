// ble_handler.m
#import <CoreBluetooth/CoreBluetooth.h>
#import "ble_handler.h"

// Global BLE state variable
int bleState = 0;

@interface BLEHandler : NSObject <CBCentralManagerDelegate, CBPeripheralDelegate>

@property (strong, nonatomic) CBCentralManager *centralManager;
@property (strong, nonatomic) CBPeripheral *targetPeripheral;

@end

@implementation BLEHandler

- (instancetype)init {
    self = [super init];
    if (self) {
        // Initialize the central manager with the main queue
        _centralManager = [[CBCentralManager alloc] initWithDelegate:self queue:nil];
    }
    return self;
}

- (void)startScanning {
    if (self.centralManager.state == CBManagerStatePoweredOn) {
        bleState = 1; // Scanning
        // Scan for all peripherals; optionally, specify service UUIDs if known
        [self.centralManager scanForPeripheralsWithServices:nil options:nil];
    }
}

// CBCentralManagerDelegate methods

- (void)centralManagerDidUpdateState:(CBCentralManager *)central {
    switch (central.state) {
        case CBManagerStatePoweredOn:
            // Bluetooth is on; ready to scan
            break;
        default:
            bleState = 0; // Reset to idle if Bluetooth is off or unsupported
            break;
    }
}

- (void)centralManager:(CBCentralManager *)central
 didDiscoverPeripheral:(CBPeripheral *)peripheral
     advertisementData:(NSDictionary *)advertisementData
                  RSSI:(NSNumber *)RSSI {
    // Check if this is the nRF52840 by name (adjust as needed)
    if ([peripheral.name containsString:@"SpinSat"]) { // Assuming name contains "nRF52840"
        self.targetPeripheral = peripheral;
        peripheral.delegate = self;
        [self.centralManager stopScan];
        [self.centralManager connectPeripheral:peripheral options:nil];
    }
}

- (void)centralManager:(CBCentralManager *)central
  didConnectPeripheral:(CBPeripheral *)peripheral {
    bleState = 2; // Connected
    // Optionally, discover services: [peripheral discoverServices:nil];
}

- (void)centralManager:(CBCentralManager *)central
didFailToConnectPeripheral:(CBPeripheral *)peripheral
                 error:(NSError *)error {
    bleState = 0; // Back to idle on failure
}

@end

// C interface functions
static BLEHandler *globalBLEHandler = nil;

void initBLE(void) {
    if (!globalBLEHandler) {
        globalBLEHandler = [[BLEHandler alloc] init];
    }
}

void startScanning(void) {
    [globalBLEHandler startScanning];
}