#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "999";
const char* password = "01234567890";
const char* mqtt_server = "mqtts.heclouds.com";
const int mqtt_port = 1883;
const char* client_id = "stm32";
const char* user = "32ta8B814b";
const char* mqtt_password = "version=2018-10-31&res=products%2F32ta8B814b%2Fdevices%2Fstm32&et=1924833600&method=md5&sign=7RwrS8IiTBDhIEc6lcmu0Q%3D%3D";
int postid =0;
int Air_Status=0;

#define ONENET_TOPIC_PROP_POST "$sys/32ta8B814b/stm32/thing/property/post" //设备--》onenet
#define ONENET_TOPIC_PROP_SET  "$sys/32ta8B814b/stm32/thing/property/set"//onenet-》设备·
#define ONENET_TOPIC_PROP_POST_REPLY "$sys/32ta8B814b/stm32/thing/property/post/reply"//onenet-》设备
#define ONENET_TOPIC_PROP_SET_REPLY  "$sys/32ta8B814b/stm32/thing/property/post/set_reply"//设备--》onenet

WiFiClient espClient;//创建一个wificlient对象
PubSubClient client(espClient);//创建一个PubSubClient对象

void setup_wifi() {
  delay(10);
  //Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  randomSeed(micros());
 // Serial.println("");
  //Serial.println("WiFi connected");
 //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
}
//当订阅的主题有消息时，调用此函数
void callback(char* topic, byte* payload, unsigned int length) {
  //Serial.print("Message arrived [");
  //Serial.print(topic);
  //Serial.print("] ");
  //for (unsigned int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
  //}
 // Serial.println();
  if(strcmp(topic,ONENET_TOPIC_PROP_SET)==0){
    DynamicJsonDocument doc(100);//创建一个json文档，用于解析消息内容
    DeserializationError error =deserializeJson(doc,payload);//解析消息内容，将消息内容存储到doc中，返回解析结果，成功返回0，失败返回其他值
    if(error){
      return;
    }
    JsonObject setAlinkMsgObj=doc.as<JsonObject>();//获取json文档的根对象
    JsonObject params = setAlinkMsgObj["params"];//获取params对象
    if(params.containsKey("air"))
    {
      Air_Status = params["air"];
      if(Air_Status){
        Serial.println("A1");
      }
      else{
        Serial.println("A0");
      }
    }
    if(params.containsKey("led"))
    {
      Air_Status = params["led"];
      if(Air_Status){
        Serial.println("L1");
      }
      else{
        Serial.println("L0");
      }
    }
    if(params.containsKey("window"))
    {
      Air_Status = params["window"];
      if(Air_Status){
        Serial.println("W1");
      }
      else{
        Serial.println("W0");
      }
    }
    if(params.containsKey("beep"))
    {
      Air_Status = params["beep"];
      if(Air_Status){
        Serial.println("B1");
      }
      else{
        Serial.println("B0");
      }
    }
    //serializeJsonPretty(setAlinkMsgObj, Serial);//打印JSON文档
		String str = setAlinkMsgObj["id"];//获取消息ID
		char SendBuf[100];//发送缓冲区
		sprintf(SendBuf, "{\"id\":\"%s\",\"code\":200,\"msg\":\"success\"}",str.c_str());//设置响应消息
		delay(100);
		if(client.publish(ONENET_TOPIC_PROP_SET_REPLY, SendBuf))//发送响应消息,不知道为什么，这里发送成功的，但是OneNET平台上没有收到
		{
			//Serial.println("Send set reply success!");
		}
		else
		{
			//Serial.println("Send set reply failed!");
		}
  }
}

void reconnect() {
  while (!client.connected()) {
    //Serial.print("Attempting MQTT connection...");
    if (client.connect(client_id, user, mqtt_password)) {
      //Serial.println("connected");
      client.subscribe(ONENET_TOPIC_PROP_SET);//订阅设备属性设置请求
      client.subscribe(ONENET_TOPIC_PROP_POST_REPLY);//订阅设备属性上报响应
    } else {
      //Serial.print("failed, rc=");
     // Serial.print(client.state());
      //Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (Serial.available() > 0) {
    String data = Serial.readString(); // 读取串口数据
    char msg[50 + data.length() + 1];
    sprintf(msg, "{\"id\":\"%u\",\"version\":\"1.0\",\"params\":%s}",postid++,data.c_str()); // 格式化为OneNET需要的格式
    client.publish(ONENET_TOPIC_PROP_POST, msg); // 发布数据
    //Serial.print("Published: ");
    //Serial.println(msg);
  }
}