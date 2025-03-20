// 메인 오더
#include <SPI.h> 
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // SPI 버스에 nRF24L01 라디오를 설정하기 위해 CE, CSN 선언

const byte address[6] = "12345"; // 1번 기기에게 받을 주소
const byte address2[6] = "34567"; // 2번 기기에게 받을 주소 
String now_mode = "receive"; // nRF24L01의 기본 형태를 수신형으로 설정

#define btn1 2 // 1번 버튼 
#define btn2 3 // 2번 버튼

int a_int_value = 0; // a가 1이고
String a_str_value;
int b_int_value = 0; // b가 2일때 b쪽으로 led on 송신 
String b_str_value;

void setup() {
  Serial.begin(9600); // 시리얼 속도는 9600
  radio.begin();      // 라디오 채널 사용
  mode_change(now_mode); // 송수신 변경
  pinMode(btn1,INPUT_PULLUP); // 풀업 스위치로 구성합니다 ( 안눌렸을때 high유지 , 눌리면 low )
  pinMode(btn2,INPUT_PULLUP); // 풀업 스위치로 구성합니다 ( 안눌렸을때 high유지 , 눌리면 low )
}

void mode_change(String t_mode){ // 수신형 -> 송신형
  if(t_mode == "receive"){ // 수신 모드
    radio.setPALevel(RF24_PA_HIGH); // 전원공급에 관한 파워레벨을 설정합니다. 
    
    radio.openReadingPipe(0, address);  // 1번 주소에게 받을 수신 채널 설정
    radio.openReadingPipe(1, address2); // 2번 주소에게 받을 수신 채널 설정
    delay(1000); // 일정시간의 딜레이를 주어야 수신 송신이 원활하게됩니다.
    
    radio.startListening(); // 모듈을 수신기로 설정
    delay(1000); // 일정시간의 딜레이를 주어야 수신 송신이 원활하게됩니다.
  }
  else if(t_mode == "send1"){ // 1번 주소에게 송신 모드
    radio.openWritingPipe(address); // 1번 주소에게 보내는 송신 채널
    delay(500);
    radio.setPALevel(RF24_PA_HIGH); //전원공급에 관한 파워레벨을 설정합니다. 모듈 사이가 가까우면 최소로 설정합니다.
    radio.stopListening();  // 모듈을 송신기로 설정
    delay(500);
  }
    else if(t_mode == "send2"){ // 2번 주소에게 송신 모드
    radio.openWritingPipe(address2); // 2번 주소에게 보내는 송신 채널
    delay(500);
    radio.setPALevel(RF24_PA_HIGH); //전원공급에 관한 파워레벨을 설정합니다. 모듈 사이가 가까우면 최소로 설정합니다.
    radio.stopListening();  // 모듈을 송신기로 설정
    delay(500);
  }
}

void send_message(String msg){ // 보낼 메시지 설정
    char *msgRF; // 문자 메시지
    msgRF = msg.c_str(); // msg(string)를 char *로 변경 
    radio.write((uint8_t *)msgRF, strlen(msgRF)); // 해당 메시지를 수신자에게 보냄
}

String msg = ""; // 보내는 문자열
char text[32] = ""; // nrf24보내는 메시지 기본 사이즈 크기 

