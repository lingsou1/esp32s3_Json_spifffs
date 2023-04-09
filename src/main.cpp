/*
接线说明:无

程序说明:通过闪存文件(Json数据格式)的解析结果来尝试连接WiFi

注意事项:在线JSON数据解析网站 : https://arduinojson.org/v6/assistant/ (支持代码生成)
         解析JSON数据大的步骤有:
         1)建立DynamicJsonDocument对象
            const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + 60;
            DynamicJsonDocument doc(capacity);
         2)反序列化
          deserializeJson(doc,file);
         3)获取解析数据
          const char* wifi_ssid = doc["wifi"]["ssid"]; // "taichimaker"
          const char* wifi_password = doc["wifi"]["password"]; // "12345678"
  


函数示例:无

作者:灵首

时间:2023_3_27

*/
#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFiMulti.h>
#include <ArduinoJson.h>
#include <WiFi.h>
 

String file_name = "/wifi_config.json";    //宏定义文件地址,同时 "data/wifi_config.json" 这个地址是不能找到相关的文件的
                                           //,应该是默认SPIFFS文件就在自己新建的文件夹 data中了
WiFiMulti wifi_multi;  //建立WiFiMulti 的对象,对象名称是 wifi_multi


/*
# brief 对闪存文件进行格式化操作
# param 无
# retval 无
*/
void SPIFFS_Format_init(){
  Serial.print("\nSPIFFS format begin");
  SPIFFS.format();    //文件格式化的函数
  Serial.print("\nSPIFFS format finshed !!!");

}



/*
# brief 启动SPIFFS
# param 无
# retval 无
*/
void SPIFFS_start_init(){
  if(SPIFFS.begin()){
    Serial.print("\nSPIFFS Start!!!");
  }
  else{
    Serial.print("\nSPIFFS Failed to start!!!");
  }
}



/*
# brief 连接WiFi的函数
# param 无
# retval 无
*/
void wifi_multi_con(void){
  int i=0;
  while(wifi_multi.run() != WL_CONNECTED){
    delay(1000);
    i++;
    Serial.print(i);
    Serial.print("  ");
  }
}


/*
# brief 写入自己要连接的WiFi名称及密码,之后会自动连接信号最强的WiFi
# param const char* ssid :以字符数组为格式的ssid
# param  const char* password :以字符数组为格式的passowrd
# retval  无
*/
void wifi_multi_init(const char* ssid,const char* password){
  wifi_multi.addAP(ssid,password);
  wifi_multi.addAP("LINGSOU12","12345678");
  wifi_multi.addAP("LINGSOU1","12345678");
  wifi_multi.addAP("LINGSOU234","12345678");   //通过 wifi_multi.addAP() 添加了多个WiFi的信息,当连接时会在这些WiFi中自动搜索最强信号的WiFi连接
}



void setup() {

  //开启串口
  Serial.begin(9600);
  Serial.print("...");

  //启动SPIFFS
  SPIFFS_start_init();    

  // 建立DynamicJsonDocument对象
  const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + 60;
  DynamicJsonDocument doc(capacity);

  // 建立File对象用于向SPIFFS中的file对象读取信息
  File file = SPIFFS.open(file_name,"r");   
  
  //反序列化
  deserializeJson(doc,file);

  // 获取解析后的数据信息(以字符数组的形式)
  const char* wifi_ssid = doc["wifi"]["ssid"]; // "taichimaker"
  const char* wifi_password = doc["wifi"]["password"]; // "12345678"
  
  //获取解析后的数据信息(以指定格式 如:字符串格式)
  // String wifi_ssid = doc["wifi"]["ssid"].as<String>();
  // String wifi_password = doc["wifi"]["password"].as<String>();

  wifi_multi_init(wifi_ssid,wifi_password);
  wifi_multi_con();

  file.close();   //关闭文件,打开文件进行需要的操作后要及时关闭文件


}

void loop() {
  //测试是否成功连接WiFi
  if(wifi_multi.run() == WL_CONNECTED){
    Serial.print("wifi connected!!!");
    Serial.print("\n");
  }
  delay(2000);
}

