                 
//定义变量程序段
//把小车左轮电机编码器码盘的OUTA信号连接到Arduino控制器的数字端口2，
//数字端口2是Arduino的外部中断0的端口。
#define PinA_left 2 //外部中断0
#define PinA_right 3 //外部中断1
#include <Servo.h>
Servo camservo;
int power =93;
int E_left =5; //L298n直流电机驱动板的左轮电机使能端口连接到数字接口5
int M_left =4; //L298n直流电机驱动板的左轮电机转向端口连接到数字接口4
int E_right =6; //连接小车右轮电机的使能端口到数字接口6
int M_right =7; //连接小车右轮电机的转向端口到数字接口7
int IN4 = 8 ;
int IN3 = 9;
int IN2 = 10;
int av4,av3,av2;
int randNumber;

//int val_right; //小车右轮电机的PWM功率值
//int val_left;
int count1 = 0;  //左轮编码器码盘脉冲计数值
int count2 = 0; //右轮编码器码盘脉冲计数值
int rpm1 = 0;  //左轮电机每分钟(min)转速(r/min)
int rpm2 = 0;  //右轮电机每分钟(min)转速(r/min)
int Lspeed;
int Rspeed;

unsigned long time = 0, old_time = 0; // 时间标记
unsigned long time1 = 0, time2 = 0; // 时间标记

//初始化程序段
void setup()
{
  Serial.begin(9600);  // 启动串口通信，波特率为9600b/s
  camservo.attach(11);
  pinMode(M_left, OUTPUT);   //L298N直流电机驱动板的控制端口设置为输出模式
  pinMode(E_left, OUTPUT); 
  pinMode(M_right, OUTPUT); 
  pinMode(E_right, OUTPUT);
  pinMode(PinA_left, INPUT); //编码器的信号端设置为输入模式
  pinMode(PinA_right, INPUT);
  pinMode(IN4, INPUT);
  pinMode(IN3, INPUT);
  pinMode(IN2, INPUT);
  //定义外部中断0和1的中断子程序Code(),中断触发为下跳沿触发
  //当编码器码盘的OUTA脉冲信号发生下跳沿中断时，
  //将自动调用执行中断子程序Code()。
  attachInterrupt(0, Code1, FALLING);//左车轮电机的编码器脉冲中断函数
  attachInterrupt(1, Code2, FALLING);//右车轮电机的编码器脉冲中断函数
  Serial.println("start");
}

void advance()//小车前进
{
    digitalWrite(M_left, LOW);
     digitalWrite(E_left, HIGH);
     //analogWrite(E_left,val_left);
     digitalWrite(M_right, LOW);
    // analogWrite(E_right,val_right);
    digitalWrite(E_right, HIGH);
}
void back()//小车后退
{
     digitalWrite(M_left, HIGH);
     digitalWrite(E_left, LOW);
     //analogWrite(E_left,val_left);
     digitalWrite(M_right, HIGH);
    // analogWrite(E_right,val_right);
    digitalWrite(E_right, LOW);
}

void left()//小车后退
{
     digitalWrite(M_left, HIGH);
     digitalWrite(E_left, LOW);
     //analogWrite(E_left,val_left);
     digitalWrite(M_right, LOW);
    // analogWrite(E_right,val_right);
    digitalWrite(E_right, HIGH);
}

void right()//小车后退
{
     digitalWrite(M_left, LOW);
     digitalWrite(E_left, HIGH);
     //analogWrite(E_left,val_left);
     digitalWrite(M_right, HIGH);
    // analogWrite(E_right,val_right);
    digitalWrite(E_right, LOW);
}

void Stop()//小车停止
{
     digitalWrite(M_left, HIGH);
     digitalWrite(E_left, HIGH);
     //analogWrite(E_left,val_left);
     digitalWrite(M_right, HIGH);
    // analogWrite(E_right,val_right);
    digitalWrite(E_right, HIGH); 
} 

void camleft()
{
    power=180;
    camservo.write(power);
} 
void camright()
{
    power=60;
    camservo.write(power);
} 

void camstop()
{
    power=0;
    camservo.write(power);
}

