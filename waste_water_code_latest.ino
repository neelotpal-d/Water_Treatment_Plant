int Liquid_level=0;
int port[8];
unsigned long t=0,t1=0,t2=0,t3=0,t4=0,t5=0,t6=0,t7=0;
boolean relay_state[8];
//int state[3];
int stateq[5]; 
int sen[5];
int mark=0;

void setup() {
  pinMode(A0,OUTPUT);
  digitalWrite(A0,HIGH);
  pinMode(A1,OUTPUT);
  digitalWrite(A1,HIGH);
pinMode(4,INPUT); //sen1
pinMode(5,INPUT);
pinMode(6,INPUT);
pinMode(7,INPUT);
pinMode(8,INPUT);
pinMode(9,OUTPUT);
pinMode(50,OUTPUT);//peri
pinMode(10,OUTPUT);
pinMode(11,OUTPUT);
pinMode(13,OUTPUT);
pinMode(2,OUTPUT);
pinMode(12,OUTPUT);
//pinMode(14,OUTPUT);

 Serial.begin(9600);
 digitalWrite(10,HIGH);
 digitalWrite(9,HIGH);
  digitalWrite(11,HIGH);
  digitalWrite(13,HIGH);
  digitalWrite(12,HIGH);
  digitalWrite(50,HIGH);
  digitalWrite(2,HIGH);
   sen[0]=4;
  sen[1]=5;
  sen[2]=6;
  sen[3]=7;
  sen[4]=8;
  
port[0]=10;
port[1]=9;
port[2]=11;
port[3]=12;
port[4]=2;
port[5]=53;
port[6]=50;
port[7]=13;
relay_state[0]=1;
relay_state[1]=1;
relay_state[2]=1;
relay_state[3]=1;
relay_state[4]=1;
relay_state[5]=1;
relay_state[6]=1;
relay_state[7]=1;
stateq[1-1]=0;
stateq[2-1]=0;
stateq[3-1]=0;
stateq[4-1]=0;
stateq[5-1]=0;
}

void loop() {
  t=millis();
  
  if(stateq[1-1]==0 && sensor(1)==1 && stateq[3-1]<2)
  { initial1_1();
  Serial.println("release valve 1");}//release valve 1

  
 else if(relay_state[1-1]==0 && sensor(2)==1 && stateq[1-1]==1)
  {initial1_2();  //switch off the valve 1 and on the peristatic pump
  Serial.println("switch off the valve 1 and on the peristatic pump");}

  
 else if((t-t1)>=23 && relay_state[2-1]==0 && t1>0)
  {    initial2_1(); //switch of the peristatic pump
    //delay(4000);
   Serial.println("switch of the peristatic pump");}

 else   if((t-t2)>=1800000 && stateq[2-1]==1 && t2>t1 )
  {initial3_1(); //release  supernetant valve
  Serial.println("release  supernetant valve");}

  
 else if(relay_state[3-1]==0 && sensor(3)==0)
  {initial3_2();
  Serial.println("stop supenetant valve");} //stop supenetant valve

 else if((t-t3)>10000 && t3>t2  && relay_state[4-1]==1 && stateq[2-1]==3)
  {
    initial4_1();
    Serial.println("sludge valve on ");
  }

else  if((t-t4)>15000 && t4>t3 && relay_state[4-1]==0 )
 {
    initial4_2();
    Serial.println("stop sludge valve and on blower");
  }

else if(stateq[2-1]==4 && (t-t5)>10000  && t5>t4)
  {initial5_1();
  Serial.println("blank on");}

  else if(stateq[2]==2 && (t-t5)>36000000 && t5>t4)
  {initial6_1();
  stateq[2]=3;
  Serial.println("Pump ON");}

  else if((relay_state[6]==0) && (sensor(4)==0))
  {initial6_2();
  Serial.println("Pump OFF");}

  else if(stateq[4-1]==1 && (t-t6)>=900000 && stateq[5-1]==0 && t6>t5)
  {initial7_1(); } //open valve 5


 else if(relay_state[8-1]==0 && sensor(5)==0)
 { initial7_2(); 
 delay(50);
 last_step1();}


//else if(stateq[5-1]==1 && (t-t6)>= 300000 && t6>t5)
  //{last_step1();}

else if(stateq[2]==2 && (t-t5)>40000 && t5>t4){
  Serial.println(stateq[3-1]);}

  delay(100);

}

void initial1_1(){
  //Serial.println("INR");
  relay_change(1);
  stateq[1-1]=1;

   
}

void initial1_2(){
  relay_change(1);
  relay_change(2);
  t1=millis();
  stateq[2-1]=1;   
}

void initial2_1(){
  relay_change(2);
  t2=millis();
   
}



void initial3_1(){
  relay_change(3);
  stateq[2-1]=3;
}

void initial3_2(){
  relay_change(3);
  t3=millis();
}



void initial4_1(){
  relay_change(4);
  t4=millis();  
}

void initial4_2(){
  
  relay_state[4-1]=!relay_state[4-1];
  digitalWrite(port[4-1],relay_state[4-1]);
  delay(15000);
  switch_blower_on();
  stateq[2-1]=4;
  t5=millis();
  stateq[3-1]=stateq[3-1]+1;
 }

/*void initial4_3(){
  relay_change(4);
}*/

void initial5_1(){
 stateq[1-1]=0;
 stateq[2-1]=0;
 
 }

void initial6_1(){
// delay(10000);
 //switch_blower_off();
 delay(10000);
relay_change(7);
 //relay_change(6);
// delay(10000);
 //relay_change(7);
 
 }


 void initial6_2(){
 relay_change(7);
 delay(2000);
// relay_change(6);
 t6=millis();
 stateq[4-1]=1;
 
 }




 void initial7_1(){
  relay_change(8);
  stateq[4-1]=2;
}

void initial7_2(){
  relay_change(8);
  stateq[4-1]=0;
  stateq[5-1]=1;
  t7=millis();
}


void last_step1(){
  stateq[3-1]=0;
  stateq[5-1]=0;
 }
 
int switch_blower_on()
{
 relay_state[5-1]=0; 
 digitalWrite(port[5-1],relay_state[5-1]); 
}

int switch_blower_off()
{
 relay_state[5-1]=1; 
 digitalWrite(port[5-1],relay_state[5-1]); 
}

void relay_change(int unitnumber){
  relay_state[unitnumber-1]=!relay_state[unitnumber-1];
  Serial.println(unitnumber);
  boolean state1=relay_state[unitnumber-1];
  digitalWrite(port[unitnumber-1],state1);
}


boolean sensor(int x){
 int z=1;
 z=sen[x-1];
Liquid_level=digitalRead(z);
return (boolean)Liquid_level;
}
