#include "mbed.h"

DigitalOut led1(LED1);

#if defined(TARGET_LPC1114)
I2C i2c(dp5, dp27);
#else
I2C i2c(D14, D15);
#endif

#define WII_CON (0xA4)

// main() runs in its own thread in the OS
int main() {
    char buf[20];
    buf[0] = 0;
    int ret;
    printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
    for (int adrs = 0; adrs < 0x80; adrs += 16) {
        printf("%02x: ", adrs);
        for (int i = 0; i < 16; i++) {
            i2c.write(((adrs + i) << 1), buf, 1);
            ret = i2c.read(((adrs + i) << 1), buf, 2);
            if ((adrs+i) == 0 || (adrs+i) == 1 || (adrs+i) == 2) {
                printf("   ");
                continue;
            }
            if ((adrs+i) >= 0x78) {
                printf("\n");
                break;
            }
            if (ret == 1) {
                printf("-- ");
            }
            if (ret == 0) {
                printf("%02x ", (adrs + i) << 1);
            }
        }
        printf("\n");
    }
    
    buf[0] = 0xA4;
    //i2c.write(WII_CON, buf, 1);

    // Disable Encryption
    buf[0] = 0xF0;
    buf[1] = 0x55;
    i2c.write(WII_CON, buf, 2);

    buf[0] = 0xFA;
    i2c.write(WII_CON, buf, 1);
    ret = i2c.read((WII_CON|1), buf, 6);
    if (ret == 0) {
       printf("Ack = %02x, %02x, %02x, %02x, %02x, %02x", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
    }
 
    while (true) {
        led1 = !led1;
        thread_sleep_for(0.5);
    }
}

