# SolarPowerSystem

### 太陽能充電控制器
+ 充電控制器官網: https://www.victronenergy.com/
+ 台灣代理商官網: https://www.formay.com.tw/
  + 產品頁(SmartSolar MPPT 75/15): https://www.formay.com.tw/outdoor/victron-energy/90-battery/351-82vescsma15
  + 價格: NTD4600 (2023/12詢價)
+ 產品說明:
  + 透過藍芽可設定，監視，更新和同步SmartSolar充電控制器。
  + 超快速MPPT(最大功率點跟蹤)，當天空有雲，陽光強度不停的變化，超快速MPPT比PWM可增加30%能源,比較慢的MPPT也可增加10%。
  + 預防電瓶過度放電可將所有負載接到負載輸出端。當電瓶到達設定的最低放電電壓，負載輸出會中斷負載。
  + 如果太陽能充電器無法在一天內將電瓶充飽，這會常常造成電瓶在部份充電狀態與結束放電狀態間連續循環，這樣的模式(無常態充飽)會造成鉛酸電瓶在幾周或幾月後就會損壞。智能電瓶管理會監視電瓶的充電狀態，如果需要會每天小幅度增加中斷負載的值(早點中斷負載)，直到獲取足夠的太陽能來把電瓶充到幾乎100%時，中斷負載的值會被調整成大約每周都會充到大約100%。
  + 可透過APP輕鬆更改電瓶充電演算法。
  + 可用軟體設定白天/夜晚時機和燈光調光訊號的選項。
  + 內建溫度感應器，依溫度做注滿和浮動充電的補償。
  + 可與Smart Battery Sense和電瓶監視器溝通取得電瓶電壓及溫度的資料。
  + 因為有高溫保護機制，所以當環境溫度太高時，輸出的電流會減少。
+ 產品規格: 
  + 型號：SmartSolar MPPT 75/15(SCC075015060R)
  + 電瓶電壓(自動選擇)：12/24V
  + 充電電流：15A
  + 額定太陽能板電源：220W(12V) 440W(24V)
  + 最大太陽能板短路電流：15A
  + 自動負載中斷：有
  + 最大太陽能板開路電壓：75V
  + 尖峰效率：98%
  + 自消耗-負載開：12V:19mA 24V:16mA
  + 自消耗-負載關：12V:10mA 24V:8mA
  + 注滿充電電壓：14.4V / 28.8V (可調)
  + 浮動充電電壓：13.8V / 27.6V (可調)
  + 充電演算法：多階段自適應
  + 溫度補償：-16mV / °C / -32mV / °C
  + 持續負載電流：15A
  + 低電壓負載中斷：11.1V / 22.2V或 11.8V / 23.6V 或 電瓶壽命演算法
  + 低電壓負載重連接：13.1V / 26.2V或 14V / 28V 或 電瓶壽命演算法
  + 保護：輸出短路 溫度過高
  + 工作溫度：-30至+60°C (全功率輸出最高至40°C)
  + 濕度：95%, 不凝結
  + 資料通訊：VE.Direct或藍芽(參照我們網站的資料通訊白皮書)
  + 外觀/尺寸
  + 顏色：藍(RAL 5012)
  + 電源接線：6mm² / AWG10
  + 防護等級：IP43 (電子元件)，IP22 (接線部)
  + 重量：0.5kg
  + 尺寸(h x w x d)：100 x 113 x 40mm
  + 標準
    + 安全：EN/IEC 62109-1, UL 1741, CSA C22.2
    + IEC 62109-1安全證書: https://www.victronenergy.com/upload/documents/Certificate-Safety-EN-IEC-62109-1-BlueSolar-&-SmartSolar-MPPT-75-10-&-75-15.pdf
    + Automotive ECE-R10-5安全證書: https://www.victronenergy.com/upload/documents/Certificate-Automotive-ECE-R10-5-BlueSolar-100-15,-100-20-&-SmartSolar-MPPT-75-10,-75-15,-100-15,-100-20.pdf
    + 已取得NCC(國家通訊傳播委員會)認證
  + 中文說明書: https://www.formay.com.tw/outdoor/manual/victron/smartsolar_charge_controller_75_manual.pdf
  + 英文規格書: https://www.victronenergy.com/upload/documents/Datasheet-SmartSolar-charge-controller-MPPT-75-10,-75-15,-100-15,-100-20_48V-EN.pdf
  + 英文系統介紹: https://www.victronenergy.com/upload/documents/Energy_Storage_System/6292-ESS_design_and_installation_manual-pdf-en.pdf
  + 英文使用說明書: https://www.victronenergy.com/upload/documents/Manual_SmartSolar_MPPT_75-10_up_to_100-20/29694-MPPT_solar_charger_manual-pdf-en.pdf
  + 英文通訊說明書: https://www.victronenergy.com/upload/documents/VictronConnect_Manual/8778-VictronConnect_manual-pdf-en.pdf
  + 英文通訊協定: https://www.victronenergy.com/upload/documents/VE.Direct-Protocol-3.33.pdf

