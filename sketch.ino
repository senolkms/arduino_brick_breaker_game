#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BRICK_ROWS 3
#define BRICK_COLUMNS 9
#define BRICK_WIDTH 12
#define BRICK_HEIGHT 4
#define BRICK_GAP 2
#define BRICK_START_X 2
#define BRICK_START_Y 10
#define LED_PIN_1 7
#define LED_PIN_2 8
#define LED_PIN_3 9
#define SLIDE_POT_PIN A0
#define PADDLE_WIDTH 30
#define PADDLE_HEIGHT 4
#define PADDLE_Y_POS 60
#define PADDLE_SPEED 2
#define BALL_RADIUS 3

#define BUTTON_DOWN_PIN 6
#define BUTTON_SELECT_PIN 13

#define PHOTO_PIN A2
#define SEGMENT_A 2
#define SEGMENT_B 3
#define SEGMENT_C 4
#define SEGMENT_D 5
#define SEGMENT_E 10
#define SEGMENT_F 11
#define SEGMENT_G 12

#define CAN_RADIUS 3
#define CAN_SPEED 2

#define LUX_THRESHOLD 700 // Lux eşik değeri

int paddleX = (SCREEN_WIDTH - PADDLE_WIDTH) / 2;
int ballX = SCREEN_WIDTH / 2;
int ballY = SCREEN_HEIGHT / 2;
int ballSpeedX = 1;
int ballSpeedY = 1;
bool ballReleased = false;
int lives = 3; // Can sayısı
int level = 1; // Oyun seviyesi
int score = 0; // Oyuncu puanı
int total_skor = 0;

// Tuğlaları tutacak matris
bool bricks[BRICK_COLUMNS][BRICK_ROWS];

bool canActive = false;
int canX, canY;

int currentMenuSelection = 1;

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP);
  
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);

  pinMode(SEGMENT_A, OUTPUT);
  pinMode(SEGMENT_B, OUTPUT);
  pinMode(SEGMENT_C, OUTPUT);
  pinMode(SEGMENT_D, OUTPUT);
  pinMode(SEGMENT_E, OUTPUT);
  pinMode(SEGMENT_F, OUTPUT);
  pinMode(SEGMENT_G, OUTPUT);

  digitalWrite(SEGMENT_A, LOW);
  digitalWrite(SEGMENT_B, LOW);
  digitalWrite(SEGMENT_C, LOW);
  digitalWrite(SEGMENT_D, LOW);
  digitalWrite(SEGMENT_E, LOW);
  digitalWrite(SEGMENT_F, LOW);
  digitalWrite(SEGMENT_G, LOW);

  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1);
  }

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(0, 0);

  delay(2000);
  displayMenu(); 
}

void loop() {
  int choice = getMenuSelection();

  if (choice == 1) {
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.println("Oyun baslıyor...");
    delay(2000);
    startGame(); 
  } else if (choice == 2) {
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.println("Oyuna gosterdiginiz ilgi icin tesekkurler...");
    oled.display();
    delay(2000);
    oled.clearDisplay();
    oled.display();
    while (1); 
  }
}

void displayMenu() {
  oled.clearDisplay();
  oled.setCursor(0, 0);
  if (currentMenuSelection == 1) {
    oled.print(">");
  } else {
    oled.print(" ");
  }
  oled.println("1. Start Game");
  if (currentMenuSelection == 2) {
    oled.print(">");
  } else {
    oled.print(" ");
  }
  oled.println("2. Exit");
  oled.display();
}

int getMenuSelection() {
  int choice = 0;
  
  if (digitalRead(BUTTON_DOWN_PIN) == LOW) {
    delay(100);
    currentMenuSelection++;
    if (currentMenuSelection > 2) {
      currentMenuSelection = 1;
    }
    displayMenu();
    while(digitalRead(BUTTON_DOWN_PIN) == LOW) delay(50);
  }
  
  if (digitalRead(BUTTON_SELECT_PIN) == LOW) {
    delay(100);
    choice = currentMenuSelection;
    while(digitalRead(BUTTON_SELECT_PIN) == LOW) delay(50);
  }
  
  return choice;
}

