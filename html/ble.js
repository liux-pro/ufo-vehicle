let serviceUuid = 0xFFFF;
let characteristicUuid = 0xFFFF;
let connected = false;

function log(s) {
    console.log(s)
}

let characteristic_uart = null;

function ble_init(deviceName, callback) {
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
        connected = true
        if (callback) {
            callback();
        }
    })
        .catch(error => {
            log('Argh! ' + error);
            alert("出错了")
        });
}

// function ble_read() {
//     characteristic_uart.readValue()
//         .then(value => {
//             console.log(value.getUint8(0))
//         })
// }

let ble_send_busy = true;

function ble_write(data) {
    // characteristic_uart.writeValue(data)
    if (ble_send_busy) {
        ble_send_busy=false
        characteristic_uart.writeValue(data).then(_ => {
                ble_send_busy = true
            }
        ).catch(_ => {
                ble_send_busy = true
            }
        )
    }else {
        console.log("busy")
    }
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