//**************************************************************************
//   Name: ESP32_MPPT_75_15_v1_wifi_timer_webserver.m 
//   Copyright:  
//   Author: HsiupoYeh 
//   Version: v20240113a
//   Description: 
//   使用模塊: NodeMCU-32S (CH340/CP2102)
//   會用到板上的LED燈，從線路圖可知，使用GPIO02，內建函式庫定義好了LED_BUILTIN。LOW=滅，HIGH=亮。
//**************************************************************************

//--
String FW_VERSION_str=String("v20240113a");
//--
#include <WiFi.h> 
const char *ssid     = "RUT200_7974"; //ssid:網路名稱 //"TP-Link_149C";
const char *password = "d0X1Wtz3"; //pasword：網路密碼 //"41384029";
int wifi_connect_led_count=0;
bool need_display_wifi_connected_info=true;
bool need_check_wifi_status=false;
bool need_check_system_running_log=false;
//--
#include <WebServer.h>
WebServer server(80);
//--
hw_timer_t * timer0;   // 宣告硬體計時器物件指標
hw_timer_t * timer1;   // 宣告硬體計時器物件指標
//hw_timer_t * timer2;   // 宣告硬體計時器物件指標
hw_timer_t * timer3;   // 宣告硬體計時器物件指標
portMUX_TYPE timerMux0 = portMUX_INITIALIZER_UNLOCKED; // 用於ISR的保護機制，確保程式碼運行不被打斷
portMUX_TYPE timerMux1 = portMUX_INITIALIZER_UNLOCKED; // 用於ISR的保護機制，確保程式碼運行不被打斷
//portMUX_TYPE timerMux2 = portMUX_INITIALIZER_UNLOCKED; // 用於ISR的保護機制，確保程式碼運行不被打斷
portMUX_TYPE timerMux3 = portMUX_INITIALIZER_UNLOCKED; // 用於ISR的保護機制，確保程式碼運行不被打斷
unsigned long timer3_run_count=0; //規劃timer3每一秒加一，這個數值可以當作ESP32開機時間，單位:[秒]。
//--
char Serial_RxChar;
char Serial1_RxChar;
char temp_str[512];
char temp_cmd_str[512];
int temp_str_index=0;
char MPPT_V_str[20];//鋰鐵電池電壓[mV]
char MPPT_I_str[20];//鋰鐵電池電流[mA]
char MPPT_IL_str[20];//負載電流[mA]
char MPPT_VPV_str[20];//太陽能板電壓[mV]
char MPPT_PPV_str[20];//太陽能板功率[W]
unsigned long temp_MPPT_update_index=0;//每秒加一可以用136年不用重置
//--
void ARDUINO_ISR_ATTR onTimer0(){
  //--------------------------------------------------------------- 
  portENTER_CRITICAL_ISR(&timerMux0);// 用於ISR的保護機制，確保程式碼運行不被打斷
  //--
  // 進入中斷，做些事情，請不要放Seria.print之類的工作
  need_check_wifi_status=true;
  //--
  portEXIT_CRITICAL_ISR(&timerMux0);// 用於ISR的保護機制，確保程式碼運行不被打斷
  // 到此中斷工作結束
  //---------------------------------------------------------------
}
//--
void ARDUINO_ISR_ATTR onTimer1(){
  //--------------------------------------------------------------- 
  portENTER_CRITICAL_ISR(&timerMux1);// 用於ISR的保護機制，確保程式碼運行不被打斷
  //--
  // 進入中斷，做些事情，請不要放Seria.print之類的工作
  need_check_system_running_log=true;
  //--
  portEXIT_CRITICAL_ISR(&timerMux1);// 用於ISR的保護機制，確保程式碼運行不被打斷
  // 到此中斷工作結束
  //---------------------------------------------------------------
}
//--
void ARDUINO_ISR_ATTR onTimer3(){
  //--------------------------------------------------------------- 
  portENTER_CRITICAL_ISR(&timerMux3);// 用於ISR的保護機制，確保程式碼運行不被打斷
  //--
  // 進入中斷，做些事情，請不要放Seria.print之類的工作
  timer3_run_count++;
  //--
  portEXIT_CRITICAL_ISR(&timerMux3);// 用於ISR的保護機制，確保程式碼運行不被打斷
  // 到此中斷工作結束
  //---------------------------------------------------------------
}
//--
void handleRoot() {
  //--
  String htmlpage = 
  String("") + 
  "<html>" + "\n" +
  "<head>" + "\n" +
  "    <meta charset=\"utf-8\">" + "\n" +
  "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">" + "\n" +
  "    <title>ESP32 WebServer</title>" + "\n" +
  "    <style>" + "\n" +
  "        html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #333333;}" + "\n" +
  "        body{margin-top: 50px;}" + "\n" +
  "        h1 {margin: 50px auto 30px;}" + "\n" + 
  "        .side-by-side{display: inline-block;vertical-align: middle;position: relative;}" + "\n" +
  "        .esp32-item-container{padding: 10px;}" + "\n" +
  "        .esp32-item-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;}" + "\n" +
  "    </style>" + "\n" +
  "</head>" + "\n" +
  "<body>" + "\n" +
  "    <div>" + "\n" +
  "        <h1>ESP32 WebServer</h1>" + "\n" +
  "    </div>" + "\n" +  
  "    <div class=\"esp32-item-container\">"
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            作者: " + "\n" +
  "        </div>" + "\n" +  
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            HsiupoYeh" + "\n" +
  "        </div>" + "\n" +  
  "    </div>" + "\n" + 
  "    <div class=\"esp32-item-container\">"
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            韌體版本: " + "\n" +
  "        </div>" + "\n" +  
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            " + FW_VERSION_str + "\n" +
  "        </div>" + "\n" +  
  "    </div>" + "\n" + 
  "    <div class=\"esp32-item-container\">"
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            ESP32 運行時間: " + "\n" +
  "        </div>" + "\n" +  
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            " + String(timer3_run_count) + " 秒" + "\n" +
  "        </div>" + "\n" +  
  "    </div>" + "\n" +  
  "    <div class=\"esp32-item-container\">"
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            ESP32 IP: " + "\n" +
  "        </div>" + "\n" +  
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            " + WiFi.localIP().toString() + "\n" +
  "        </div>" + "\n" +  
  "    </div>" + "\n" +  
  "    <div class=\"esp32-item-container\">"
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            ESP32 MAC: " + "\n" +
  "        </div>" + "\n" +  
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            " + String(WiFi.macAddress()) + "\n" +
  "        </div>" + "\n" +  
  "    </div>" + "\n" +  
  "    <div class=\"esp32-item-container\">"
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            Client IP: " + "\n" +
  "        </div>" + "\n" +  
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            " + server.client().remoteIP().toString() + "\n" +
  "        </div>" + "\n" +  
  "    </div>" + "\n" + 
  "    <div class=\"esp32-item-container\">"
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            MPPT 更新索引: " + "\n" +
  "        </div>" + "\n" +  
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            " + String(temp_MPPT_update_index) + "\n" +
  "        </div>" + "\n" +  
  "    </div>" + "\n" + 
  "    <div class=\"esp32-item-container\">"
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            鋰鐵電池-電壓: " + "\n" +
  "        </div>" + "\n" +  
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            " + MPPT_V_str + "[mV]\n" +
  "        </div>" + "\n" +  
  "    </div>" + "\n" + 
  "    <div class=\"esp32-item-container\">"
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            鋰鐵電池-說明: " + "\n" +
  "        </div>" + "\n" +  
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            電壓用來評估電池電量，13600[mV]接近充飽，11800[mV]剩餘電量約20%，停止供電。\n" +
  "        </div>" + "\n" +  
  "    </div>" + "\n" + 
  "    <div class=\"esp32-item-container\">"
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            鋰鐵電池-電流: " + "\n" +
  "        </div>" + "\n" +  
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            " + MPPT_I_str + "[mA]\n" +
  "        </div>" + "\n" +  
  "    </div>" + "\n" + 
  "    <div class=\"esp32-item-container\">"
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            鋰鐵電池-說明: " + "\n" +
  "        </div>" + "\n" +  
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            電流用來評估耗電量，負值表示整體為耗電中，正值表示整體為充電中。\n" +
  "        </div>" + "\n" +  
  "    </div>" + "\n" + 
  "    <div class=\"esp32-item-container\">"
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            太陽能板-電壓: " + "\n" +
  "        </div>" + "\n" +  
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            " + MPPT_VPV_str + "[mV]\n" +
  "        </div>" + "\n" +  
  "    </div>" + "\n" + 
  "    <div class=\"esp32-item-container\">"
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            太陽能板-功率: " + "\n" +
  "        </div>" + "\n" +  
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            " + MPPT_PPV_str + "[W]\n" +
  "        </div>" + "\n" +  
  "    </div>" + "\n" + 
  "    <div class=\"esp32-item-container\">"
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            負載電流: " + "\n" +
  "        </div>" + "\n" +  
  "        <div class=\"side-by-side esp32-item-text\">" + "\n" +
  "            " + MPPT_IL_str + "[mA]\n" +
  "        </div>" + "\n" +  
  "    </div>" + "\n" + 
  "</body>" + "\n" +
  "</html>"+ "\n";
  //--
  // 回傳內容
  server.send(200, "text/html", htmlpage);
  //--  
}
//--
void handleInfo() {
  //--
  String json_str = 
  String("") + 
  "{" +
  "\"From\":\"ESP32S\"" +
  "," +
  "\"To\":\"CLIENT\"" +
  "," +
  "\"FW_VERSION\":\"" + FW_VERSION_str + "\"" +
  "," +
  "\"Author\":\"HsiupoYeh\"" +
  "," +
  "\"ESP32_Uptime\":\"" + String(timer3_run_count) + "\"" +  
  "," +
  "\"ESP32_IP\":\"" + WiFi.localIP().toString() + "\"" +  
  "," +
  "\"ESP32_MAC\":\"" + String(WiFi.macAddress()) + "\"" +  
  "," +
  "\"Client_IP\":\"" + server.client().remoteIP().toString() + "\"" +  
  "," +
  "\"MPPT_Update\":\"" + String(temp_MPPT_update_index) + "\"" +  
  "," +
  "\"MPPT_V\":\"" + MPPT_V_str + "\"" +  
  "," +
  "\"MPPT_I\":\"" + MPPT_I_str + "\"" +  
  "," +
  "\"MPPT_VPV\":\"" + MPPT_VPV_str + "\"" +  
  "," +
  "\"MPPT_PPV\":\"" + MPPT_PPV_str + "\"" +  
  "," +
  "\"MPPT_IL_str\":\"" + MPPT_IL_str+ "\"" +     
  "}";
  
  //--
  // 回傳內容
  server.send(200, "application/json", json_str);
  //--  
}
//--
void handleNotFound() {
  String message = "404 Page Not Found!\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
//--
void setup() {
  //---------------------------------------------------------------
  // 初始化LED燈，設定GPIO為輸出
  pinMode(LED_BUILTIN, OUTPUT);
  //--
  // 開機燈號，亮50ms，滅50ms，亮滅共10次。效果會讓LED燈閃爍3次。
  for(int i=1;i<=3;i++)
  { 
    digitalWrite(LED_BUILTIN, HIGH);// 板上LED燈亮
    delay(50);// 等待50[ms]
    digitalWrite(LED_BUILTIN, LOW);// 板上LED燈滅
    delay(50);// 等待950[ms]
  }
  //---------------------------------------------------------------
  //---------------------------------------------------------------
  // Debug訊息(UART0):
  // 在函式庫裡有定義了三個物件 Serial，Serial1，Serial2 可以用來控制UART。
  // 在函式庫定義裡不指定PIN腳的狀況下，Serial是指板上USB上的UART0
  // ，對應TX0、RX0，GPIO1->TX0，GPIO3->RX0
  // UART0初始化，函式庫中是Serial物件。沒填的參數就是預設。
  Serial.begin(115200);
  //--
  // 印出歡迎訊息:
  Serial.print("\n");//第一行常有亂碼，先使用換行字元
	Serial.print("*************************************\n");
	Serial.print("* 韌體名稱: ESP32_MPPT_75_15_v3_wifi_timer \n"); 
  Serial.print(String("") + "* 版本: " + FW_VERSION_str + " \n");
	Serial.print("* Author: HsiupoYeh \n");
	Serial.print("*************************************\n");
  //---------------------------------------------------------------
  //---------------------------------------------------------------
  // MPPT控制器用的UART1
  // 在函式庫裡有定義了三個物件 Serial，Serial1，Serial2 可以用來控制UART。
  // 參考了線路圖找出可用的GPIO PIN，選擇了GPIO26->TX1，GPI25->RX1
  // UART1初始化，函式庫中是Serial1物件。自己指定參數就可以選擇自己要用的GPIO。
  Serial1.begin(19200, SERIAL_8N1, 25, 26);
  //--
  // 印出歡迎訊息:
  Serial1.print("\n");//第一行常有亂碼，先使用換行字元
  Serial1.print("This is Serial1 for MPPT!\n");
  //---------------------------------------------------------------
  //---------------------------------------------------------------
  // LCD觸碰螢幕用的UART2
  // 在函式庫裡有定義了三個物件 Serial，Serial1，Serial2 可以用來控制UART。
  // 參考了線路圖找出可用的GPIO PIN，選擇了GPIO17->TX2，GPIO16->RX2
  // UART1初始化，函式庫中是Serial1物件。自己指定參數就可以選擇自己要用的GPIO。
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  //--
  // 印出歡迎訊息:
  Serial2.print("\n");//第一行常有亂碼，先使用換行字元
  Serial2.print("This is Serial1 for MPPT!\n");
  //--
  // 傳送HMI命令，要用write才能處理不可見字元。
  sprintf(temp_cmd_str,"t1.txt=\"Hello\"\xff\xff\xff");
  Serial2.write(temp_cmd_str);
  sprintf(temp_cmd_str,"t1.txt=\"Hello\"\xff\xff\xff");
  Serial2.write(temp_cmd_str);  
  //---------------------------------------------------------------
  Serial.print("--\n");
  //---------------------------------------------------------------
  // Timer0初始化
  //--
  Serial.print("Timer0初始化...\n");
  // 設定Timer0的頻率 = 1Mhz
  timer0 = timerBegin(1000000);
  // 綁定中斷函數
  timerAttachInterrupt(timer0, &onTimer0);
  // 設定中斷警報器:
  // 第1個參數: Timer指標。
  // 第2個參數: 間隔多久，單位[us]。1000000[us]=1000[ms]=1[sec]。
  // 第3個參數: 使否重複執行。true=重複執行，false=不重複執行。
  // 第4個參數: 執行限制次數。0=無限制。
  timerAlarm(timer0, 1000000, true, 0);
  //--
  Serial.print("Timer0初始化...完成!\n");
  //---------------------------------------------------------------
  Serial.print("--\n");
  //---------------------------------------------------------------
  // Timer1初始化
  //--
  Serial.print("Timer1初始化...\n");
  // 設定Timer1的頻率 = 1Mhz
  timer1 = timerBegin(1000000);
  // 綁定中斷函數
  timerAttachInterrupt(timer1, &onTimer1);
  // 設定中斷警報器:
  // 第1個參數: Timer指標。
  // 第2個參數: 間隔多久，單位[us]。500000[us]=500[ms]=0.5[sec]。
  // 第3個參數: 使否重複執行。true=重複執行，false=不重複執行。
  // 第4個參數: 執行限制次數。0=無限制。
  timerAlarm(timer1, 5000000, true, 0);
  //--
  Serial.print("Timer1初始化...完成!\n");
  //---------------------------------------------------------------
  Serial.print("--\n");
  //---------------------------------------------------------------
  // Timer3初始化
  //--
  Serial.print("Timer3初始化...\n");
  // 設定Timer1的頻率 = 1Mhz
  timer3 = timerBegin(1000000);
  // 綁定中斷函數
  timerAttachInterrupt(timer3, &onTimer3);
  // 設定中斷警報器:
  // 第1個參數: Timer指標。
  // 第2個參數: 間隔多久，單位[us]。1000000[us]=1000[ms]=1[sec]。
  // 第3個參數: 使否重複執行。true=重複執行，false=不重複執行。
  // 第4個參數: 執行限制次數。0=無限制。
  timerAlarm(timer3, 1000000, true, 0);
  //--
  Serial.print("Timer3初始化...完成!\n");
  //---------------------------------------------------------------
  Serial.print("--\n");
  //---------------------------------------------------------------  
  // 啟動WIFI(預設就是STA模式，以DHCP取得路由器配給的IP)
  WiFi.begin(ssid, password);
  Serial.println(String("WiFi: 開始連線到無線網路 SSID = ")+ssid+String(" PASS = ")+password);
  need_display_wifi_connected_info=true;  
  //---------------------------------------------------------------
  Serial.print("--\n");
  //---------------------------------------------------------------
  server.on("/", handleRoot);
  server.on("/info", handleInfo);
  server.onNotFound(handleNotFound);
  server.enableDelay(true);
  server.begin();
  Serial.println("WebServer: 簡單HTTP網頁伺服器啟動...");
  //---------------------------------------------------------------
  Serial.print("--\n");
}

void loop() {
  //---------------------------------------------------------------
  //檢查 WIFI狀態
  if (need_check_wifi_status==true)
  {
    //---------------------------------------------------------------
    // WiFi狀態意義:
    // 0=WL_IDLE_STATUS      (準備中)
    // 1=WL_NO_SSID_AVAIL    (沒有找到SSID)
    // 2=WL_SCAN_COMPLETED   (掃SSID完畢)
    // 3=WL_CONNECTED        (連線成功)
    // 4=WL_CONNECT_FAILED   (連線失敗)
    // 5=WL_CONNECTION_LOST  (連線丟失)
    // 6=WL_DISCONNECTED     (主動斷線)
    //---------------------------------------------------------------
    if((WiFi.status() != WL_CONNECTED)) 
    {
      //--
      //亮1ms後滅，效果會讓LED燈不要這麼亮的閃爍。
      digitalWrite(LED_BUILTIN, HIGH);// 板上LED燈亮
      delay(1);// 等待1[ms]
      digitalWrite(LED_BUILTIN, LOW);// 板上LED燈滅
      //--
      wifi_connect_led_count++;
      Serial.println("WiFi: WifiConnect LED Count: "+String(wifi_connect_led_count)+ " ,Wifi Status = "+String(WiFi.status()));  
      if (wifi_connect_led_count>=5)
      {
        Serial.print("--\nWiFi: 到達最大等待次數!準備重新連線:\n");
        wifi_connect_led_count=0;
        WiFi.disconnect();//斷線（初始化的意思）  
        WiFi.reconnect();
        need_display_wifi_connected_info=true;
      }
      //--
    }
    else if (need_display_wifi_connected_info==true && (WiFi.status() == WL_CONNECTED))
    {
      Serial.print("\n--\n"); 
      Serial.print("WiFi: 連線成功! 資訊如下:\n"); 
      WiFi.printDiag(Serial); //顯示WiFi連線狀態資訊：工作模式、Channel、SSID、Passphrase、BSSID
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      Serial.print("ESP Mac Address: ");
      Serial.println(WiFi.macAddress());
      Serial.print("Subnet Mask: ");
      Serial.println(WiFi.subnetMask());
      Serial.print("Gateway IP: ");
      Serial.println(WiFi.gatewayIP());
      Serial.print("DNS: ");
      Serial.println(WiFi.dnsIP());
      Serial.print("--\n"); 
      need_display_wifi_connected_info=false;
      //--
      digitalWrite(LED_BUILTIN, HIGH);// 板上LED燈亮
      //--
    }
    //--    
    portENTER_CRITICAL_ISR(&timerMux0);// 用於ISR的保護機制，確保程式碼運行不被打斷
    need_check_wifi_status=false;    
    portEXIT_CRITICAL_ISR(&timerMux0);// 用於ISR的保護機制，確保程式碼運行不被打斷
    //--
  }
  //---------------------------------------------------------------

  //---------------------------------------------------------------
  //檢查系統運行並印出紀錄
  if (need_check_system_running_log==true && false)
  {
    //Serial.print("Log: Running...\n"); 
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.print("Log: IP address: ");
      Serial.println(WiFi.localIP());
    }
    else
    {
      Serial.print("Log: IP address: Null\n");
    }
    //--    
    portENTER_CRITICAL_ISR(&timerMux1);// 用於ISR的保護機制，確保程式碼運行不被打斷
    need_check_system_running_log=false;    
    portEXIT_CRITICAL_ISR(&timerMux1);// 用於ISR的保護機制，確保程式碼運行不被打斷
    //--
  }  
  //---------------------------------------------------------------

  //---------------------------------------------------------------
  // 處理用戶端請求
  server.handleClient();
  delay(2);//稍微delay或許可更穩定
  //---------------------------------------------------------------

  //---------------------------------------------------------------
  // 接收MPPT的UART
  if (Serial1.available() > 0) {
    //--
    // 讀取一個字元
    Serial1_RxChar = Serial1.read();
    //Serial.print(Serial1_RxChar);
    // 存到暫存字元陣列
    temp_str[temp_str_index]=Serial1_RxChar;
    temp_str_index++;
    // 避免超出buffer
    if (temp_str_index>=500)
    {
      temp_str_index=0;
    }
    //--
    // 每次遇到換行就解析
    if (Serial1_RxChar=='\n')
    {
      // 找到「V」
      if (temp_str[0]=='V' && temp_str[1]=='\t')
      {
        //Serial.println("V[mV]: ");
        memset(MPPT_V_str, 0, sizeof(MPPT_V_str));
        for(int i = 2; i < temp_str_index-2; i++)
        {
          MPPT_V_str[i-2]=temp_str[i];
        }
        //Serial.println(MPPT_V_str);
        //
        temp_MPPT_update_index++;
      }
      // 找到「I」
      if (temp_str[0]=='I' && temp_str[1]=='\t')
      {
        //Serial.println("I[mA]: ");
        memset(MPPT_I_str, 0, sizeof(MPPT_I_str));
        for(int i = 2; i < temp_str_index-2; i++)
        {
          MPPT_I_str[i-2]=temp_str[i];
        }
        //Serial.println(MPPT_I_str);
      }
      // 找到「IL」
      if (temp_str[0]=='I' && temp_str[1]=='L' && temp_str[2]=='\t')
      {
        //Serial.println("IL[mA]: ");
        memset(MPPT_IL_str, 0, sizeof(MPPT_IL_str));
        for(int i = 3; i < temp_str_index-2; i++)
        {
          MPPT_IL_str[i-3]=temp_str[i];
        }
        //Serial.println(MPPT_IL_str);
      }
      // 找到「VPV」
      if (temp_str[0]=='V' && temp_str[1]=='P' && temp_str[2]=='V' && temp_str[3]=='\t')
      {
        //Serial.println("VPV[mV]: ");
        memset(MPPT_VPV_str, 0, sizeof(MPPT_VPV_str));
        for(int i = 4; i < temp_str_index-2; i++)
        {
          MPPT_VPV_str[i-4]=temp_str[i];
        }
        //Serial.println(MPPT_VPV_str);
      }
      // 找到「PPV」
      if (temp_str[0]=='P' && temp_str[1]=='P' && temp_str[2]=='V' && temp_str[3]=='\t')
      {
        //Serial.println("PPV[W]: ");
        memset(MPPT_PPV_str, 0, sizeof(MPPT_PPV_str));
        for(int i = 4; i < temp_str_index-2; i++)
        {
          MPPT_PPV_str[i-4]=temp_str[i];
        }
        //Serial.println(MPPT_PPV_str);
      }
      // 找到「Checksum」
      if (temp_str[0]=='C' && temp_str[1]=='h' && temp_str[2]=='e' && temp_str[3]=='c')
      {      
        sprintf(temp_cmd_str, "接收更新: #%d", temp_MPPT_update_index);   
        Serial.println(temp_cmd_str);      
        sprintf(temp_cmd_str, "鋰鐵電池:%s[mV] %s[mA]", MPPT_V_str,MPPT_I_str);   
        Serial.println(temp_cmd_str);
        sprintf(temp_cmd_str, "太陽能板:%s[mV] %s[W]", MPPT_VPV_str,MPPT_PPV_str);   
        Serial.println(temp_cmd_str);
        sprintf(temp_cmd_str, "負載:%s[mA]", MPPT_IL_str);   
        Serial.println(temp_cmd_str);
        Serial.println("--");
        //--
        sprintf(temp_cmd_str,"tV.txt=\"%s[mV]\"\xff\xff\xff",MPPT_V_str);
        Serial2.write(temp_cmd_str);
        sprintf(temp_cmd_str,"tI.txt=\"%s[mA]\"\xff\xff\xff",MPPT_I_str);
        Serial2.write(temp_cmd_str);
        sprintf(temp_cmd_str,"tVPV.txt=\"%s[mV]\"\xff\xff\xff",MPPT_VPV_str);
        Serial2.write(temp_cmd_str);
        sprintf(temp_cmd_str,"tPPV.txt=\"%s[W]\"\xff\xff\xff",MPPT_PPV_str);
        Serial2.write(temp_cmd_str);
        sprintf(temp_cmd_str,"tIL.txt=\"%s[mA]\"\xff\xff\xff",MPPT_IL_str);
        Serial2.write(temp_cmd_str);        
      }
      // 清空
      temp_str_index=0;
      memset(temp_str, 0, sizeof(temp_str));
    }    
  }
  //---------------------------------------------------------------
}
