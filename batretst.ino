class BatteryMonitor {
public:
    BatteryMonitor(int pin, float calibration = 0.36) : _pin(pin), _calibration(calibration) {}

    void begin() {
        Serial.begin(9600);
    }

    int readADCRaw() {
        return analogRead(_pin);
    }

    float getBatteryVoltage() {
        int sensorValue = readADCRaw();
        return (((sensorValue * 3.3) / 1024) * 2 + _calibration); // Tegangan referensi ADC ESP8266 adalah 1.0V, dikalikan 2 karena pembagi tegangan
    }

  int getBatteryPercentage() {
    float voltage = getBatteryVoltage();
    int percentage = mapfloat(voltage, 0.0, 4.2, 0, 100); // Sesuaikan batas atas dengan tegangan yang biasanya tercapai oleh baterai LiPo

    if (percentage > 100) {
        percentage = 100;
    } else if (percentage < 0) {
        percentage = 0;
    }

    return percentage;
}

    bool isBatteryLow(float threshold = 3.0) {
        return getBatteryVoltage() < threshold;
    }

    void sendBatteryStatus() {
        int rawValue = readADCRaw();
        float voltage = getBatteryVoltage();
        int percentage = getBatteryPercentage();

       
        Serial.print("\t Tegangan Baterai = ");
        Serial.print(voltage);
        Serial.print(" V");
        Serial.print("\t Persentase Baterai = ");
        Serial.print(percentage);
        Serial.println(" %");
    }

private:
    int _pin;
    float _calibration;

    float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
};

// Penggunaan kelas
BatteryMonitor batteryMonitor(A0);

void setup() {
    batteryMonitor.begin();
}

void loop() {
    batteryMonitor.sendBatteryStatus();
    delay(1000);
}
