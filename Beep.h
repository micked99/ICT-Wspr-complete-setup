/* 
 *  Beep functions and CW
 */

#define DIT_DURATION 50

void beep(int on, int off) {
  // freq = 14096950UL;
     freq = 18105900UL;
  // freq = 28125870UL;
  digitalWrite(2, HIGH); // Si5351 on
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  
  Si5351a_Write_Reg (17, 128);   // Disable output CLK1
  Si5351a_Write_Reg (18, 128);   // Disable output CLK2
  Si5351a_Write_Reg (16, 77);    // Enable output CLK0, set crystal as source and Integer Mode on PLLA
  SetFrequency((freq * 100));
  delay(off);
  Si5351a_Write_Reg (16, 128);   // Disable output CLK0
  delay(on);
    
  digitalWrite(2, LOW);  // Si5351 off
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  wdt_reset();
}

void dit() { beep(DIT_DURATION, DIT_DURATION); }

void dah() { beep(DIT_DURATION * 3, DIT_DURATION); }

void sk() { delay(DIT_DURATION * 3 ); }

void cw_a() {rf_on();  dit(); dah();                sk(); rf_off();}
void cw_b() {rf_on();  dah(); dit(); dit(); dit();  sk(); rf_off();}
void cw_c() {rf_on();  dah(); dit(); dah(); dit();  sk(); rf_off();}
void cw_d() {rf_on();  dah(); dit(); dit();         sk(); rf_off();}
void cw_e() {rf_on();  dit();                       sk(); rf_off();}
void cw_f() {rf_on();  dit(); dit(); dah(); dit();  sk(); rf_off();}
void cw_g() {rf_on();  dah(); dah(); dit();         sk(); rf_off();}
void cw_h() {rf_on();  dit(); dit(); dit(); dit();  sk(); rf_off();}
void cw_i() {rf_on();  dit(); dit();                sk(); rf_off();}
void cw_j() {rf_on();  dit(); dah(); dah(); dah();  sk(); rf_off();}
void cw_k() {rf_on();  dah(); dit(); dah();         sk(); rf_off();}
void cw_l() {rf_on();  dit(); dah(); dit(); dit();  sk(); rf_off();}
void cw_m() {rf_on();  dah(); dah();                sk(); rf_off();}
void cw_n() {rf_on();  dah(); dit();                sk(); rf_off();}
void cw_o() {rf_on();  dah(); dah(); dah();         sk(); rf_off();}
void cw_p() {rf_on();  dit(); dah(); dah(); dit();  sk(); rf_off();}
void cw_q() {rf_on();  dah(); dah(); dit(); dah();  sk(); rf_off();}
void cw_r() {rf_on();  dit(); dah(); dit();         sk(); rf_off();}
void cw_s() {rf_on();  dit(); dit(); dit();         sk(); rf_off();}
void cw_t() {rf_on();  dah();                       sk(); rf_off();}
void cw_u() {rf_on();  dit(); dit(); dah();         sk(); rf_off();}
void cw_v() {rf_on();  dit(); dit(); dit(); dah();  sk(); rf_off();}
void cw_w() {rf_on();  dit(); dah(); dah();         sk(); rf_off();}
void cw_x() {rf_on();  dah(); dit(); dit(); dah();  sk(); rf_off();}
void cw_y() {rf_on();  dah(); dit(); dah(); dah();  sk(); rf_off();}
void cw_z() {rf_on();  dah(); dah(); dit(); dit();  sk(); rf_off();}

void cw_1() {rf_on(); dit(); dah(); dah(); dah(); dah(); sk(); rf_off();}
void cw_2() {rf_on(); dit(); dit(); dah(); dah(); dah(); sk(); rf_off();}
void cw_3() {rf_on(); dit(); dit(); dit(); dah(); dah(); sk(); rf_off();}
void cw_4() {rf_on(); dit(); dit(); dit(); dit(); dah(); sk(); rf_off();}
void cw_5() {rf_on(); dit(); dit(); dit(); dit(); dit(); sk(); rf_off();}
void cw_6() {rf_on(); dah(); dit(); dit(); dit(); dit(); sk(); rf_off();}
void cw_7() {rf_on(); dah(); dah(); dit(); dit(); dit(); sk(); rf_off();}
void cw_8() {rf_on(); dah(); dah(); dah(); dit(); dit(); sk(); rf_off();}
void cw_9() {rf_on(); dah(); dah(); dah(); dah(); dit(); sk(); rf_off();}
void cw_0() {rf_on(); dah(); dah(); dah(); dah(); dah(); sk(); rf_off();}
