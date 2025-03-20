//1번 RC카
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SoftwareSerial.h>  //가상 시리얼 통신을 위한 라이브러리 선언
#include <Servo.h>
Servo EduServo;
RF24 radio(3, 4); // SPI 버스에 nRF24L01 라디오를 설정하기 위해 CE, CSN를 선언

const byte address[6] = "12345"; // 메인 오더에게 보낼 채널 주소
String now_mode = "receive"; // nRF24L01의 기본 형태를 수신형으로 설정

int drvcos = 3;  // 1:직진, 2:우회전, 3:좌회전
String line_number = "A"; // 사거리 A:하, B:좌, C:상, D:우

int led_pin = A5; // LED핀
int btn1 = A4;    // 버튼핀

int RightMotor_E_pin = 5;  // 오른쪽 모터의 Enable & PWM
int LeftMotor_E_pin = 6;   // 왼쪽 모터의 Enable & PWM
int RightMotor_1_pin = 7;  // 오른쪽 바퀴 모터 제어선 IN1
int RightMotor_2_pin = 8;  // 오른쪽 바퀴 모터 제어선 IN2
int LeftMotor_3_pin = 9;  // 왼쪽 바퀴 모터 제어선 IN3
int LeftMotor_4_pin = 10;  // 왼쪽 바퀴 모터 제어선 IN4

int L_Line = A3;  // 왼쪽 라인트레이서 센서 A5 핀에 연결
int C_Line = A2;  // 가운데 라인트레이서 센는 A4 핀에 연결
int R_Line = A1;  // 오른쪽 라인트레이서 센서 A3 핀에 연결
int LL_Line = A0;  // 제일 왼쬑 라인트레이서 센서 A0 핀에 연결

//좌우 모터 속도 조절, 설정 가능 최대 속도 : 255
int R_MotorSpeed = 0;  // 오른쪽 모터 속도
int L_MotorSpeed = 0;  // 왼쪽 모터 속도

int SL = 1;
int SC = 1;
int SR = 1;      // 왼쪽, 가운데, 오른쪽 센서 명령
int SLL = 1;

int count = 0; // 라인트레이서 4개가 모두 HIGH일 때 +1

void mode_change(String t_mode){ // 수신형 -> 송신형
  if(t_mode == "receive"){ // 수신 모드
    radio.setPALevel(RF24_PA_HIGH); // 전원공급에 관한 파워레벨을 설정합니다. 모듈 사이가 가까우면 최소로 설정합니다.
    radio.openReadingPipe(0, address); // 메인 주소에게 수신
    radio.startListening(); // 모듈을 수신기로 설정
  }
  else if(t_mode == "send"){ // 송신 모드
    radio.openWritingPipe(address); // 이전에 설정한 5글자 문자열인 데이터를 보낼 수신의 주소를 설정
    delay(1000);
    radio.setPALevel(RF24_PA_HIGH); // 전원공급에 관한 파워레벨을 설정합니다. 모듈 사이가 가까우면 최소로 설정합니다.
    radio.stopListening();  // 모듈을 송신기로 설정
    delay(1000); 
  }
}
void send_message(String msg){ // 보내는 메시지 함수
    char *msgRF; // 문자 메시지
    msgRF = msg.c_str(); // msg(string)를 char *로 변경 
    radio.write((uint8_t *)msgRF, strlen(msgRF)); // 해당 메시지를 수신자에게 보냄
}

void setup() {  // 설정값

  EduServo.attach(2);  // 서보모터 PWM 디지털입출력 2번핀 연결
  radio.begin(); // 라디오 채널 사용
  mode_change(now_mode); // 송수신 변경
  pinMode(RightMotor_E_pin, OUTPUT);
  pinMode(LeftMotor_E_pin, OUTPUT);
  pinMode(RightMotor_1_pin, OUTPUT);
  pinMode(RightMotor_2_pin, OUTPUT);
  pinMode(LeftMotor_3_pin, OUTPUT);
  pinMode(LeftMotor_4_pin, OUTPUT);
  pinMode(btn1, INPUT_PULLUP);// 풀업 스위치로 구성합니다 ( 안눌렸을때 high유지 , 눌리면 low )
  pinMode(led_pin, OUTPUT); // LED 출력
  Serial.begin(9600);  // PC와의 시리얼 통신 9600bps로 설정
}

