#include "mbed.h"
#include "LM75B.h"
#include "BME280.h"


/* Digital Out Pin Configuration */
DigitalOut RED(p23,1);
DigitalOut GREEN(p24,1);
DigitalOut BLUE(p25,1);

AnalogIn LDR(p16);// 1 para iluminado e 0.1 para escuro

AnalogIn moisture(p15); // 0.34 para seco , 0.25 para molhado

AnalogIn fogo(p17); // 1 para fogo e 0.6 pra suave

DigitalOut motor(p21);// 1 para ligado , 0 para desligado

DigitalIn presente(p5);// 1 tem alguem , 0 nao tem ninguem

/* UART Pin Configuration */
Serial pc(USBTX, USBRX);
Serial bt(p13,p14);



#if defined(TARGET_LPC1768)
BME280 thp(p28, p27); // scl no 27,
#else
BME280 thp(I2C_SDA, I2C_SCL);
#endif


int main(void)
{
    /* baudrate configuration */
    pc.baud(115200);
    bt.baud(9600);


    pc.printf("WIZwiki-W7500 BT\n\r");

    char ch;
    char msg[256];


    float vfogo = 0.0f;
    float value = 0.0f;


    while(1) {

        /* Detectar fumaça */
        value = moisture;
        vfogo = fogo;
        if (vfogo >= 0.6) {
            bt.printf ("TA PEGANDO FOGO BIXOOO\n");
        }
        /* Acionar motor caso o solo esteja seco */
        if (value >= 0.8) {
            motor = 1;
            wait(0.8);
            motor =0;
        }
        /* Sensor de presença */
        if (presente) {
            bt.printf("\n OPA!!!! \n Tem Alguem Aqui");
        }
        wait(1.0);


        /* WIZwiki-W7500 para Bluetooth */
        if(pc.readable()) {
            pc.scanf("%s",&msg);
            bt.printf("%s",msg);
        }

        /* Bluetooth para WIZwiki-W7500 */
        if(bt.readable()) {
            ch = bt.getc();
            pc.putc(ch);


            if(ch == 'r') {
                /* Mostra todas as Informações da estufa no celular */
                bt.printf ("\n\n\n\n\n\n\n\n\n\n\n\n\n");
                bt.printf ("MWMWMWMWMWMWMWMWMWMW\n");
                bt.printf ("Luminosidade = %f\n", LDR.read());
                bt.printf ("Humidade do Solo = %f\n", (float)value);
                bt.printf ("Pressao = %04.2f hPa\n", thp.getPressure());
                bt.printf ("Temperatura = %2.2f °C\n", thp.getTemperature());
                bt.printf ("MWMWMWMWMWMWMWMWMWMW\n");
                bt.printf ("\n");
            
            /* Acionar motor pelo botão no celular */
            } else if(ch == 'g') {
                motor = 1;
                wait(0.8);
                motor =0;
            }
        }

    }
}

