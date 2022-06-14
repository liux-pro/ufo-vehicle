let serviceUuid = 0xFFE0;
let characteristicUuid = 0xFFE1;

function log(s) {
    console.log(s)
}

let characteristic_uart = null;

function ble_init(deviceName) {
    navigator.bluetooth.requestDevice({filters: [{name: deviceName}], optionalServices: [serviceUuid]})
        .then(device => {
            log('Connecting to GATT Server...');
            return device.gatt.connect();
        })
        .then(server => {
            log('Getting Service...');
            return server.getPrimaryService(serviceUuid);
        })
        .then(service => {
            log('Getting Characteristic...');
            return service.getCharacteristic(characteristicUuid);
        })
        .then(characteristic => {
            log('> Characteristic UUID:  ' + characteristic.uuid);
            log('> Broadcast:            ' + characteristic.properties.broadcast);
            log('> Read:                 ' + characteristic.properties.read);
            log('> Write w/o response:   ' +
                characteristic.properties.writeWithoutResponse);
            log('> Write:                ' + characteristic.properties.write);
            log('> Notify:               ' + characteristic.properties.notify);
            log('> Indicate:             ' + characteristic.properties.indicate);
            log('> Signed Write:         ' +
                characteristic.properties.authenticatedSignedWrites);
            log('> Queued Write:         ' + characteristic.properties.reliableWrite);
            log('> Writable Auxiliaries: ' +
                characteristic.properties.writableAuxiliaries);
            characteristic_uart = characteristic
            return characteristic.readValue();
        }).then(value => {
        console.log(value.getUint8(0))
    })
        .catch(error => {
            log('Argh! ' + error);
        });
}

// function ble_read() {
//     characteristic_uart.readValue()
//         .then(value => {
//             console.log(value.getUint8(0))
//         })
// }

let count = 1

function ble_write() {
    let temp = Uint8Array.of(count++);
    characteristic_uart.writeValue(temp)
}

function ble_notify() {
    characteristic_uart.startNotifications().then(_ => {
        log('> Notifications started');
        characteristic_uart.addEventListener('characteristicvaluechanged',
            handleNotifications);
    });
}

function handleNotifications(event) {
    let value = event.target.value;
    let a = [];
    // Convert raw data bytes to hex values just for the sake of showing something.
    // In the "real" world, you'd use data.getUint8, data.getUint16 or even
    // TextDecoder to process raw data bytes.
    for (let i = 0; i < value.byteLength; i++) {
        a.push('0x' + ('00' + value.getUint8(i).toString(16)).slice(-2));
    }
    log('> ' + a.join(' '));
}