class _RainbowPattern_t: public LedStripPattern
{
public:
  void init()
  {
    HAL.ledstrip.registerPattern(this, "RainbowPattern");
  }
  
  void play()
  {
    fill_rainbow(HAL.ledstrip.leds, LD_NUM_LEDS, HAL.ledstrip.global_hue, 7);
  }
};
static _RainbowPattern_t RainbowPattern;

class _RainbowWithGlitterPattern_t: public LedStripPattern
{
public:
  void init()
  {
    HAL.ledstrip.registerPattern(this, "RainbowWithGlitterPattern");
  }
  
  void play()
  {
    fill_rainbow(HAL.ledstrip.leds, LD_NUM_LEDS, HAL.ledstrip.global_hue, 7);
    addGlitter(80);
  }

private:
  void addGlitter(fract8 chanceOfGlitter) 
  {
    if(random8() < chanceOfGlitter)
    {
      HAL.ledstrip.leds[random16(LD_NUM_LEDS)] += CRGB::White;
    }
  }
};
static _RainbowWithGlitterPattern_t RainbowWithGlitterPattern;

class _ConfettiPattern_t: public LedStripPattern
{
public:
  void init()
  {
    HAL.ledstrip.registerPattern(this, "ConfettiPattern");
  }
  
  void play()
  {
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy(HAL.ledstrip.leds, LD_NUM_LEDS, 10);
    int pos = random16(LD_NUM_LEDS);
    HAL.ledstrip.leds[pos] += CHSV(HAL.ledstrip.global_hue + random8(64), 200, 255);
  }
};
static _ConfettiPattern_t ConfettiPattern;

class _SinelonPattern_t: public LedStripPattern
{
public:
  void init()
  {
    HAL.ledstrip.registerPattern(this, "SinelonPattern");
  }
  
  void play()
  {
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy(HAL.ledstrip.leds, LD_NUM_LEDS, 20);
    int pos = beatsin16(13, 0, LD_NUM_LEDS - 1);
    HAL.ledstrip.leds[pos] += CHSV(HAL.ledstrip.global_hue, 255, 192);
  }
};
static _SinelonPattern_t SinelonPattern;

class _JugglePattern_t: public LedStripPattern
{
public:
  void init()
  {
    HAL.ledstrip.registerPattern(this, "JugglePattern");
  }
  
  void play()
  {
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy(HAL.ledstrip.leds, LD_NUM_LEDS, 20);
    byte dothue = 0;
    for(int i = 0; i < 8; i++) {
      HAL.ledstrip.leds[beatsin16(i + 7, 0, LD_NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
      dothue += 32;
    }
  }
};
static _JugglePattern_t JugglePattern;

class _BpmPattern_t: public LedStripPattern
{
public:
  void init()
  {
    HAL.ledstrip.registerPattern(this, "BpmPattern");
  }
  
  void play()
  {
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
    for(int i = 0; i < LD_NUM_LEDS; i++) {
      HAL.ledstrip.leds[i] = ColorFromPalette(palette, HAL.ledstrip.global_hue + (i * 2), beat - HAL.ledstrip.global_hue + (i * 10));
    }
  }
};
static _BpmPattern_t BpmPattern;

void installPatterns()
{
  RainbowPattern.init();
  RainbowWithGlitterPattern.init();
  ConfettiPattern.init();
  SinelonPattern.init();
  JugglePattern.init();
  BpmPattern.init();
}
