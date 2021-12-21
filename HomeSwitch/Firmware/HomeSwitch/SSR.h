class SSR
{
public:
  void init();

  void onoff(int value);
};

void SSR::init()
{
  pinMode(SSR_PIN, OUTPUT);
  digitalWrite(SSR_PIN, LOW);
}

void SSR::onoff(int value)
{
  digitalWrite(SSR_PIN, (value == 0) ? LOW : HIGH);
}
