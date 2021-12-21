#include <FastLED.h>

FASTLED_USING_NAMESPACE

class LedStripPattern
{
public:
  virtual void init();
  
  virtual void play();
};

class LedStrip
{
public:
  
  CRGB leds[LD_NUM_LEDS];
  uint8_t global_hue;
  
  void init();

  void update();

  void play();

  void stop();

  void registerPattern(LedStripPattern* pattern, char* description);

  void nextPattern();

private:
  LedStripPattern* patterns[LD_NUM_PATTERNS];
  uint8_t patterns_size;
  uint8_t current_pattern_index;
};

void LedStrip::init()
{
  Serial.println("Initializing led strip ... ");

  FastLED.addLeds<LD_LED_TYPE,LD_DATA_PIN,LD_COLOR_ORDER>(leds, LD_NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(LD_BRIGHTNESS);

  global_hue = 0;

  patterns_size = 0;
  current_pattern_index = 0;
}

void LedStrip::update()
{
  global_hue++;
}

void LedStrip::play()
{
  patterns[current_pattern_index]->play();
  FastLED.show();
}

void LedStrip::stop()
{
  current_pattern_index = 0;
  global_hue = 0;
  fadeToBlackBy(leds, LD_NUM_LEDS, 10);
  FastLED.show();
}

void LedStrip::registerPattern(LedStripPattern* pattern, char* description)
{
  patterns[patterns_size] = pattern;
  patterns_size = (patterns_size + 1) % LD_NUM_PATTERNS;

  Serial.print("Registered a new pattern: ");
  Serial.println(description);
}

void LedStrip::nextPattern()
{
  current_pattern_index = (current_pattern_index + 1) % patterns_size;
}
