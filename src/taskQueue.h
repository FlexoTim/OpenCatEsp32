long taskTimer = 0;
long taskInterval = -1;
class Task {
public:
  char tkn;
  char* parameters;
  int paraLength;
  int dly;
  template<typename T> Task(char t, T* p, int d = 0)
    : tkn{ t }, dly{ d } {
    paraLength = (tkn < 'a') ? strlenUntil(p, '~') : strlen((char*)p);
    parameters = new char[paraLength + 1];
    arrayNCPY(parameters, p, paraLength);
    parameters[paraLength] = tkn < 'a' ? '~' : '\0';
  };
  ~Task() {
    // if (paraLength)
    delete[] parameters;
  };
  void info() {
    printCmdByType(tkn, parameters);
  }
};

class TaskQueue : public QList<Task* > {
public:
  int exceptionQ;
  TaskQueue() {
    PTLF("TaskQ");
    exceptionQ = false;
  };
  void createTask() {  //this is an example task
    this->push_back(new Task('k', "vtF", 2000));
    this->push_back(new Task('k', "up"));
  }
  template<typename T> void addTask(char t, T* p, int d = 0) {
    PTH("add ", p);
    this->push_back(new Task(t, p, d));
  }
  template<typename T> void addTaskToFront(char t, T* p, int d = 0) {
    PTH("add front", p);
    this->push_front(new Task(t, p, d));
  }
  bool Cleared() {
    return this->size() == 0 && long(millis() - taskTimer) > taskInterval;
  }
  void popTask() {
    if (taskInterval == -1 || millis() - taskTimer > taskInterval) {
      Task* t = this->front();
      // t->info();
      token = t->tkn;
      cmdLen = t->paraLength;
      taskInterval = t->dly;
      arrayNCPY(newCmd, t->parameters, cmdLen);
      newCmd[cmdLen] = token < 'a' ? '~' : '\0';
      taskTimer = millis();
      newCmdIdx = 5;
      delete t;
      this->pop_front();
    }
    if (this->size() == 0 && exceptionQ) {
      exceptionQ = false;
    }
  }
};
TaskQueue* tQueue;
