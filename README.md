# Smart parking lot

### Preface:

* This project is ***[ADATA Technology](https://www.adata.com/)*** cooperate with ***[FCU](http://www.fcu.edu.tw/wSite/mp?mp=1)*** to make a ***smart parking lot***

### Purpose:
  
* Applying ***ADATA's LED lights*** to be a smart parking lot indicator

  * We have to drive and control it through wifi

### Architecture:

![Architecture](/img/architecture.jpg)

### Working detail:

* I finished those job ***by myself***:

  * ***Monitor, counting  and indicate state of every parking space***
  
  * ***Communication*** between ***parking lot*** and ***server***

* Due to some reason, we have to using ***[tenx technology](https://www.tenx.com.tw/)*** [TM52F5288](https://www.tenx.com.tw/product_detail.aspx?ProductID=309) WiFi board ***without any SDK***

* For the detail please download the ***[final_report](https://github.com/ihunhh/Smart_parking_lot/raw/master/final_report.docx)***

* It needs ***3*** PWM outputs to drive the ***RGB*** light, due to hardware limited, I used two PWM ouput and, 

  simulated the other PWM output by ***interrupt*** 
  
### HW blockdiagram:

#### Parking place

![PKP](/img/HWblockdiagram/HW_blockdiagram_PKP.png)

#### Pillar server

![Pillar](/img/HWblockdiagram/HW_blockdiagram_PKP.png)
  
 
 
 



  


