const uint16_t BLINK_INTERVAL = 500;  // Time interval between toggling LED1 in milliseconds
const uint16_t DEBOUNCE_INTERVAL = 10;  // Timer interval between successive SW1 reads in milliseconds
const uint8_t GREEN_LED = 13;         // GREEN_LED is connected to D13
const uint8_t YELLOW_LED = 12;        // YELLOW_LED is connected to D12
const uint8_t RED_LED = 11;           // RED_LED is connected to D11
const uint8_t SW1 = 10;               // SW1 is connected to D10
const uint8_t SW2 = 9;                // SW2 is connected to D9

enum states {                         // Define enumerated type for state machine states
  NONE, 
  ALL_ON, 
  ALL_FLASHING, 
  ALL_OFF,
  ALTERNATE_FLASHING,
  BOUNCING_LIGHT, 
};

states prior_state, state;            // Global variables to store the prior and current states
uint32_t flashing_time;                 // Global variable to store the time that YELLOW_LED last changed state
uint16_t flashing_count;                // Global variable to store the number of times YELLOW_LED has changed 
                                      //   states
uint32_t debounce_time;               // Global variable to store the time since SW1 or SW2 was read
bool SW1_went_back_low;               // Global variable to indicate that SW1 returned low after going high
bool SW2_went_back_low;               // Global variable to indicate that SW2 returned low after going high
bool SW1_high;                        // Global variable to store whether SW1 is high
bool SW2_high;                        // Global variable to store whether SW is high


void all_on() {
  uint32_t t;                           // Local variable to store the current value of the millis timer

  if (state != prior_state) {         // If we are entering the state, do initialization stuff
    prior_state = state;
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, HIGH);
  }

  // Perform state tasks
  // Check for state transitions
  t = millis();                     // Get the current value of the millis timer
  if (t >= debounce_time + DEBOUNCE_INTERVAL) { // Sees if DEBOUNCE_INTERVAL millisecodnds have elapsed since debounce_time,
    SW1_high = digitalRead(SW1) == HIGH;        
    SW2_high = digitalRead(SW2) == HIGH;
    if (SW1_went_back_low && SW1_high) {
      state = ALL_FLASHING;
      SW1_went_back_low = false;
    } else if (!SW1_went_back_low && !SW1_high) {
      SW1_went_back_low = true;
    } else if (SW2_went_back_low && SW2_high) {
      state = BOUNCING_LIGHT;
      SW2_went_back_low = false;
    } else if (!SW2_went_back_low && !SW2_high) {
      SW2_went_back_low = true;
    }
    debounce_time = t;
  }
  
  if (state != prior_state) {         // If we are leaving the state, do clean up stuff
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }
}

void all_flashing() {
  uint32_t t;                           // Local variable to store the current value of the millis timer

  if (state != prior_state) {         // If we are entering the state, do initialization stuff
    prior_state = state;
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, HIGH);
    flashing_time = millis();
  }

  // Perform state tasks
  t = millis();
  if (t >= flashing_time + BLINK_INTERVAL) {
    digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
    digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
    digitalWrite(RED_LED, !digitalRead(RED_LED));
    flashing_time = t;
  }

  // Check for state transitions
  if (t >= debounce_time + DEBOUNCE_INTERVAL) {
    SW1_high = digitalRead(SW1) == HIGH;
    SW2_high = digitalRead(SW2) == HIGH;
    if (SW1_went_back_low && SW1_high) {
      state = ALL_OFF;
      SW1_went_back_low = false;
    } else if (!SW1_went_back_low && !SW1_high) {
      SW1_went_back_low = true;
    } else if (SW2_went_back_low && SW2_high) {
      state = ALL_ON;
      SW2_went_back_low = false;
    } else if (!SW2_went_back_low && !SW2_high) {
      SW2_went_back_low = true;
    }
    debounce_time = t;
  }
  

  if (state != prior_state) {         // If we are leaving the state, do clean up stuff
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }
}