+ 目前設定(2023-12-18更新):
  + 以下使用中文語系進行設定。
  + 注意: 沒有儲存參數的按鈕，從APP修改參數後立刻變更軟體參數內容，硬體延遲一陣子之後才會生效。
  + 設置>電池:
    + 電池類型=12V
    + 電池預設=用戶定義 (後續詳細參數變更後就會自動改為用戶定義)
    + 均充電壓=13.80V (鋰鐵電池官方建議最大不可超過14V)
    + 浮充電壓=13.60V (此項要低於均充電壓)
  + 設置>負載:
    + 操作模式=用戶定義算法1: 高壓水平負載開關=13.4V(不可以超過浮充電壓)，低壓水平負載開關=11.8V(參考電池規格)。意義:低於11.8V就不供電讓外部負載使用，純充電(萬一不幸真的沒電可充，會持續耗電永遠充不飽)。充到13.4V以上才開始繼續供電給負載。
      + 註1: 原廠預設值是:操作模式=電池壽命(某電壓以上)
      + 註2: 目前測試MT主機加上磁棒以LOW模式紀錄功耗約5-7W，4G路由器功耗約1-2W，充電控制器本身0.2W。同時接上作為負載，從耗電到沒電約可使用7天。
  + 安裝:
    + 請參照說明書建議順序
    + 太陽能板預計並聯兩片。不建議超過兩片。需要兩條太陽能板並聯線(MC4)

### 太陽能充電控制器自製擴充
+ 開發版: NODEMCU-32 V1.3 CH340
+ 開發環境: Arduino IDE 2.2.1

### 充電電池
+ 電池官網: https://www.grenergy.com.tw/
+ 訂做規格: 鋰鐵電池(GreenRun2)100AH/~10KG ，價格: 15000未稅，保固: 半年。
+ 

### 便攜式太陽能板
+ 產品名稱: 70mai Solar Panel
+ 台灣地區代理商: 天傑科技有限公司(https://www.70mai.com.tw/)
+ 官方網站: https://powerstation.70mai.com/
+ 官方網站繁體中文: https://zh-tw.powerstation.70mai.com/
+ 母公司: 上海七十迈数字科技有限公司(https://www.70mai.com.cn/)，英文品牌名稱:70mai，為小米生態鏈。
+ 產品型號: ME11MA1
  + 價格: 原價NTD19999，特價NTD10500，2023-12-18查價。
  + 規格:
    + 峰值功率(額定功率): 110W
    + 電源電壓： 12V
    + 電源電流： 3A
    + 開路電壓： 22.5V
    + 短路電流：5.8A
    + 能源轉換效率: 22%
    + 電池類型: 單矽晶
    + 太陽能電池層壓: ETFE
    + 太陽能接頭: MC4
    + 重量： 11.0 磅/5.0 公斤
    +  尺寸（折疊）：28.2x23.2x1.2 英吋 / 71.6x59.0x3 公分
    +  尺寸（展開）： 28.2x46.8x0.6 英吋 / 71.6x119.0x1.5 公分
    +  工作使用溫度： -10-70℃
    +  保固：12 個月
+ 使用方式: 兩片並聯
