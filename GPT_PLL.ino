const int inputPin = 10;

const int outputPin = 11;

volatile unsigned long pulseCount = 0;

volatile unsigned long phaseAccumulator = 0;

void setup() {

  pinMode(inputPin, INPUT);

  pinMode(outputPin, OUTPUT);

  // Configure Timer1 to generate a 32 kHz output signal

  TCCR1A = 0;

  TCCR1B = _BV(WGM12) | _BV(CS10);

  OCR1A = 499;

  TIMSK1 = _BV(OCIE1A);

  // Attach interrupt to handle incoming pulses

  attachInterrupt(digitalPinToInterrupt(inputPin), handlePulse, RISING);

}

void loop() {

  // Do any other processing here

}

void handlePulse() {

  // Increment the pulse count

  pulseCount++;

  // Calculate the phase error between the output signal and the incoming pulse

  unsigned long phaseError = (pulseCount * 625) - phaseAccumulator;

  // Accumulate the phase error over time to adjust the output frequency

  phaseAccumulator += phaseError;

  // Calculate the frequency adjustment factor based on the phase error

  float adjustmentFactor = phaseError / 625000.0;

  // Adjust the output frequency using the PLL

  OCR1A = (int)(499 + (adjustmentFactor * 499));

}

ISR(TIMER1_COMPA_vect) {

  // Generate the output signal

  digitalWrite(outputPin, HIGH);

  delayMicroseconds(8); // Half of the 16 us period to achieve a 50% duty cycle

  digitalWrite(outputPin, LOW);

  // Increment the phase accumulator

  phaseAccumulator += 625;

}