void loop() {
  int btn_value1 = digitalRead(btn1); // 1번 버튼값 입력
  int btn_value2 = digitalRead(btn2); // 2번 버튼값 입력
  
  if(btn_value1 == 1){ // 1번 주소를 향한 버튼이 눌리면 
    now_mode = "send1"; // 1번 주소로 송신 모드
    Serial.println("mode changed to send1"); // 1번 주소에게 보내는 것을 송출
  }
  else if(btn_value2 == 1){ // 2번 주소를 향한 버튼이 눌리면 
                            //(버튼두개는 동시에 수신 못하도록 막음 if-else if문으로)
    now_mode = "send2"; // 2번 주소로 송신 모드
    Serial.println("mode changed to send2"); // 2번 주소에게 보내는 것을 송출
  }
  // 경로 분석
  else if(a_str_value.indexOf("A") > -1){ // 1번 차가 A지점에서 출발
    if(b_str_value.indexOf("B") > -1){ // 2번 차가 B지점에서 출발
     if(
      ((a_int_value == 1) and ((b_int_value == 1) or (b_int_value == 3))) // A:직진 -> B:직진, 좌회전
      or ((a_int_value == 2) and (b_int_value == 1)) // A:우회전 -> B:직진
      or ((a_int_value == 3) and ((b_int_value == 1) or (b_int_value == 3))) // A:좌회전 -> B:직진
      ){ // 수신조건을 만족할 경우
       a_int_value = 0;
       b_int_value = 0;    // 초기화
       now_mode = "send2"; // 2번으로 송신 
       Serial.println("mode changed to send2");
     } 
    } 
    else if(b_str_value.indexOf("C") > -1){ // 2번 차가 C지점에서 출발
      if(
        ((a_int_value == 1) and (b_int_value == 3)) // A:직진 -> C:좌회전
        or ((a_int_value == 2) and (b_int_value == 3)) // A:우회전 -> C:좌회전
        or (a_int_value == 3) // A:좌회전 -> C:직진, 우회전, 좌회전(모두)
        ){ // 수신조건을 만족할 경우
          a_int_value = 0;
          b_int_value = 0;    // 초기화
          now_mode = "send2"; // 2번으로 송신 
          Serial.println("mode changed to send2");
         }
     }
     else if(b_str_value.indexOf("D") > -1){ // 2번 차가 D지점에서 출발
      if(
        (a_int_value == 1) // A:직진 -> D:직진, 우회전, 좌회전(모두)
        or ((a_int_value == 3) and ((b_int_value == 1) or (b_int_value == 3))) // A:좌회전 -> D:직진, 좌회전
        ){ // 수신조건을 만족할 경우
          a_int_value = 0;
          b_int_value = 0;    // 초기화
          now_mode = "send2"; // 2번으로 송신 
          Serial.println("mode changed to send2");
      }
     }
  }
  
  
  if(now_mode == "receive"){ // 평상시에는 수신 모드 
    byte pipeNum = 0;
    if (radio.available(&pipeNum)) { // 수신 받은 메시지가 있을 때
      radio.read(text, sizeof(text)); // 수신 메시지을 읽음
      String msg = String(text);  // 수신 메시지를 저장
      int one = msg.indexOf(", "); // 콤마 위치
      int two = msg.indexOf("-"); // 바 위치
      int length_ = msg.length(); // 문자열 길이
      
      String str1 = msg.substring(0, one); // 첫 번째 토큰 : 수신한 문자열 
      String str2 = msg.substring(one + 1, two); // 두 번째 토큰 : 수신한 정수
      String str3 = msg.substring(two + 1, length_);
      int new_number = str3.toInt(); // 수신한 문자를 숫자로 변경
      
      if(pipeNum == 0){ // 1번 주소에서 보낸 신호가 있을 때
        Serial.print("received by sender1 : ");  
        Serial.print(str1); // 받은 문자 메시지 송출
        Serial.print(" / line : "); // 받은 메시지 구분
        Serial.print(str2); // 받은 정수 메시지 송출
        a_str_value = str2;
        Serial.print(" / course : "); // 받은 메시지 구분
        if(new_number == 1)
          Serial.println("직진");
        else if(new_number == 2)
          Serial.println("우회전");
        else if(new_number == 3)
          Serial.println("좌회전");
        a_int_value = new_number; // 1번에서 수신한 숫자
        Serial.println(msg);
        }
      else if(pipeNum == 1){ // 2번 주소에서 보낸 신호가 있을 때
        Serial.print("received by sender1 : ");  
        Serial.print(str1); // 받은 문자 메시지 송출
        Serial.print(" / line : "); // 받은 메시지 구분
        Serial.print(str2); // 받은 정수 메시지 송출
        b_str_value = str2;
        Serial.print(" / course : "); // 받은 메시지 구분
        if(new_number == 1)
          Serial.println("직진");
        else if(new_number == 2)
          Serial.println("우회전");
        else if(new_number == 3)
          Serial.println("좌회전");
        b_int_value = new_number; // 2번에서 수신한 숫자
        Serial.println(msg); // 받은 전체 메시지 송출
        }
      }
  }
  else { // 버튼을 눌렀을 때 송신모드
    if(now_mode == "send1"){  // 1번 주소로 보내는 코드
      mode_change(now_mode);  // 송신 모드로 변경
      send_message("turn_on_led"); // 문자열 보내기 
      Serial.println("send message to send1 turn on led");
      }
    else if(now_mode == "send2"){ // 2번 주소로 보내는 코드 
      mode_change(now_mode);
      send_message("turn_on_led");
      Serial.println("send message to send2 turn on led");
      }
     now_mode = "receive";  // 송신 후 다시 수신모드로 전환
     mode_change(now_mode); // 수신모드로 전환
    }
}
