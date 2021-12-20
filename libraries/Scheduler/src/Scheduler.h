#define QUANTUM_TIME  		1

#define TASK_IDLE           0

#define FREQ_TO_DEADLINE(f) (1000 / f)

struct Task
{
  void (*callback)(float dt);
  unsigned int  deadline;
  unsigned long last_exec_time;  
};

class Scheduler
{
public:
  void calibrate(Task* task, int sample_count);
  
  void register_tasks(Task* tasks, int count);

  void run_mainloop();

private:
  Task*             _tasks;
  int               _task_count;
  unsigned int      _time_quantum = QUANTUM_TIME;
};

void Scheduler::calibrate(Task* task, int sample_count)
{
  Serial.print("Calibrating scheduler ... ");
  
  unsigned long start_time = millis();
  for(int i = 0; i < sample_count; i++)
  {
    task->callback(0);
  }
  _time_quantum = 1 + (millis() - start_time) / sample_count;

  Serial.print(_time_quantum);
  Serial.println("ms");
}

void Scheduler::register_tasks(Task* tasks, int count)
{
  _tasks = tasks;
  _task_count = count;
  
  unsigned long current_time = millis();
  for(int i = 0; i < _task_count; i++)
  {
    _tasks[i].last_exec_time = current_time;
  }
}

void Scheduler::run_mainloop()
{
  unsigned long current_time = millis();

  for(int i = 0; i < _task_count; i++)
  {
    unsigned int t = current_time - _tasks[i].last_exec_time;
    if(t >= _tasks[i].deadline)
    {
      _tasks[i].last_exec_time = millis();
      _tasks[i].callback(t * 0.001);
    }
  }

  unsigned int t = millis() - current_time;
  if(t < _time_quantum)
  {
    delay(_time_quantum - t);
  }
}