void startGame() {
  oled.clearDisplay();
  resetGame(); 
  displayLives();
  drawBricks(); 
  oled.display();
  while (lives > 0) { 
    movePaddle();
    if (!ballReleased && digitalRead(BUTTON_SELECT_PIN) == LOW) {
      ballReleased = true;
    }
    if (ballReleased) {
      moveBall();
    }
    delay(10);
  }

  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.println("Oyun Bitti!");
  oled.print("skorunuz: ");
  oled.println(total_skor);
  oled.display();
  delay(2000);
}

void resetGame() {
  for (int col = 0; col < BRICK_COLUMNS; col++) {
    for (int row = 0; row < BRICK_ROWS; row++) {
      bricks[col][row] = true;
    }
  }

  paddleX = (SCREEN_WIDTH - PADDLE_WIDTH) / 2;
  ballX = SCREEN_WIDTH / 2;
  ballY = SCREEN_HEIGHT / 2;
  ballSpeedX = 1 + level; 
  ballSpeedY = 1 + level; 
  ballReleased = false;
}

void displayLives() {
  oled.setCursor(0, 0);
  oled.print("Can: ");
  oled.println(lives);

  if (lives >= 3) {
    digitalWrite(LED_PIN_1, HIGH);
    digitalWrite(LED_PIN_2, HIGH);
    digitalWrite(LED_PIN_3, HIGH);
  } else if (lives == 2) {
    digitalWrite(LED_PIN_1, LOW);
    digitalWrite(LED_PIN_2, HIGH);
    digitalWrite(LED_PIN_3, HIGH);
  } else if (lives == 1) {
    digitalWrite(LED_PIN_1, LOW);
    digitalWrite(LED_PIN_2, LOW);
    digitalWrite(LED_PIN_3, HIGH);
  } else {
    digitalWrite(LED_PIN_1, LOW);
    digitalWrite(LED_PIN_2, LOW);
    digitalWrite(LED_PIN_3, LOW);
  }
}

void displayScore(int score) {
  digitalWrite(SEGMENT_A, HIGH);
  digitalWrite(SEGMENT_B, HIGH);
  digitalWrite(SEGMENT_C, HIGH);
  digitalWrite(SEGMENT_D, HIGH);
  digitalWrite(SEGMENT_E, HIGH);
  digitalWrite(SEGMENT_F, HIGH);
  digitalWrite(SEGMENT_G, HIGH);

  switch (score) {
    case 1:
      displayOne();
      break;
    case 2:
      displayTwo();
      break;
    case 3:
      displayThree();
      break;
    case 4:
      displayFour();
      break;
    case 5:
      displayFive();
      break;
    case 6:
      displaySix();
      break;
    case 7:
      displaySeven();
      break;
    case 8:
      displayEight();
      break;
    case 9:
      displayNine();
      break;
    default:
      displayZero();
      break;
  }
}

void drawBricks() {
  for (int col = 0; col < BRICK_COLUMNS; col++) {
    for (int row = 0; row < BRICK_ROWS; row++) {
      if (bricks[col][row]) {
        int x = BRICK_START_X + col * (BRICK_WIDTH + BRICK_GAP);
        int y = BRICK_START_Y + row * (BRICK_HEIGHT + BRICK_GAP);
        if (analogRead(PHOTO_PIN) < LUX_THRESHOLD) {
          oled.fillRect(x, y, BRICK_WIDTH, BRICK_HEIGHT, WHITE);
          oled.invertDisplay(false);
        } else {
          oled.invertDisplay(true);
          oled.fillRect(x, y, BRICK_WIDTH, BRICK_HEIGHT, WHITE);
        }
      }
    }
  }
}

