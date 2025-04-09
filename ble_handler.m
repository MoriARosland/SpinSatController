#import <CoreBluetooth/CoreBluetooth.h>
#import "ble_handler.h"

int bleState = 0;

@interface BLEHandler : NSObject <CBCentralManagerDelegate, CBPeripheralDelegate>
@property (strong, nonatomic) CBCentralManager *centralManager;
@property (strong, nonatomic) CBPeripheral *targetPeripheral;
@end

@implementation BLEHandler

- (instancetype)init {
    self = [super init];
    if (self) {
        _centralManager = [[CBCentralManager alloc] initWithDelegate:self queue:nil];
        NSLog(@"BLE Initialized.");
    }
    return self;
}

- (void)startScanning {
    NSLog(@"Attempting Scan...");
    NSLog(@"BT state: %ld", (long)self.centralManager.state);
    if (self.centralManager.state == CBManagerStatePoweredOn) {
        bleState = 1;
        [self.centralManager scanForPeripheralsWithServices:nil options:nil];
        NSLog(@"Scan Successful");
    } else {
        NSLog(@"Scan Failed - Bluetooth not powered on. State: %ld", (long)self.centralManager.state);
    }
}

- (void)centralManagerDidUpdateState:(CBCentralManager *)central {
    switch (central.state) {
        case CBManagerStatePoweredOn:
            NSLog(@"Bluetooth is Powered On");
            [self startScanning]; // Auto-start scan when ready
            break;
        case CBManagerStateUnauthorized:
            NSLog(@"Bluetooth Unauthorized - Check app permissions");
            bleState = 0;
            break;
        case CBManagerStatePoweredOff:
            NSLog(@"Bluetooth is Powered Off");
            bleState = 0;
            break;
        case CBManagerStateUnsupported:
            NSLog(@"Bluetooth Unsupported");
            bleState = 0;
            break;
        case CBManagerStateUnknown:
            NSLog(@"Bluetooth State Unknown");
            bleState = 0;
            break;
        default:
            NSLog(@"Unknown Bluetooth State: %ld", (long)central.state);
            bleState = 0;
            break;
    }
}

- (void)centralManager:(CBCentralManager *)central
 didDiscoverPeripheral:(CBPeripheral *)peripheral
     advertisementData:(NSDictionary *)advertisementData
                  RSSI:(NSNumber *)RSSI {
    NSLog(@"Discovered: %@, RSSI: %@", peripheral.name, RSSI);
    if ([peripheral.name containsString:@"Spinsat"]) {
        NSLog(@"Found SpinSat: %@", peripheral.name);
        self.targetPeripheral = peripheral;
        peripheral.delegate = self;
        [self.centralManager stopScan];
        [self.centralManager connectPeripheral:peripheral options:nil];
    }
}

- (void)centralManager:(CBCentralManager *)central
  didConnectPeripheral:(CBPeripheral *)peripheral {
    NSLog(@"Connected to %@", peripheral.name);
    bleState = 2;
}

- (void)centralManager:(CBCentralManager *)central
didFailToConnectPeripheral:(CBPeripheral *)peripheral
                 error:(NSError *)error {
    NSLog(@"Failed to connect to %@: %@", peripheral.name, error);
    bleState = 0;
}

- (void)centralManager:(CBCentralManager *)central
didDisconnectPeripheral:(CBPeripheral *)peripheral
                 error:(NSError *)error {
    NSLog(@"Disconnected from %@: %@", peripheral.name, error ? error : @"No error");
    bleState = 0;
    
    // Optionally restart scanning after disconnect
    // [self startScanning];
}

@end

static BLEHandler *globalBLEHandler = nil;

void initBLE(void) {
    if (!globalBLEHandler) {
        globalBLEHandler = [[BLEHandler alloc] init];
    }
}

void startScanning(void) {
    [globalBLEHandler startScanning];
}