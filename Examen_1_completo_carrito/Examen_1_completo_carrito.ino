// Pines motores y sensor
const byte IN1 = 14, IN2 = 33, ENA = 13, IN3 = 27, IN4 = 26, ENB = 25;
const byte trigPin = 21, echoPin = 32;

// Variables de tiempo
unsigned long tiempoAnteriorSensor = 0;
const unsigned long intervaloSensor = 100;
float distanciaFrontal = 0, distanciaDerecha = 0, distanciaIzquierda = 0;

// Configuración inicial
void setup() {
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(trigPin, OUTPUT); pinMode(echoPin, INPUT);

  Serial.begin(115200);
}

// Función para mover los motores
void moverMotores(bool in1, bool in2, bool in3, bool in4, byte velocidad) {
  digitalWrite(IN1, in1); digitalWrite(IN2, in2);
  analogWrite(ENA, velocidad);
  digitalWrite(IN3, in3); digitalWrite(IN4, in4);
  analogWrite(ENB, velocidad);
}

// Función para medir la distancia
float medirDistancia() {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10); digitalWrite(trigPin, LOW);
  long duracion = pulseIn(echoPin, HIGH, 30000);  // Timeout de 30 ms
  return duracion > 0 ? duracion * 0.034 / 2 : -1;
}

// Función para controlar el movimiento del carro
void controlMovimiento() {
  // Medir la distancia frontal
  distanciaFrontal = medirDistancia();
  
  if (distanciaFrontal != -1 && distanciaFrontal < 35) {
    // Detener el carro si hay un objeto al frente
    moverMotores(LOW, LOW, LOW, LOW, 0);

    // Medir la distancia hacia la derecha
    // Supongamos que rotamos el sensor para medir
    // distanciaDerecha = medirDistanciaDerecha();

    // Medir la distancia hacia la izquierda
    // distanciaIzquierda = medirDistanciaIzquierda();

    // Verificar si hay obstáculos a ambos lados
    if (distanciaDerecha < 30 && distanciaIzquierda < 30) {
      // Si hay obstáculos en ambos lados, retroceder
      moverMotores(HIGH, LOW, HIGH, LOW, 255); // Retroceder
    } else if (distanciaDerecha >= 30) {
      // Si no hay obstáculo a la derecha, girar a la derecha
      moverMotores(LOW, HIGH, LOW, HIGH, 255); // Girar derecha
    } else if (distanciaIzquierda >= 30) {
      // Si no hay obstáculo a la izquierda, girar a la izquierda
      moverMotores(HIGH, LOW, LOW, HIGH, 255); // Girar izquierda
    }
  } else {
    // Si no hay obstáculos, avanzar
    moverMotores(LOW, HIGH, HIGH, LOW, 230);
  }
}

void loop() {
  // Medición del sensor de distancia
  if (millis() - tiempoAnteriorSensor >= intervaloSensor) {
    tiempoAnteriorSensor = millis();
    distanciaFrontal = medirDistancia();
    Serial.println(distanciaFrontal != -1 ? String("Distancia: ") + distanciaFrontal + " cm" : "Fuera de rango");
  }

  // Control de los motores
  controlMovimiento();
}
