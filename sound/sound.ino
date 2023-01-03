#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

#define LED1 5
#define LED2 6
#define LED3 9
#define BUZZER 11

int step1 = 500;
int step2 = 1000;

void setup() 
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.begin(115200);
}

void loop() 
{
  String command = "";
  if (Serial.available()) 
  {
    command = Serial.readStringUntil('\n');
    interpret(command);
  }
}

void interpret(String command)
{
  if(command == "s1") 
  {
    step1 = 500;
    step2 = 800;    
    S1();
  } else if(command == "s2") 
  {
    step1 = 200;
    step2 = 400;
    S2();
  } else if(command == "help") 
  {
    Serial.println("\nPossible commands :");
    Serial.println("s1   - Start scene 1");
    Serial.println("s2   - Start scene 2");
    Serial.println("stop - Stop scene");
  }
}

int shouldStopProgram()
{
  String command = "";
  if (Serial.available()) 
  {
    command = Serial.readStringUntil('\n');
    if(command == "stop")
    {
      return 1;
    }
  }

  return 0;
}

void playMelody(int melody[], int tempo, int notes)
{
  int wholenote = (60000 * 4) / tempo;
  int divider = 0, noteDuration = 0;

  int timeBeforeCombo = 0;

  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) 
  {
    // this part comes from us
    if(shouldStopProgram())
    {
      return;
    }

    if(timeBeforeCombo <= 0)
    {
      lightCombo();
      timeBeforeCombo = 5000;
    }
    
    // this part comes from the internet
    divider = melody[thisNote + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }

    timeBeforeCombo -= noteDuration;

    buzz(melody[thisNote], noteDuration);
    delay(noteDuration);
    noTone(BUZZER);
  }
}

// calls tone function and turns the 3 leds on according to the frequency
void buzz(int note, int noteDuration) 
{
  analogWrite(LED1, 0);
  analogWrite(LED2, 0);
  analogWrite(LED3, 0);
  tone(BUZZER, note, noteDuration * 0.9);
  if(note > step1)
  {
    analogWrite(LED3, 255);
    if(note > step2)
    {
      analogWrite(LED2, 255);
      analogWrite(LED1, (note-step2)/2);
    } else
    {
      analogWrite(LED2, (note-step1)/2);
    }
  } else
  {
    analogWrite(LED3, note/2);
  } 

}

void wait(size_t delaySeconds) 
{
  Serial.println("z1");
  for(size_t i = 0; i < delaySeconds; i++) 
  {
    Serial.println("a");
  }
}

void S1()
{
  Serial.println("Starting scene 1");

  /* ===== INTERNET CODE START ===== */

  // notes of the moledy followed by the duration.
  // a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
  // so -4 means a dotted quarter note, that is, a quarter plus an eighteenth
  int melody[] = 
  {
    NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
    REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
    NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
    REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
    NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
    
    REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
    NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
    REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
    NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
    REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
    
    NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
    REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  };

  /* ===== INTERNET CODE END ===== */

  int notes = sizeof(melody) / sizeof(melody[0]) / 2;

  playMelody(melody, 140, notes);
}

void S2()
{
  Serial.println("Starting scene 2");
  int tempo = 150;

  /* ===== INTERNET CODE START ===== */

  int melody[] = 
  {
    NOTE_DS4,8, 
    NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
    NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_E4,8, NOTE_G4,-8, NOTE_B4,8,   
    NOTE_AS4,2, NOTE_A4,-16, NOTE_G4,-16, NOTE_E4,-16, NOTE_D4,-16, 
    NOTE_E4,2, REST,4, REST,8, NOTE_DS4,4,

    NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
    NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_G4,8, NOTE_B4,-8, NOTE_E5,8,
    NOTE_DS5,1,   
    NOTE_D5,2, REST,4, REST,8, NOTE_DS4,8, 
    NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
    NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_E4,8, NOTE_G4,-8, NOTE_B4,8,   
    
    NOTE_AS4,2, NOTE_A4,-16, NOTE_G4,-16, NOTE_E4,-16, NOTE_D4,-16, 
    NOTE_E4,-4, REST,4,
    REST,4, NOTE_E5,-8, NOTE_D5,8, NOTE_B4,-8, NOTE_A4,8, NOTE_G4,-8, NOTE_E4,-8,
    NOTE_AS4,16, NOTE_A4,-8, NOTE_AS4,16, NOTE_A4,-8, NOTE_AS4,16, NOTE_A4,-8, NOTE_AS4,16, NOTE_A4,-8,   
    NOTE_G4,-16, NOTE_E4,-16, NOTE_D4,-16, NOTE_E4,16, NOTE_E4,16, NOTE_E4,2,
  };

  /* ===== INTERNET CODE END ===== */

  int notes = sizeof(melody) / sizeof(melody[0]) / 2;

  playMelody(melody, 140, notes);
}

/* ===== 5s functions that call good light function combinations ===== */
void lightCombo()
{
  int combo = random() % 5;

  switch(combo)
  {
    case 0:
      lightCombo0();
      break;
    case 1:
      lightCombo1();
      break;
    case 2:
      lightCombo2();
      break;
    case 3:
      lightCombo3();
      break;
    case 4:
      lightCombo4();
      break;
    
  }
}

void lightCombo0()
{
  Serial.println("z2");
  Serial.println("e4");

  Serial.println("z1");
  Serial.println("b");
  
  Serial.println("z2");
  Serial.println("e8");
}

void lightCombo1()
{
  Serial.println("z1");
  Serial.println("a");
  Serial.println("f0");
  Serial.println("f1");
  Serial.println("f2");
  Serial.println("f3");
}

void lightCombo2()
{
  Serial.println("z2");
  Serial.println("c01");
  Serial.println("c11");

  Serial.println("z1");
  Serial.println("c00");
}

void lightCombo3()
{
  Serial.println("z1");
  Serial.println("e2");

  Serial.println("z2");
  Serial.println("d01");
  Serial.println("d11");
}

void lightCombo4()
{
  Serial.println("z1");
  Serial.println("a");

  Serial.println("z2");
  Serial.println("d21");
  Serial.println("d31");
}
