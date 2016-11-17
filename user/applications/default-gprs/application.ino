/*
 * gprs 默认程序
 */

#define LED_PIN D7

void setup()
{
    pinMode(LED_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    delay(1000);
}

