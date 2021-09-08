//const uint16_t BLINK_INTERVAL = 500;  // Time interval between toggling LED1 in milliseconds
const uint8_t GREEN_LED = 13;         // GREEN_LED is connected to D13
const uint8_t YELLOW_LED = 12;        // YELLOW_LED is connected to D12
const uint8_t RED_LED = 11;           // RED_LED is connected to D11
//const uint8_t SW1 = 10;               // SW1 is connected to D10
//const uint8_t SW2 = 9;                // SW2 is connected to D9

typedef void (*STATE_HANDLER_T)();    // Defined type to store pointer to a state handler function

void ALL_ON();                         // Forward declarations of state handler functions
void FLASHING();
void ALL_OFF();
void OUT_IN();
void BOUNCEING();

STATE_HANDLER_T prior_state, state;   // Global variables to store the prior and current states
//uint32_t flashing_time;                 // Global variable to store the time that lights last changed state
//uint16_t flashing_count;                // Global variable to store the number of times lights has changed states

void ALL_ON() {
  if (state != prior_state) {         // If we are entering the state, do initialization stuff
    prior_state = state;              // If we have entered the state we then set the prior state to the next ready to move forward
    digitalWrite(GREEN_LED, HIGH);    // we set the green light to be on
    digitalWrite(YELLOW_LED, HIGH);   // we set the yellow light to be on
    digitalWrite(RED_LED, HIGH);      // we set the red light to be on
  }

  // Perform state tasks

  // Check for state transitions
  if (digitalRead(SW1) == HIGH) {    // if the button is pressed we move on the state flashing
    state = FLASHING;
  }

  if (state != prior_state) {         // If we are leaving the state, do clean up stuff
    digitalWrite(GREEN_LED, LOW);     // turn the green light off
    digitalWrite(YELLOW_LED, LOW);    // turn the yellow light off
    digitalWrite(RED_LED, LOW);       // turn the red light off
  }
}

void FLASHING() {
  uint32_t t;
  
  if (state != prior_state) {         // If we are entering the state, do initialization stuff
    prior_state = state;
    digitalWrite(GREEN_LED, HIGH);    // we set the green light to be on
    digitalWrite(YELLOW_LED, HIGH);   // we set the yellow light to be on
    digitalWrite(RED_LED, HIGH);      // we set the red light to be on
    flashing_time = millis();
    flashing_count = 0;
  }

  // Perform state tasks
  t = millis();
  if (t >= flashing_time + BLINK_INTERVAL) {
    digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
    digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
    digitalWrite(RED_LED, !digitalRead(RED_LED));
    flashing_time = t;
    flashing_count++;
  }

  // Check for state transitions
  if (digitalRead(SW2) == HIGH) {
    state = ALL_ON;
  } else if (flashing_count == 20) {
    state = ALL_OFF;
  }

  if (state != prior_state) {         // If we are leaving the state, do clean up stuff
     digitalWrite(GREEN_LED, LOW);     // turn the green light off
    digitalWrite(YELLOW_LED, LOW);    // turn the yellow light off
    digitalWrite(RED_LED, LOW);       // turn the red light off
  }
}
void ALL_OFF() {
  
  if (state != prior_state) {         // If we are entering the state, do initialization stuff
    prior_state = state;
    digitalWrite(GREEN_LED, LOW);    // we set the green light to be off
    digitalWrite(YELLOW_LED, LOW);   // we set the yellow light to be off
    digitalWrite(RED_LED, LOW);      // we set the red light to be off
  }

  if (digitalRead(SW1) == HIGH) {    // if the button is pressed we move on the state flashing
    state = FLASHING;

  if (state != prior_state) {         // If we are leaving the state, do clean up stuff
    digitalWrite(GREEN_LED, LOW);     // turn the green light off
    digitalWrite(YELLOW_LED, LOW);    // turn the yellow light off
    digitalWrite(RED_LED, LOW);       // turn the red light off
  }
}
void OUT_IN() {
  uint32_t t;
  
  if (state != prior_state) {         // If we are entering the state, do initialization stuff
    prior_state = state;
    digitalWrite(GREEN_LED, HIGH);    // we set the green light to be on
    digitalWrite(YELLOW_LED, LOW);   // we set the yellow light to be off
    digitalWrite(RED_LED, HIGH);      // we set the red light to be on
    flashing_time = millis();
    flashing_count = 0;
  }

  // Perform state tasks
  t = millis();
  if (t >= flashing_time + BLINK_INTERVAL) {
    digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
    digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
    digitalWrite(RED_LED, !digitalRead(RED_LED));
    flashing_time = t;
    flashing_count++;
  }

  // Check for state transitions
  if (digitalRead(SW2) == HIGH) {
    state = ALL_OFF;
  } else if (flashing_count == 20) {
    state = BOUNCING;
  }

  if (state != prior_state) {         // If we are leaving the state, do clean up stuff
     digitalWrite(GREEN_LED, LOW);     // turn the green light off
    digitalWrite(YELLOW_LED, LOW);    // turn the yellow light off
    digitalWrite(RED_LED, LOW);       // turn the red light off
  }
}
void BOUNCING() {
  uint32_t t;
  
  if (state != prior_state) {         // If we are entering the state, do initialization stuff
    prior_state = state;
    digitalWrite(GREEN_LED, HIGH);    // we set the green light to be on
    digitalWrite(YELLOW_LED, LOW);   // we set the yellow light to be off
    digitalWrite(RED_LED, LOW);      // we set the red light to be off
    flashing_time = millis();
    flashing_count = 0;
  }

  // Perform state tasks
  t = millis();
  if (t >= flashing_time + BLINK_INTERVAL & t <= flashing_time+(BLINK_INTERVAL*2)) {
    digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
    digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
    flashing_time = t;
    flashing_count++;
  }
   if ( t >= flashing_time+(BLINK_INTERVAL*2)) {
    digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
    digitalWrite(RED_LED, !digitalRead(RED_LED));
    flashing_time = t;
    flashing_count++;
  }

  // Check for state transitions
  if (digitalRead(SW2) == HIGH) {
    state = OUT_IN;
  } else if (flashing_count == 20) {
    state = BOUNCING;
  }

  if (state != prior_state) {         // If we are leaving the state, do clean up stuff
     digitalWrite(GREEN_LED, LOW);     // turn the green light off
    digitalWrite(YELLOW_LED, LOW);    // turn the yellow light off
    digitalWrite(RED_LED, LOW);       // turn the red light off
  }
}

void setup() {
  pinMode(GREEN_LED, OUTPUT);         // Configure GREEN_LED pin as a digital output
  pinMode(YELLOW_LED, OUTPUT);        // Configure YELLOW_LED pin as a digital output
  pinMode(RED_LED, OUTPUT);           // Configure RED_LED pin as a digital output
  pinMode(SW1, INPUT);                // Configure SW1 pin as a digital input
  pinMode(SW2, INPUT);                // Configure SW2 pin as a digital input
  
  digitalWrite(GREEN_LED, LOW);       // Set GREEN_LED low initially
  digitalWrite(YELLOW_LED, LOW);      // Set YELLOW_LED low initially
  digitalWrite(RED_LED, LOW);         // Set RED_LED low initially

  prior_state = NULL;
  state = ALL_ON;
}

void loop() {
  state();
}