void elude()
{
    if(av4==LOW && av3==LOW && av2==LOW)
        {
            advance();
            delay(200);
        }
    else if(av4==HIGH && av3==HIGH && av2==HIGH)
        {
            back();
            delay(300);
            randNumber=random(10);
            {
                if(randNumber<5)
                {
                  right();
                  delay(500);
                }
                else
                {
                   left();
                   delay(500);
                }
            }   
        }
    else if(av4==LOW && av3==LOW && av2==HIGH)
        {
            left();
            delay(300);
        }
     else if(av4==HIGH && av3==LOW && av2==LOW)
        {
            right();
            delay(300);
        }
     else if(av4==LOW && av3==HIGH && av2==LOW)
        {
            back();
            delay(500);
          randNumber=random(10);
            {
                if(randNumber<5)
                {
                  right();
                  delay(500);
                }
                else
                {
                   left();
                   delay(500);
                }
            }
        }  
}


  
void loop()
{
  count1=digitalRead(PinA_right);
  count2=digitalRead(PinA_left);
  av4=digitalRead(IN4);
  av3=digitalRead(IN3);
  av2=digitalRead(IN2);
  while(Serial.available())
  {
    char g=Serial.read();
    //char b=Serial.read();
    //char l=Serial.read();
    //char r=Serial.read();
    //char s=Serial.read();
    //char e=Serial.read();
      if (g == 'a')
      {
          advance();
          //delay(500);
          //Stop();
          //delay(100);
      }
      else if (g == 'b')
      {
          back();
          //delay(500);
          //Stop();
          //delay(100);
      }
      else if (g == 'l')
      { 
          left();
          //delay(500);
          //Stop();
          //delay(100);
      }
      else if (g == 'r')
      {
          right();
          //delay(500);
          //Stop();
          //delay(100);
      }
      else if (g == 'e')
      {
          elude();
      }
      else if (g =='s')
      {
          Stop();
      }
  }
         
   time = millis();//以毫秒为单位，计算当前时间
  //计算出每一秒钟编码器码盘计得的脉冲数，
  if(abs(time - old_time) >= 1000) // 如果计时时间已达1秒
  {
    
    detachInterrupt(0); // 关闭外部中断0
    detachInterrupt(1); // 关闭外部中断1    
    //把每一秒钟编码器码盘计得的脉冲数，换算为当前转速值
    //转速单位是每分钟多少转，即r/min。这个编码器码盘为12个齿。
    rpm1 = (float)count1*60;//小车左车轮电机转速
    rpm2 = (float)count2*60; //小车右车轮电机转速
    Lspeed =(float)rpm1*31/100;
    Rspeed = (float)rpm2*31/100;
      
    
    //恢复到编码器测速的初始状态
    count1 = 0;   //把脉冲计数值清零，以便计算下一秒的脉冲计数
    count2 = 0; 
    old_time =  millis();     // 记录每秒测速时的时间节点   
    attachInterrupt(0, Code1,FALLING); // 重新开放外部中断0
    attachInterrupt(1, Code2,FALLING); // 重新开放外部中断1
    
    Serial.print("L_r/min:");              
    Serial.println(rpm1);                  
    Serial.print("R_r/min:");
    Serial.println(rpm2);
    Serial.print("LSPEED  m/min:");
    Serial.println(Lspeed);
    Serial.print("RSPEED  m/min:");
    Serial.println(Rspeed);   
   
  }
}

// 左侧车轮电机的编码器码盘计数中断子程序
void Code1()
{  
  //为了不计入噪音干扰脉冲，
  //当2次中断之间的时间大于5ms时，计一次有效计数
  if((millis()-time1)>5) 
  //当编码器码盘的OUTA脉冲信号下跳沿每中断一次，
  count1 += 1; // 编码器码盘计数加一  
  time1==millis();
}
// 右侧车轮电机的编码器码盘计数中断子程序
void Code2()
{  
  if((millis()-time2)>5) 
  //当编码器码盘的OUTA脉冲信号下跳沿每中断一次，
  count2 += 1; // 编码器码盘计数加一
  time2==millis();  
}
