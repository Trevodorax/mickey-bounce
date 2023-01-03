#define PIN_A1 7
#define PIN_A2 10
#define PIN_A3 11
#define PIN_B1 5
#define PIN_B2 13
#define PIN_B3 12
#define PIN_C1 4
#define PIN_C2 3
#define PIN_C3 2

size_t functionTime = 1000;

// all available pin configurations
const int linePins[9] = {
  PIN_A1, PIN_A2, PIN_A3, 
  PIN_B1, PIN_B2, PIN_B3, 
  PIN_C1, PIN_C2, PIN_C3
};

const int columnPins[9] = {
  PIN_A1, PIN_B1, PIN_C1, 
  PIN_A2, PIN_B2, PIN_C2, 
  PIN_A3, PIN_B3, PIN_C3
};

const int snakePins[9] = {
  PIN_A1, PIN_A2, PIN_A3, 
  PIN_B3, PIN_B2, PIN_B1, 
  PIN_C1, PIN_C2, PIN_C3
};

const int snailPins[9] = {
  PIN_A1, PIN_A2, PIN_A3, 
  PIN_B3, PIN_C3, PIN_C2, 
  PIN_C1, PIN_B1, PIN_B2
};

String command;

void setup() 
{
  for(size_t i = 0; i < 9; i++)
  {
    pinMode(linePins[i], OUTPUT);    
  }

  Serial.begin(115200);
}

void loop() 
{
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    interpretCommand(command);
  }

  delay(1);
}


// see documentation to understand the cases
void interpretCommand(String command)
{
  switch(command.charAt(0))
  {
    case 'a':
      delay(functionTime);
      break;
    case 'b':
      allOn();
      break;
    case 'c':
      lineByLine(
        command.charAt(1) == '0' ? linePins : columnPins,
        command.charAt(2) == '1'
      );
      break;
    case 'd':
      ledByLed(
        command.charAt(1) == '0' ? linePins : command.charAt(1) == '1' ? columnPins : command.charAt(1) == '2' ? snakePins : snailPins,
        command.charAt(2) == '1'
      );
      break;
    case 'e':
      // char-'0' converts the char into an int
      wave(command.charAt(1)-'0');
      break;
    case 'f':
      diagonal(command.charAt(1));
      break;
    case 'z':
      // char-'0' converts the char into an int
      functionTime = (command.charAt(1)-'0') * 1000;
      break;

  }
}

/* ===== EXPORTED FUNCTIONS ===== */
// turns all leds on
// b
void allOn() {
  size_t i;
  for(i = 0; i < 9; i++)
  {
    digitalWrite(linePins[i], HIGH);
  }

  delay(functionTime);

  for(i = 0; i < 9; i++)
  {
    digitalWrite(linePins[i], LOW);
  }
}

void allOff() {
  size_t i;

  for(i = 0; i < 9; i++)
  {
    digitalWrite(linePins[i], LOW);
  }
}

// each line lights up once
// c<pinSchema><backAndForth>
void lineByLine(size_t * pins, bool backAndForth)
{
  size_t i;
  size_t stop = 3;
  size_t currentRow;
  int delayPerLine = functionTime / 3;
  if(backAndForth) {
    delayPerLine /= 2;
  }
  
  if(backAndForth)
  {
    stop--;
  }
  
  for(i = 0; i < stop; i++)
  {
    digitalWrite(pins[3 * i + 0], HIGH);
    digitalWrite(pins[3 * i + 1], HIGH);    
    digitalWrite(pins[3 * i + 2], HIGH);
    
    delay(delayPerLine);
    
    digitalWrite(pins[3 * i + 0], LOW);
    digitalWrite(pins[3 * i + 1], LOW);
    digitalWrite(pins[3 * i + 2], LOW);
  }

  if(backAndForth)
  {
    stop++;

    for(i = 0; i < stop; i++)
    {      
      digitalWrite(pins[3 * (2 - i) + 0], HIGH);
      digitalWrite(pins[3 * (2 - i) + 1], HIGH);
      digitalWrite(pins[3 * (2 - i) + 2], HIGH);
      
      delay(delayPerLine);
      
      digitalWrite(pins[3 * (2 - i) + 0], LOW);
      digitalWrite(pins[3 * (2 - i) + 1], LOW);
      digitalWrite(pins[3 * (2 - i) + 2], LOW);
    }
  }
}


// each led blinks in line order
// d<pinSchema><backAndForth>
void ledByLed(size_t * pins, bool backAndForth)
{
  size_t i;
  size_t stop = 9;
  int delayPerPin = functionTime / 8;
  if(backAndForth) {
    delayPerPin /= 2;
  }

  if(backAndForth)
  {
    stop--;
  }
  
  for(i = 0; i < stop; i++)
  {
    blink(pins[i], delayPerPin);
  }

  if(backAndForth)
  {
    stop++;

    for(i = 0; i < stop; i++)
    {
      blink(pins[8 - i], delayPerPin);
    }
  }  
}

// middle lights, then rest, and it repeats
// e<nbWaves>
void wave(unsigned short nbWaves)
{
  size_t i, j;
  size_t delayPerWave = functionTime / nbWaves;

  for(j = 0; j < nbWaves; j++)
  {
    digitalWrite(PIN_B2, HIGH);
  
    delay(delayPerWave / 2);

    digitalWrite(PIN_B2, LOW);

    for(i = 0; i < 9; i++)
    {
      if(i == 4) continue;
      
      digitalWrite(linePins[i], HIGH);
    }

    delay(delayPerWave / 2);

    for(i = 0; i < 9; i++)
    {
      digitalWrite(linePins[i], LOW);
    }
  }
}