String msg = ""; // 보내는 문자열
char text[32] = ""; // nrf24보내는 메시지 기본 사이즈 크기

void loop() {
    
  int btn_value1 = digitalRead(btn1); // 버튼값 입력
  
  int L = digitalRead(L_Line);
  int C = digitalRead(C_Line);
  int R = digitalRead(R_Line);
  int LL = digitalRead(LL_Line);

  
  if(btn_value1 == 1){  // 버튼을 누르면
    now_mode = "send";  // 송신 모드로 변경
    delay(2000);
    Serial.println("mode changed to send");
   }
   if(now_mode == "receive"){ // 평상시에는 수신 모드
     byte pipeNum = 0;
     if(radio.available(&pipeNum)) { // 수신 받은 메시지가 있을 때
      radio.read(text, sizeof(text)); // 수신 메시지을 읽음
      String msg = String(text); // 수신 메시지를 저장
      
      if(pipeNum == 0){ // 메인 기기에서 메시지가 왔을 때
        Serial.print("received by Main");
        }
     Serial.println(msg); // 수신된 메시지 출력
                         // (메인 기기의 버튼을 눌렀을 때 보내는 정지 신호)
     if(msg.indexOf("turn_on_led") > -1){ // 내장된 정지신호와 일치할 때
      Serial.println("turn_on_led!!!!");
      digitalWrite(led_pin, HIGH); // LED(빨간불) ON
      analogWrite(RightMotor_E_pin, 0);
      analogWrite(LeftMotor_E_pin, 0);
      delay(5000); // 5초 뒤
      Serial.println("turn_off_led!");
      digitalWrite(led_pin, LOW); // LED OFF
     }
    }
// 운행모드
// 0번
   else if (drvcos == 1) {  // 사거리 직진

      L_MotorSpeed = 170;
      R_MotorSpeed = 175;
      
      if (LL == LOW && L == LOW && C == LOW && R == LOW) {  //  0 0 0 0
        L = SL;
        C = SC;
        R = SR;  // 라인을 찾아 돈다
        LL = SLL;
      }

      if (L == LOW && C == HIGH && R == LOW) {
        motor_role(HIGH, HIGH); 
      }
      else if (LL == LOW && L==HIGH && C==HIGH && R==HIGH) {  // ?010 or 0111
        motor_role(HIGH, HIGH);  // 직진 명령
      }
      
      else if (L == LOW && R == HIGH ){          // ?001, ?011
        motor_role(HIGH, HIGH);
        R_MotorSpeed = 120;
      }
 
      else if (L == HIGH && R == LOW) {    // ?100, ?110 or ?101
        motor_role(HIGH, HIGH);
        L_MotorSpeed = 100;
      }
      else if (LL == HIGH && L==HIGH && C==HIGH && R==HIGH){ // 1 1 1 1
        count += 1;
        if(count > 5){
        analogWrite(RightMotor_E_pin, 0);
        analogWrite(LeftMotor_E_pin, 0);
        delay(10000);
        }
      }
      SL = L;
      SC = C;
      SR = R;  // 각 함수에 센서값 대입
      SLL = LL;
    }
// 운행모드 1번

  else if (drvcos == 2) {  // 사거리 우회전

      R_MotorSpeed = 200;  // 오른쪽 모터 속도
      L_MotorSpeed = 200;  // 왼쪽 모터 속도
      
      if (L == LOW && C == LOW && R == LOW) {  // ? 0 0 0
        L = SL;
        C = SC;
        R = SR;  // 라인을 찾아 돈다
        LL = SLL;
      }

      if (L == LOW && C == HIGH && R == LOW) {  // ? 0 1 0
        
        motor_role(HIGH, HIGH);                 // 직진 명령
      }

      else if ((L == LOW && R == HIGH) || (LL == LOW && L == HIGH && R == HIGH)) {  // ? 0 ? 1 or 0 1 ? 1
        L_MotorSpeed = 210;  // 왼쪽 모터 속도
        R_MotorSpeed = 200;  // 오른쪽 모터 속도
        motor_role(LOW, HIGH);   // 우회전 명령

      }
      else if (LL == HIGH && L==HIGH && C==HIGH && R==HIGH){ // 1 1 1 1
        count += 1;
        if(count > 5){
        analogWrite(RightMotor_E_pin, 0);
        analogWrite(LeftMotor_E_pin, 0);
        delay(10000);
        }
      
      }
      SL = L;
      SC = C;
      SR = R;  // 각 함수에 센서값 대입
      SLL= LL;
    }
  

// 운행모드 2번

  else if (drvcos == 3) {  // 사거리 좌회전
  
      L_MotorSpeed = 205;
      R_MotorSpeed = 190;
      
      if (L == LOW && C == LOW && R == LOW && LL == LOW) {  // 0 0 0
        L = SL;
        C = SC;
        R = SR;  // 라인을 찾아 돈다
        LL = SLL;
      }

      if (L == LOW && C == HIGH && R == LOW) {  // 0 1 0
        motor_role(HIGH, HIGH);                 // 직진 명령
      }

      else if ((L == HIGH && R == LOW) || 
      (LL==LOW && L == HIGH && R == HIGH)) {  // 1 0 0, 1 1 0
        motor_role(HIGH, LOW);                                         // 좌회전 명령
      }
      
      else if (LL == HIGH && L == HIGH && C == HIGH && R == HIGH){
        count += 1;
        if(count > 5){
        analogWrite(RightMotor_E_pin, 0);
        analogWrite(LeftMotor_E_pin, 0);
        delay(10000);
        }
      }
      SL = L;
      SC = C;
      SR = R;  // 각 함수에 센서값 대입
      SLL = LL;
    }
  
 }
  else{ // 버튼을 눌렀을 때 송신모드
    if(now_mode == "send"){  // 메인 주소로 보내는 코드
      mode_change(now_mode); // 송신 모드로 변경
      String new_msg = "1_RC_CAR"; // 1번 차에서 보내는 메시지를 알림
      new_msg = new_msg + ", " + line_number + "-" + String(drvcos); // 보내는 메시지는 문자와 숫자 조합 
      send_message(new_msg); // 메인 주소로 메시지를 보냄
      }
    now_mode = "receive"; // 송신 후 다시 수신모드로 전환
    mode_change(now_mode); // 수신모드로 전환
    Serial.println("now mode : receive"); // 현재 모드를 출력
    }
}


void motor_role(int R_motor, int L_motor) {  // 모터 실행 함수
  digitalWrite(RightMotor_1_pin, R_motor);
  digitalWrite(RightMotor_2_pin, !R_motor);
  digitalWrite(LeftMotor_3_pin, L_motor);
  digitalWrite(LeftMotor_4_pin, !L_motor);

  analogWrite(RightMotor_E_pin, R_MotorSpeed);  // 우측 모터 속도값
  analogWrite(LeftMotor_E_pin, L_MotorSpeed);   // 좌측 모터 속도값
}

void moveFoward() {
  Serial.println("moveFoward");
  digitalWrite(RightMotor_1_pin, HIGH);
  digitalWrite(RightMotor_2_pin, LOW);
  digitalWrite(LeftMotor_3_pin, HIGH);
  digitalWrite(LeftMotor_4_pin, LOW);
  analogWrite(RightMotor_E_pin, R_MotorSpeed);
  analogWrite(LeftMotor_E_pin, L_MotorSpeed);
}
