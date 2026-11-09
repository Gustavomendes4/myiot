
#define N_OUTPUT 3
#define N_INPUT  6

const int InputPins[N_INPUT]   = {13, 14, 27, 26, 25, 33}; // Used for 30 pins module
// const int InputPins[N_INPUT]   = {12, 27, 26, 25, 33, 32}; // Used for 38 pins module
const int OutputPins[N_OUTPUT] = {15, 23, 22};

bool input[N_INPUT];
bool output[N_OUTPUT];

void gpioBegin(){
  int i;

  for(i = 0; i < N_INPUT; i++){
    pinMode(InputPins[i], INPUT);
    input[i] = false;
  }

  for(i = 0; i < N_OUTPUT; i++){
    pinMode(OutputPins[i], OUTPUT);
    output[i] = false;
  }
}

bool gpioUpdate(){
  int i;

  bool input_changed = false;

  for(i = 0; i < N_INPUT; i++){

    if( input[i] != (!digitalRead(InputPins[i])) ){
      input[i] = (!digitalRead(InputPins[i]));
      input_changed = true;
    }
  }

  for(i = 0; i < N_OUTPUT; i++){
    digitalWrite(OutputPins[i], output[i]);
  }

  return input_changed;
}

String inputJson(bool in[]){
  String base("\"Inputs\": {\n");

  int i;
  for(i = 0; i < N_INPUT; i++){
    base += "\"DI" + String(i) + "\": ";

    base += (in[i]) ? "true" : "false";
    base += (i+1 == N_INPUT) ? "\n" : ",\n";
  }

  base += "}";

  return base;
}

String outputJson(bool out[]){
  String base("\"Outputs\": {\n");

  int i;
  for(i = 0; i < N_OUTPUT; i++){
    base += "\"DO" + String(i) + "\": ";

    base += (out[i]) ? "true" : "false";
    base += (i+1 == N_OUTPUT) ? "\n" : ",\n";
  }

  base += "}";

  return base;
}

String createJson(){
  int pos;

  String base("{\"GPIOS\": {\n%, \n%\n}\n}");

  pos = base.indexOf('%');
  base = base.substring(0, pos) + inputJson(input) + base.substring(pos + 1);

  pos = base.indexOf('%');
  base = base.substring(0, pos) + outputJson(output) + base.substring(pos + 1);

  return base;
}

