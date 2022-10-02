#include <ESP8266WiFi.h>
#include<ESP8266HTTPClient.h>

/*Put your SSID & Password*/
const char* ssid = "zuk z1";    // Enter SSID here
const char* password = "password";  // Enter Password here

float as2[1000] = {};
float as1[1000] = {};
float as3[1000] = {};
float acm1[1000] = {};
float acs2[1000] = {};
float acs1[1000] = {};
float acs3[1000] = {};
int Sample;
int a = 0;
int b = 0;
int c = 0;

WiFiClient client;
HTTPClient http;
//Sensors input data
const int m1 = D3; //main machine
const int s1 = D1; //subsystem 1
const int s2 = D2; //susystem 2

//Kalman Variables for m1
float m1_varVolt = 1.12184278324081E-05; //calculated variance
float m1_varProcess = 1e-10; //factor determining the agressiveness of the Kalman Filter
float m1_Pc = 0.0;
float m1_G = 0.0;
float m1_P = 1.0;
float m1_Xp = 0.0;
float m1_Zp = 0.0;
float m1_Xe = 0.0;

//Kalman Variables for s1
float s1_varVolt = 1.12184278324081E-05; //calculated variance
float s1_varProcess = 1e-10; //factor determining the agressiveness of the Kalman Filter
float s1_Pc = 0.0;
float s1_G = 0.0;
float s1_P = 1.0;
float s1_Xp = 0.0;
float s1_Zp = 0.0;
float s1_Xe = 0.0;

//Kalman Variables for s2
float s2_varVolt = 1.12184278324081E-05; //calculated variance
float s2_varProcess = 1e-10; //factor determining the agressiveness of the Kalman Filter
float s2_Pc = 0.0;
float s2_G = 0.0;
float s2_P = 1.0;
float s2_Xp = 0.0;
float s2_Zp = 0.0;
float s2_Xe = 0.0;

//Kalman Variables for s3
float s3_varVolt = 1.12184278324081E-05; //calculated variance
float s3_varProcess = 1e-10; //factor determining the agressiveness of the Kalman Filter
float s3_Pc = 0.0;
float s3_G = 0.0;
float s3_P = 1.0;
float s3_Xp = 0.0;
float s3_Zp = 0.0;
float s3_Xe = 0.0;

int m;
int n;
int o;
int p;
int q;
int r;
int s;
int t;
int i;
int j;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
delay(1000);

