const enum TouchPin {
    P0 = DAL.CFG_PIN_P0,
    P1 = DAL.CFG_PIN_P1,
    P2 = DAL.CFG_PIN_P2
}

namespace input {

    /**
     * Button pressed
     */
    //% blockId=device_button_event block="on button|%NAME|pressed"
    export function onButtonPressed(button: Button, body: () => void): void {
        button.onEvent(ButtonEvent.Click, body);
    }

    /**
     * Running time
     */
    //% blockId=device_get_running_time block="running time (ms)"
    export function runningTime() {
        return control.millis();
    }

    /**
     * Touch pin
     */
    //% blockId=device_pin_event block="on pin %name|pressed"
    export function onPinPressed(name: TouchPin, body: () => void): void {
        const pin = pins.pinByCfg(name) as DigitalInOutPin;
        if (!pin) return;
        pin.setPull(PinPullMode.PullUp);
        pin.onEvent(PinEvent.Fall, body);
    }


    //      ACCELEROMETER


    // LSM303AGR, maka address = 0x19
    const ACC_ADDR = 0x19;
    const CTRL_REG1_A = 0x20;
    const OUT_X_L_A = 0x28;

    let accInitialized = false;

    function initHWAccel() {
        if (accInitialized) return;

        const buf = pins.createBuffer(2);
        buf[0] = CTRL_REG1_A;
        buf[1] = 0x57; // 50 Hz + enable XYZ
        pins.i2cWriteBuffer(ACC_ADDR, buf);

        accInitialized = true;
        basic.pause(20);
    }

    function readAccelRaw(): number[] {
        initHWAccel();

        // Read 6 bytes starting from OUT_X_L_A
        pins.i2cWriteNumber(ACC_ADDR, OUT_X_L_A, NumberFormat.UInt8LE, false);
        const data = pins.i2cReadBuffer(ACC_ADDR, 6);

        const x = data.getNumber(NumberFormat.Int16LE, 0);
        const y = data.getNumber(NumberFormat.Int16LE, 2);
        const z = data.getNumber(NumberFormat.Int16LE, 4);

        return [x, y, z];
    }

    export enum XirkabitDimension {
        //% block="x"
        X = 0,
        //% block="y"
        Y = 1,
        //% block="z"
        Z = 2,
        //% block="strength"
        Strength = 3
    }

    /**
     * Get acceleration (mg)
     */
    //% blockId=xirkabit_acceleration
    //% block="acceleration (mg) %axis"
    export function acceleration(axis: XirkabitDimension): number {


        //       SIMULATOR

        if ((<any>pxsim).isSimulator) {

            const ax = pxsim.input.acceleration(Dimension.X)
            const ay = pxsim.input.acceleration(Dimension.Y)
            const az = pxsim.input.acceleration(Dimension.Z)

            switch (axis) {
                case XirkabitDimension.X: return ax;
                case XirkabitDimension.Y: return ay;
                case XirkabitDimension.Z: return az;
                case XirkabitDimension.Strength:
                    return Math.sqrt(ax * ax + ay * ay + az * az);
            }
        }


        //       HARDWARE

        const raw = readAccelRaw();
        const x = raw[0];
        const y = raw[1];
        const z = raw[2];

        switch (axis) {
            case XirkabitDimension.X: return x;
            case XirkabitDimension.Y: return y;
            case XirkabitDimension.Z: return z;
            case XirkabitDimension.Strength:
                return Math.sqrt(x * x + y * y + z * z);
        }

        return 0;
    }

}