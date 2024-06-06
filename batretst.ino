class BatteryMonitor {
private:
    int adcPin;           
    float adcMaxValue;   
    float referenceVoltage;
    float voltageDivider; 

public:
    BatteryMonitor(int pin, float maxAdcValue = 4096.0, float refVoltage = 3.3, float divider = 1.0) 
        : adcPin(pin), adcMaxValue(maxAdcValue), referenceVoltage(refVoltage), voltageDivider(divider) {}

    int readADCRaw() {
        return analogRead(adcPin);
    }

    float getBatteryVoltage() {
        float rawValue = readADCRaw();
        float voltage = (rawValue / adcMaxValue) * referenceVoltage * voltageDivider;
        return voltage;
    }

    int getBatteryPercentage() {
        float voltage = getBatteryVoltage();
        float minVoltage = 3.0;
        float maxVoltage = 4.2;
      
        int percentage = (int)(((voltage - minVoltage) / (maxVoltage - minVoltage)) * 100.0); 
        percentage = constrain(percentage, 0, 100); 
        return percentage;
    }

    bool isBatteryLow(float threshold = 3.0) {
        float voltage = getBatteryVoltage();
        return (voltage < threshold);
    }
};

BatteryMonitor batteryMonitor(27, 4096.0, 3.3, 1.3); 

void setup() {
    Serial.begin(9600);
}

void loop() {
    float voltage = batteryMonitor.getBatteryVoltage();
    int percentage = batteryMonitor.getBatteryPercentage();
    bool isLow = batteryMonitor.isBatteryLow();

    Serial.print("Battery Voltage: ");
    Serial.print(voltage);
    Serial.println(" V");

    Serial.print("Battery Percentage: ");
    Serial.print(percentage);
    Serial.println(" %");

    if (isLow) {
        Serial.println("Warning: Battery is low!");
    }

    delay(1000); // Update every second
}