//Connecting to WiFi network
  Serial.println("Connecting to ");
  Serial.print(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
float x1 = analogRead(m1); //take m1 reading
float x2 = analogRead(s1); //take s1 reading
float x3 = analogRead(s2); //take s2 reading
float x4 = x1 - (x2 + x3);//find s3 reading

 //converting analog values into voltage
float v1 = x1*5.0/1024.0;
float v2 = x2*5.0/1024.0;
float v3 = x3*5.0/1024.0;
float v4 = x4*5.0/1024.0;

//the fluctuation magnitude readings
float f1 = ( v2 - s1_Xe );
float f2 = ( v3 - s2_Xe );
float f3 = ( v4 - s3_Xe );

//Kalman Filtering for m1
m1_Pc = m1_P + m1_varProcess;
m1_G = m1_Pc/(m1_Pc + m1_varVolt); //Gains for m1
m1_P = (1 - m1_G)*m1_Pc;
m1_Xp = m1_Xe;
m1_Zp = m1_Xp;
m1_Xe = m1_G*(v1 - m1_Zp) + m1_Xp; //Kalman Value for m1

//Kalman Filtering for s1
s1_Pc = s1_P + s1_varProcess;
s1_G = s1_Pc/(s1_Pc + s1_varVolt); //Gains for s1
s1_P = (1 - s1_G)*s1_Pc;
s1_Xp = s1_Xe;
s1_Zp = s1_Xp;
s1_Xe = s1_G*(v2 - s1_Zp) + s1_Xp; //Kalman Value for s1

//Kalman Filtering for s2
s2_Pc = s2_P + s2_varProcess;
s2_G = s2_Pc/(s2_Pc + s2_varVolt); //Gains for s2
s2_P = (1 - s2_G)*s2_Pc;
s2_Xp = s2_Xe;
s2_Zp = s2_Xp;
s2_Xe = s2_G*(v3 - s2_Zp) + s2_Xp; //Kalman Value for s2

//Kalman Filtering for s3
s3_Pc = s3_P + s3_varProcess;
s3_G = s3_Pc/(s3_Pc + s3_varVolt); //Gains for s3
s3_P = (1 - s3_G)*s3_Pc;
s3_Xp = s3_Xe;
s3_Zp = s3_Xp;
s3_Xe = s3_G*(v4 - s3_Zp) + s3_Xp; //Kalman Value for s3
//dealing with main machine failure
if(v1 == 0)
{
  n == 0;
  if(m == 0)
  {
  Serial.println("Complete Main Machine Has Stopped");
  
http.begin("http://192.168.43.105:3000/status?machineid=5&status=false");
int httpCode = http.GET();
if (httpCode > 0) { //Check the returning code
 
//Serial.println("Yay");
    }else Serial.println("An error ocurred");
 
 http.end();
  }
  m++;
}

else
{
  m == 0;
  if(n == 0)
 {
  Serial.println("Machine Startup success"); 
 
http.begin("http://192.168.43.105:3000/status?machineid=5&status=true");
int httpCode = http.GET();
if (httpCode > 0) { //Check the returning code
 
//Serial.println("Yay");
    }else Serial.println("An error ocurred");
 
 http.end();
 }
 n++;
}
  
//dealing with subsystem 1 failure
if(v2 == 0)
{
  p == 0;
  if(o == 0)
  {
  Serial.println("Subsystem 1 Has Stopped");
  
http.begin("http://192.168.43.105:3000/status?machineid=2&status=false");
int httpCode = http.GET();
Serial.println(httpCode);
if (httpCode > 0) { //Check the returning code
 http.end(); 
//Serial.println("Yay");
    }else {Serial.println("An error ocurred");
 
}
  }
  o++;
}

else
{
  o == 0;
  if(p == 0)
 {
  Serial.println("Subsystem 1 Startup success");
  
http.begin("http://192.168.43.105:3000/status?machineid=2&status=true");
int httpCode = http.GET();
Serial.println(httpCode);
if (httpCode > 0) { //Check the returning code
 
//Serial.println("Yay");
    }else Serial.println("An error ocurred");
 
  
 }
 p++;
}

//dealing with subsystem 2 failure
if(v3 == 0)
{
  r == 0;
  if(q == 0)
  {
  Serial.println("Subsystem 2 Has Stopped");
  
http.begin("http://192.168.43.105:3000/status?machineid=3&status=false");
int httpCode = http.GET();
if (httpCode > 0) { //Check the returning code
 
//Serial.println("Yay");
    }else Serial.println("An error ocurred");
 
 http.end();
  }
  q++;
}

else
{
  q == 0;
  if(r == 0)
 {
  Serial.println("Subsystem 2 Startup success");
  
http.begin("http://192.168.43.105:3000/status?machineid=3&status=true");
int httpCode = http.GET();
if (httpCode > 0) { //Check the returning code
 
//Serial.println("Yay");
    }else Serial.println("An error ocurred");
 
 http.end(); 
 }
 r++;
}

//dealing with subsystem 3 failure
if(v4 == 0)
{
  t == 0;
  if(s == 0)
  {
  Serial.println("Subsystem 3 Has Stopped");
  
http.begin("http://192.168.43.105:3000/status?machineid=4&status=false");
int httpCode = http.GET();
if (httpCode > 0) { //Check the returning code
 
//Serial.println("Yay");
    }else Serial.println("An error ocurred");
 
 http.end();
  }
  s++;
}
else
{
  s == 0;
  if(t == 0)
 {
  Serial.println("Subsystem 3 Startup success");
  
http.begin("http://192.168.43.105:3000/status?machineid=4&status=true");
int httpCode = http.GET();
if (httpCode > 0) { //Check the returning code
 
//Serial.println("Yay");
    }else Serial.println("An error ocurred");
 
 http.end(); 
 }
 t++;
}

//Dealing with Subsystem 1 fluctuations
if(v2 >= s1_Xe + 1 && v2 <= s1_Xe - 1 )
{
 a++;
 as1[a] = {f1};
 acs1[a] = {i};
 if(a == 800)
 {
  Serial.println("Subsystem 1 array memory full, transmitting data");
  int qwer = 0; int tyui = 0;
  String ghj = "";
  for(;qwer<800;qwer++){
      ghj = ghj + acs1[qwer] + "," + as1[qwer]+ ",";
    }
    //ghj is the string
    String P = "http://192.168.43.105:3001/arduino?" + ghj;
    http.begin(P);
int httpCode = http.GET();
if (httpCode > 0) { //Check the returning code
 
//Serial.println("Yay");
    }else Serial.println("An error ocurred");
 
 http.end();

 }
 else if(a == 1000)
 {
  a==0;
 }
}
//Dealing with subsystem 2 fluctuations
if(v3 >= s2_Xe + 1 && v3 <= s1_Xe - 1)
{
  b++;
  as2[b] = {f2};
  acs2[b] = {i};
  if(b == 800)
  {
    Serial.println("Subsystem 2 array memory full, transmitting data");
  int A = 0; int B = 0;
  String C = "";
  for(;A<800;A++){
      C = C + acs1[A] + "," + as1[A]+ ",";
    }
    //C is the string
    String Q = "http://192.168.43.105:3001/arduino?" + C;
    http.begin(Q);
int httpCode = http.GET();
if (httpCode > 0) { //Check the returning code
 
//Serial.println("Yay");
    }else Serial.println("An error ocurred");
 
 http.end();

  }
else if(b ==1000);
{
  b ==0;
}
}
//Dealing with subsystem 3 fluctuations

if(v4 >= s3_Xe + 1 && v4 <= s2_Xe - 1)
{
  c++;
  as3[b] = {f3};
  acs3[b] = {i};
  if(c == 800)
  {
        Serial.println("Subsystem 3 array memory full, transmitting data");
  int D = 0; int E = 0;
  String F = "";
  for(;D<800;D++){
      F = F + acs1[D] + "," + as1[D]+ ",";
    }
    //F is the string
    String R = "http://192.168.43.105:3001/arduino?" + F;
    http.begin(R);
int httpCode = http.GET();
if (httpCode > 0) { //Check the returning code
 
//Serial.println("Yay");
    }else Serial.println("An error ocurred");
 
 http.end();

  }
  else if(c == 1000)
  {
    c==0;
  }
}
} 
