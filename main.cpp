#include "mbed.h"
#include<cmath>
#include<iostream>
Serial pc( USBTX, USBRX );
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
DigitalIn  Switch(SW3);
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
DigitalOut redLED(LED2);
DigitalOut greenLED(LED1);
int sample = 500;

char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

int main(){
    int cnt = 0;
    float temp1=0, temp2=0;
    float ADCdata[sample];
    for (int i = 0; i < sample; i++){
        ADCdata[i] = Ain;
        Aout = Ain;
        if(i == 0){
            temp1 = Ain;
            temp2 = Ain;
        }
        temp2 = temp1;
        temp1 = Ain;
        if(temp1 > 0.2 && temp2 <= 0.2) cnt++;
        wait(1./sample);
    }

    for (int i = 0; i < sample; i++){
        pc.printf("%1.3f\r\n", ADCdata[i]);
    }

    int frequency = round(cnt);
    int num;
    if(frequency>=100) num = 3;
    else num = 2;
    char dig[num];
    int divfre = frequency;
    
    for (int j = 0; j<num; j = j+1){
        dig[num-j-1] = table[divfre%10];
        divfre = divfre/10;
    }
    dig[num-1]= dig[num-1] ^ (1<<7);

    int flag = 0;
    while(1){
        if( Switch == 0 ){
            redLED = 1;
            greenLED = 0;
            display = dig[flag];
            flag++;
            flag = flag%num;
            wait(1);
        }
        else{
            redLED = 0;
            greenLED = 1;
            display = 0x00;
        }
        for(float p=0; p<2; p+=(0.05)){
            Aout = 0.5 + 0.5*sin(p*3.14159);
            wait(1./frequency/40);
        }
    }
}