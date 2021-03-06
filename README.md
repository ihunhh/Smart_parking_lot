# Smart parking lot

### Preface:

* This is an ***IoT consultancy*** project between ***[Feng Chia University(Taichung City, Taiwan)](http://www.fcu.edu.tw/wSite/mp?mp=1)*** and ***[ADATA Technology](https://www.adata.com/)*** for ***large-scale*** smart parking lot soulution

### Purpose:
  
* Applying ***ADATA's LED lights*** to be a smart parking lot indicator

  * We have to drive and control it through **wifi**

### Architecture:

![Architecture](/img/architecture.jpg)

### Working detail:

* ***Individual work***:

  * ***Monitoring, counting  and indicating*** the ***state*** of each parking space
  
  * ***Communication*** between ***parking lot*** and ***server***

* Given the subject, this project only use ***[tenx technology](https://www.tenx.com.tw/)*** [TM52F5288](https://www.tenx.com.tw/product_detail.aspx?ProductID=309) WiFi board ***without any SDK***

* For the ***detail*** please download the ***[final_report](https://github.com/ihunhh/Smart_parking_lot/raw/master/final_report.docx)***

* It needs ***3 PWM*** outputs to drive the ***RGB*** light, due to ***hardware limitation*** that ***only 2 PWM outputs*** could be used. Therefore I simulated the other PWM output by ***interrupt*** 

* Pillar sever algorithm is implement on ***Arduino YUN board***

  * ***[Source code](/project/Pillar_server/)***

* Parking place algorithm is implement on ***TM52F5288***

  * ***[Source code](/project/Parking_place/)***
 
  
### Hardware blockdiagram:

* #### Parking place

  <img src="/img/HWblockdiagram/HW_blockdiagram_PKP.png" height="600">

* #### Pillar server

  <img src="/img/HWblockdiagram/HW_diagram_Pillar.png" height="400">

 
 
 



  


