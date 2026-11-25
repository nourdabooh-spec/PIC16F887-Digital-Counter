 sbit LCD_RS at RD0_bit;
sbit LCD_EN at RD1_bit;
sbit LCD_D4 at RD4_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D7 at RD7_bit;
sbit LCD_RS_Direction at TRISD0_bit;
sbit LCD_EN_Direction at TRISD1_bit;
sbit LCD_D4_Direction at TRISD4_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D7_Direction at TRISD7_bit;

#define BTN_START RA0_bit
#define BTN_PAUSE RA1_bit
#define BTN_RESET RA2_bit
#define RUN_LED   RA3_bit
#define TENS_SEL  RD2_bit
#define UNITS_SEL RD3_bit
#define LIMIT 25

const unsigned char seg[10] = {0x3F,0x06,0x5B,0x4F,0x66,
                               0x6D,0x7D,0x07,0x7F,0x6F};

unsigned char count = 0;
unsigned char running = 1;   // starts counting automatically

void update_outputs(void) {
  PORTB = count;
  RUN_LED = running;
  Lcd_Out(1,1,"Count:");
  Lcd_Chr(1,8,'0' + count/10);
  Lcd_Chr(1,9,'0' + count%10);
  if (running) Lcd_Out(2,1,"Status:Running");
  else         Lcd_Out(2,1,"Status:Paused ");
}

void main() {
  unsigned char tick = 0;
  unsigned char pS = 1, pP = 1, pR = 1;

  ANSEL = 0; ANSELH = 0;
  C1ON_bit = 0; C2ON_bit = 0;
  TRISA = 0b00000111;
  TRISB = 0; TRISC = 0; TRISD = 0;
  PORTB = 0; PORTC = 0; PORTD = 0;

  Lcd_Init();
  Lcd_Cmd(_LCD_CLEAR);
  Lcd_Cmd(_LCD_CURSOR_OFF);
  update_outputs();

  while (1) {
    if (!BTN_START && pS) { running = 1; update_outputs(); }
    if (!BTN_PAUSE && pP) { running = 0; update_outputs(); }
    if (!BTN_RESET && pR) { count = 0; tick = 0; running = 0; update_outputs(); }
    pS = BTN_START; pP = BTN_PAUSE; pR = BTN_RESET;

    if (running) {
      tick++;
      if (tick >= 100) {
        tick = 0;
        if (count >= LIMIT) count = 0;
        else count++;
        update_outputs();
      }
    }

    TENS_SEL = 1; UNITS_SEL = 1;
    PORTC = seg[count/10];
    TENS_SEL = 0;  Delay_ms(5);
    TENS_SEL = 1;
    PORTC = seg[count%10];
    UNITS_SEL = 0; Delay_ms(5);
    UNITS_SEL = 1;
  }
}