void drawPaddle() {
  oled.fillRect(paddleX, PADDLE_Y_POS, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
}

void movePaddle() {
  int slideValue = analogRead(SLIDE_POT_PIN);
  
  paddleX = map(slideValue, 0, 1023, 0, SCREEN_WIDTH - PADDLE_WIDTH);

  oled.clearDisplay();
  drawBricks();
  drawPaddle();
  drawBall();
  oled.display();
}

void drawBall() {
  oled.fillCircle(ballX, ballY, BALL_RADIUS, WHITE);
}

void moveBall() {
  oled.fillCircle(ballX, ballY, BALL_RADIUS, BLACK);

  ballX += ballSpeedX; 
  ballY += ballSpeedY; 

  if (ballX + BALL_RADIUS >= SCREEN_WIDTH || ballX - BALL_RADIUS <= 0) {
    ballSpeedX = -ballSpeedX;
  }

  if (ballY - BALL_RADIUS <= 0) {
    ballSpeedY = -ballSpeedY;
  }

  if (ballY + BALL_RADIUS >= PADDLE_Y_POS && ballY - BALL_RADIUS <= PADDLE_Y_POS + PADDLE_HEIGHT) {
    if (ballX + BALL_RADIUS >= paddleX && ballX - BALL_RADIUS <= paddleX + PADDLE_WIDTH) {
      ballSpeedY = -ballSpeedY;
    }
  }

  int ballCol = (ballX - BRICK_START_X) / (BRICK_WIDTH + BRICK_GAP);
  int ballRow = (ballY - BRICK_START_Y) / (BRICK_HEIGHT + BRICK_GAP);

  if (ballRow >= 0 && ballRow < BRICK_ROWS && ballCol >= 0 && ballCol < BRICK_COLUMNS && bricks[ballCol][ballRow]) {
    bricks[ballCol][ballRow] = false; 
    ballSpeedY = -ballSpeedY; 
    total_skor++;
    score++; 

    if (score >= 10) {
      score = score % 10;
    }
    displayScore(score); 

    if (random(1, 11) == 1) {
      createNewCan(ballCol, ballRow); 
    }
  }

  if (ballY + BALL_RADIUS >= SCREEN_HEIGHT) {
    lives--;
    displayLives();

    ballX = SCREEN_WIDTH / 2;
    ballY = SCREEN_HEIGHT / 2;
    ballSpeedX = 1 + level;
    ballSpeedY = 1 + level; 
    ballReleased = true;

    delay(1000);
  }

  drawBall();
  moveCan();
  oled.display();

  if (checkBricksDestroyed()) {
    level++;
    if (level > 3) {

      oled.clearDisplay();
      oled.setCursor(0, 0);
      oled.println("Tebrikler, Oyunu Tamamladınız!");
      oled.print("skorunuz: ");
      oled.println(total_skor);
      oled.display();
      delay(15000);
      resetGame();
    } else {
      oled.clearDisplay();
      oled.setCursor(0, 0);
      oled.print("Level ");
      oled.println(level);
      oled.display();
      delay(2000);
      resetGame(); 
    }
  }
}

bool checkBricksDestroyed() {
  for (int col = 0; col < BRICK_COLUMNS; col++) {
    for (int row = 0; row < BRICK_ROWS; row++) {
      if (bricks[col][row]) {
        return false; 
      }
    }
  }
  return true; 
}

void displayZero() {
  digitalWrite(SEGMENT_A, LOW);
  digitalWrite(SEGMENT_B, LOW);
  digitalWrite(SEGMENT_C, LOW);
  digitalWrite(SEGMENT_D, LOW);
  digitalWrite(SEGMENT_E, LOW);
  digitalWrite(SEGMENT_F, LOW);
  digitalWrite(SEGMENT_G, HIGH);
}

void displayOne() {
  digitalWrite(SEGMENT_A, HIGH);
  digitalWrite(SEGMENT_B, LOW);
  digitalWrite(SEGMENT_C, LOW);
  digitalWrite(SEGMENT_D, HIGH);
  digitalWrite(SEGMENT_E, HIGH);
  digitalWrite(SEGMENT_F, HIGH);
  digitalWrite(SEGMENT_G, HIGH);
}

void displayTwo() {
  digitalWrite(SEGMENT_A, LOW);
  digitalWrite(SEGMENT_B, LOW);
  digitalWrite(SEGMENT_C, HIGH);
  digitalWrite(SEGMENT_D, LOW);
  digitalWrite(SEGMENT_E, LOW);
  digitalWrite(SEGMENT_F, HIGH);
  digitalWrite(SEGMENT_G, LOW);
}

void displayThree() {
  digitalWrite(SEGMENT_A, LOW);
  digitalWrite(SEGMENT_B, LOW);
  digitalWrite(SEGMENT_C, LOW);
  digitalWrite(SEGMENT_D, LOW);
  digitalWrite(SEGMENT_E, HIGH);
  digitalWrite(SEGMENT_F, HIGH);
  digitalWrite(SEGMENT_G, LOW);
}

void displayFour() {
  digitalWrite(SEGMENT_A, HIGH);
  digitalWrite(SEGMENT_B, LOW);
  digitalWrite(SEGMENT_C, LOW);
  digitalWrite(SEGMENT_D, HIGH);
  digitalWrite(SEGMENT_E, HIGH);
  digitalWrite(SEGMENT_F, LOW);
  digitalWrite(SEGMENT_G, LOW);
}

void displayFive() {
  digitalWrite(SEGMENT_A, LOW);
  digitalWrite(SEGMENT_B, HIGH);
  digitalWrite(SEGMENT_C, LOW);
  digitalWrite(SEGMENT_D, LOW);
  digitalWrite(SEGMENT_E, HIGH);
  digitalWrite(SEGMENT_F, LOW);
  digitalWrite(SEGMENT_G, LOW);
}

void displaySix() {
  digitalWrite(SEGMENT_A, LOW);
  digitalWrite(SEGMENT_B, HIGH);
  digitalWrite(SEGMENT_C, LOW);
  digitalWrite(SEGMENT_D, LOW);
  digitalWrite(SEGMENT_E, LOW);
  digitalWrite(SEGMENT_F, LOW);
  digitalWrite(SEGMENT_G, LOW);
}

void displaySeven() {
  digitalWrite(SEGMENT_A, LOW);
  digitalWrite(SEGMENT_B, LOW);
  digitalWrite(SEGMENT_C, LOW);
  digitalWrite(SEGMENT_D, HIGH);
  digitalWrite(SEGMENT_E, HIGH);
  digitalWrite(SEGMENT_F, HIGH);
  digitalWrite(SEGMENT_G, HIGH);
}

void displayEight() {
  digitalWrite(SEGMENT_A, LOW);
  digitalWrite(SEGMENT_B, LOW);
  digitalWrite(SEGMENT_C, LOW);
  digitalWrite(SEGMENT_D, LOW);
  digitalWrite(SEGMENT_E, LOW);
  digitalWrite(SEGMENT_F, LOW);
  digitalWrite(SEGMENT_G, LOW);
}

void displayNine() {
  digitalWrite(SEGMENT_A, LOW);
  digitalWrite(SEGMENT_B, LOW);
  digitalWrite(SEGMENT_C, LOW);
  digitalWrite(SEGMENT_D, LOW);
  digitalWrite(SEGMENT_E, HIGH);
  digitalWrite(SEGMENT_F, LOW);
  digitalWrite(SEGMENT_G, LOW);
}

void createNewCan(int col, int row) {
  canActive = true;
  canX = BRICK_START_X + col * (BRICK_WIDTH + BRICK_GAP) + BRICK_WIDTH / 2;
  canY = BRICK_START_Y + row * (BRICK_HEIGHT + BRICK_GAP) + BRICK_HEIGHT / 2;
}

void drawCan() {
  if (canActive) {
    oled.fillCircle(canX, canY, CAN_RADIUS, WHITE);
  }
}

void moveCan() {
  if (canActive) {
    oled.fillCircle(canX, canY, CAN_RADIUS, BLACK);
    canY += CAN_SPEED;
    if (canY + CAN_RADIUS >= SCREEN_HEIGHT) {
      canActive = false;
      lives++;
      displayLives();
    }
    drawCan();
  }
}