// diagonals light up one after the other
// f<diagonalIndex>
void diagonal(char corner)
{
  size_t delayPerDiagonal = functionTime / 5;
  switch(corner)
  {
    case '0':
      digitalWrite(PIN_A1, HIGH);
      delay(delayPerDiagonal);
      digitalWrite(PIN_A1, LOW);

      digitalWrite(PIN_B1, HIGH);
      digitalWrite(PIN_A2, HIGH);
      delay(delayPerDiagonal); 
      digitalWrite(PIN_B1, LOW);
      digitalWrite(PIN_A2, LOW);

      digitalWrite(PIN_C1, HIGH);
      digitalWrite(PIN_B2, HIGH);
      digitalWrite(PIN_A3, HIGH);
      delay(delayPerDiagonal); 
      digitalWrite(PIN_C1, LOW);
      digitalWrite(PIN_B2, LOW);
      digitalWrite(PIN_A3, LOW);

      digitalWrite(PIN_C2, HIGH);
      digitalWrite(PIN_B3, HIGH);
      delay(delayPerDiagonal); 
      digitalWrite(PIN_C2, LOW);
      digitalWrite(PIN_B3, LOW);

      digitalWrite(PIN_C3, HIGH);
      delay(delayPerDiagonal);
      digitalWrite(PIN_C3, LOW);

      break;

    case '1':
      digitalWrite(PIN_A3, HIGH);
      delay(delayPerDiagonal);
      digitalWrite(PIN_A3, LOW);

      digitalWrite(PIN_B3, HIGH);
      digitalWrite(PIN_A2, HIGH);
      delay(delayPerDiagonal); 
      digitalWrite(PIN_B3, LOW);
      digitalWrite(PIN_A2, LOW);

      digitalWrite(PIN_C3, HIGH);
      digitalWrite(PIN_B2, HIGH);
      digitalWrite(PIN_A1, HIGH);
      delay(delayPerDiagonal); 
      digitalWrite(PIN_C3, LOW);
      digitalWrite(PIN_B2, LOW);
      digitalWrite(PIN_A1, LOW);

      digitalWrite(PIN_C2, HIGH);
      digitalWrite(PIN_B1, HIGH);
      delay(delayPerDiagonal); 
      digitalWrite(PIN_C2, LOW);
      digitalWrite(PIN_B1, LOW);

      digitalWrite(PIN_C1, HIGH);
      delay(delayPerDiagonal);
      digitalWrite(PIN_C1, LOW);

      break;

    case '2':
      digitalWrite(PIN_C3, HIGH);
      delay(delayPerDiagonal);
      digitalWrite(PIN_C3, LOW);

      digitalWrite(PIN_C2, HIGH);
      digitalWrite(PIN_B3, HIGH);
      delay(delayPerDiagonal); 
      digitalWrite(PIN_C2, LOW);
      digitalWrite(PIN_B3, LOW);

      digitalWrite(PIN_C1, HIGH);
      digitalWrite(PIN_B2, HIGH);
      digitalWrite(PIN_A3, HIGH);
      delay(delayPerDiagonal); 
      digitalWrite(PIN_C1, LOW);
      digitalWrite(PIN_B2, LOW);
      digitalWrite(PIN_A3, LOW);

      digitalWrite(PIN_B1, HIGH);
      digitalWrite(PIN_A2, HIGH);
      delay(delayPerDiagonal); 
      digitalWrite(PIN_B1, LOW);
      digitalWrite(PIN_A2, LOW);

      digitalWrite(PIN_A1, HIGH);
      delay(delayPerDiagonal);
      digitalWrite(PIN_A1, LOW);
      
      break;
    
    case '3':
      digitalWrite(PIN_C1, HIGH);
      delay(delayPerDiagonal);
      digitalWrite(PIN_C1, LOW);

      digitalWrite(PIN_C2, HIGH);
      digitalWrite(PIN_B1, HIGH);
      delay(delayPerDiagonal); 
      digitalWrite(PIN_C2, LOW);
      digitalWrite(PIN_B1, LOW);

      digitalWrite(PIN_C3, HIGH);
      digitalWrite(PIN_B2, HIGH);
      digitalWrite(PIN_A1, HIGH);
      delay(delayPerDiagonal); 
      digitalWrite(PIN_C3, LOW);
      digitalWrite(PIN_B2, LOW);
      digitalWrite(PIN_A1, LOW);

      digitalWrite(PIN_B3, HIGH);
      digitalWrite(PIN_A2, HIGH);
      delay(delayPerDiagonal); 
      digitalWrite(PIN_B3, LOW);
      digitalWrite(PIN_A2, LOW);

      digitalWrite(PIN_A3, HIGH);
      delay(delayPerDiagonal);
      digitalWrite(PIN_A3, LOW);

      break;
  }
}

// ===== OTHER FUNCTIONS ===== */
void blink(int pin, size_t delayTime)
{
  digitalWrite(pin, HIGH);
  delay(delayTime);
  digitalWrite(pin, LOW);
}


