#define CTL_RST  0x00
#define CTL_RPCL 0x07
#define CTL_RPCH 0x08
#define CTL_JPC  0x09
#define CTL_OC   0x0A

#define OPC_NOP      0xEA
#define OPC_LDA_A    0xAD
#define OPC_LDA_IA   0xA9

#define _H(x) (((x) >> 8) & 0x00FF)

#define _L(x) ((x) & 0x00FF)

word pc;
word sp;
byte sr;
byte a;
byte x;
byte y;

word mem;
word state;
void (*state_func)();

void mc_next_state() {
  state++;
}

void mc_fetch_oc() {
  state = PINB << 4;
  state_func = opcode_state;
}

void mc_next_oc() {
  state = CTL_OC;
  state_func = control_state;
}

void mc_jump_pc() {
  PORTA = _H(pc);
  PORTC = _L(pc);
}

void mc_next_pc() {
  pc++;
  PORTA = _H(pc);
  PORTC = _L(pc);
}

void mc_addr_nzp() {
  mem = PINB;
  state++;
}

void mc_addr_nzp_a() {
  mem |= PINB << 8;
  PORTA = _H(mem);
  PORTC = _L(mem);
}

void mc_addr_nzp_ax() {
  mem |= PINB << 8;
  mem += x;
  PORTA = _H(mem);
  PORTC = _L(mem);
}

void mc_addr_nzp_ay() {
  mem |= PINB << 8;
  mem += y;
  PORTA = _H(mem);
  PORTC = _L(mem);
}

void mc_addr_nzp_ai() {
  mem |= PINB << 8;
  pc = mem;
  PORTA = _H(pc);
  PORTC = _L(pc);
}

void mc_addr_nzp_aix() {
  mem |= PINB << 8;
  mem += x;
  pc = mem;
  PORTA = _H(pc);
  PORTC = _L(pc);
}

void mc_addr_r() {
  pc += PINB;
  PORTA = _H(pc);
  PORTC = _L(pc);
}

void mc_addr_s() {
  PORTA = 1;
  PORTC = _L(sp);
}

void mc_addr_zp() {
  mem = PINB;
  PORTA = 0;
  PORTC = _L(mem);
}

void mc_addr_zpx() {
  mem = PINB;
  mem += x;
  PORTA = 0;
  PORTC = _L(mem);
}

void mc_addr_zpy() {
  mem = PINB;
  mem += y;
  PORTA = 0;
  PORTC = _L(mem);
}

void mc_addr_zpi() {
  mem = PINB;
  PORTA = 0;
  PORTC = _L(mem);
}

void mc_addr_zpix() {
  mem = PINB;
  mem += x;
  PORTA = 0;
  PORTC = _L(mem);
}

void mc_addr_zpiy() {
  mem = PINB;
  mem += y;
  PORTA = 0;
  PORTC = _L(mem);
}

void mc_load_a() {
  a = PINB;
}

void mc_load_x() {
  x = PINB;
}

void mc_load_y() {
  y = PINB;
}

void opcode_state() {
  switch (state) {
    case OPC_NOP << 4:
      mc_next_oc();
      break;

    case OPC_LDA_A << 4:
      mc_addr_nzp();
      mc_next_state();
      break;
    case OPC_LDA_A << 4 + 1:
      mc_addr_nzp_a();
      mc_next_state();
      break;
      
    case OPC_LDA_A << 4 + 2:
    case OPC_LDA_IA << 4:
      mc_load_a();
      mc_next_oc();
      break;
      
    default:
      mc_next_oc();
  }
}

void control_state() {
  switch (state) {
    case CTL_RPCL:
      PORTA = 0xFF;
      PORTC = 0xFC;
      mc_next_state();
      break;
      
    case CTL_RPCH:
      pc = PINB;
      PORTA = 0xFF;
      PORTC = 0xFD;
      mc_next_state();
      break;
      
    case CTL_JPC:
      pc |= PINB << 8;
      mc_jump_pc();
      mc_next_state();
      break;
      
    case CTL_OC:
      mc_next_pc();
      mc_fetch_oc();
      break;

    default:
      state++;
  }
}

void setup() {
  
  // Setup clock interrupt
  GICR |= _BV(INT0);
  MCUCR = MCUCR & ~(_BV(ISC00) | _BV(ISC01)) | _BV(ISC00) | _BV(ISC01);

  // ADDR Lines
  DDRA = B11111111;
  DDRC = B11111111;

  // DATA Lines
  DDRB = B00000000;

  // CTRL Lines
  DDRD = B00010000;

  // Reset
  state = CTL_RST;
  state_func = control_state;
  
  sei();
}

void loop() {
}

ISR(INT0_vect) {
  state_func();
}
