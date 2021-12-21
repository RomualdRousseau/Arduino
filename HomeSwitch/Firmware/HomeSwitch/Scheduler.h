typedef void (*SchedulerFunc)();

class Scheduler
{
  public:
    void register_mainloop(SchedulerFunc fn);
    
    void register_timer(int duration, SchedulerFunc timer_fn);
    
    void loop();
    
  private:
    long last = 0;
    
    int duration;
    SchedulerFunc timer_fn;

    SchedulerFunc mainloop_fn;
};

void Scheduler::register_mainloop(SchedulerFunc fn)
{
  mainloop_fn = fn;
}

void Scheduler::register_timer(int d, SchedulerFunc fn)
{
  duration = d;
  timer_fn = fn;
}

void Scheduler::loop()
{
  mainloop_fn();
  
  long now = millis();
  if (now - last > duration) {
    last = now;
    timer_fn();
  }
}