void all_off() {
  uint32_t t;                           // Local variable to store the current value of the millis timer

  if (state != prior_state) {         // If we are entering the state, do initialization stuff
    prior_state = state;
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }

  // Perform state tasks


  // Check for state transitions
  t = millis();
  if (t >= debounce_time + DEBOUNCE_INTERVAL) {
    SW1_high = digitalRead(SW1) == HIGH;
    SW2_high = digitalRead(SW2) == HIGH;
    if (SW1_went_back_low && SW1_high) {
      state = ALTERNATE_FLASHING;
      SW1_went_back_low = false;
    } else if (!SW1_went_back_low && !SW1_high) {
      SW1_went_back_low = true;
    } else if (SW2_went_back_low && SW2_high) {
      state = ALL_FLASHING;
      SW2_went_back_low = false;
    } else if (!SW2_went_back_low && !SW2_high) {
      SW2_went_back_low = true;
    }
    debounce_time = t;
  }

  if (state != prior_state) {         // If we are leaving the state, do clean up stuff
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }
}

void alternate_flashing() {
  uint32_t t;                           // Local variable to store the current value of the millis timer

  if (state != prior_state) {         // If we are entering the state, do initialization stuff
    prior_state = state;
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    flashing_time = millis();
  }

  // Perform state tasks
  t = millis();
  if (t >= flashing_time + BLINK_INTERVAL) {
    digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
    digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
    digitalWrite(RED_LED, !digitalRead(RED_LED));
    flashing_time = t;
  }

  // Check for state transitions
  if (t >= debounce_time + DEBOUNCE_INTERVAL) {
    SW1_high = digitalRead(SW1) == HIGH;
    SW2_high = digitalRead(SW2) == HIGH;
    if (SW1_went_back_low && SW1_high) {
      state = BOUNCING_LIGHT;
      SW1_went_back_low = false;
    } else if (!SW1_went_back_low && !SW1_high) {
      SW1_went_back_low = true;
    } else if (SW2_went_back_low && SW2_high) {
      state = ALL_OFF;
      SW2_went_back_low = false;
    } else if (!SW2_went_back_low && !SW2_high) {
      SW2_went_back_low = true;
    }
    debounce_time = t;
  }
  

  if (state != prior_state) {         // If we are leaving the state, do clean up stuff
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }
}

void bouncing_light() {
  uint32_t t;                           // Local variable to store the current value of the millis timer

  if (state != prior_state) {         // If we are entering the state, do initialization stuff
    prior_state = state;
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    flashing_time = millis();
    flashing_count = 1;
  }

  // Perform state tasks
  t = millis();
  if (t >= flashing_time + BLINK_INTERVAL && flashing_count%3 == 1) {
    digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
    digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
    flashing_time = t;
    flashing_count++;
  }
  if (t >= flashing_time + BLINK_INTERVAL && flashing_count%3 == 2) {
    digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
    digitalWrite(RED_LED, !digitalRead(RED_LED));
    flashing_time = t;
    flashing_count++;
  }
  if (t >= flashing_time + BLINK_INTERVAL && flashing_count%3 == 0) {
    digitalWrite(RED_LED, !digitalRead(RED_LED));
    digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
    flashing_time = t;
    flashing_count++;
  }


  if (t >= debounce_time + DEBOUNCE_INTERVAL) {
    SW1_high = digitalRead(SW1) == HIGH;
    SW2_high = digitalRead(SW2) == HIGH;
    if (SW1_went_back_low && SW1_high) {
      state = ALL_ON;
      SW1_went_back_low = false;
    } else if (!SW1_went_back_low && !SW1_high) {
      SW1_went_back_low = true;
    } else if (SW2_went_back_low && SW2_high) {
      state = ALTERNATE_FLASHING;
      SW2_went_back_low = false;
    } else if (!SW2_went_back_low && !SW2_high) {
      SW2_went_back_low = true;
    }
    debounce_time = t;
  }

  if (state != prior_state) {         // If we are leaving the state, do clean up stuff
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
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

  debounce_time = millis();
  SW1_went_back_low = true;
  SW2_went_back_low = true;
  
  prior_state = NONE;
  state = ALL_ON;
}

void loop() {
  switch (state) {
    case ALL_ON:
      all_on();
      break;
    case ALL_FLASHING:
      all_flashing();
      break;
    case ALL_OFF:
      all_off();
      break;
    case ALTERNATE_FLASHING:
      alternate_flashing();
      break;
    case BOUNCING_LIGHT:
      bouncing_light();
      break;
  }
}
