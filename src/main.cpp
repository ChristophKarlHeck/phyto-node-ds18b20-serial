#include "PinNames.h"
#include "mbed.h"
//#include "onewire_class.h"
#include "ds18x20/ds18x20.h"

// main() runs in its own thread in the OS

onewireclass T1(PB_8);
onewireclass T2(PB_9);
onewireclass T3(PA_6);
onewireclass T4(PA_7);
//onewireclass T2(PC_6);

int main()
{

    uint8_t sp[25];
    uint8_t num_sensor = 0;
    uint8_t subzero, cel, cel_frac_bits;
    bool connected = false;
    Timer timer;
    uint8_t gSensorIDs[MAXSENSORS][OW_ROMCODE_SIZE];
    uint8_t nSensors;

    static BufferedSerial pc(USBTX,USBRX);
    uint8_t buffer[11] = {0};
    pc.set_baud(19200);
    pc.set_format(
            /* bits */ 8,
            /* parity */ BufferedSerial::Odd,
            /* stop bit */ 1
    );

    
    
    while (true) {
        //wait_us(1000);
        while (!connected){
            if (T1.search_sensors(&nSensors,&gSensorIDs[0][0])) {
                printf("T1 not found\n");
            } else if(T2.search_sensors(&nSensors,&gSensorIDs[1][0])) {
                   printf("T2 not found\n");
            } else if(T3.search_sensors(&nSensors,&gSensorIDs[2][0])) {
                   printf("T3 not found\n");
            } else if(T4.search_sensors(&nSensors,&gSensorIDs[3][0])) {
                   printf("T4 not found\n");
            } else {
                printf("Sensors found\n");
                connected = true;  
                char text_id[25]; 

                T1.ow_show_id( &gSensorIDs[num_sensor][0], OW_ROMCODE_SIZE,text_id );

                printf("%s Sensor # %d is a ",text_id, num_sensor+1);

                if (( gSensorIDs[num_sensor][0] == DS18S20_ID) || ( gSensorIDs[num_sensor][0] == DS18B20_ID)) {
                    if ( gSensorIDs[num_sensor][0] == DS18S20_ID)
                        printf("DS18S20/DS1820 ");
                    if ( gSensorIDs[num_sensor][0] == DS18B20_ID)
                        printf("DS18B20 ");
                    if ( DS18X20_get_power_status( &gSensorIDs[num_sensor][0], T1 ) ==
                            DS18X20_POWER_PARASITE )
                        printf( "parasite" );
                    else printf( "externally" );
                    printf( " powered\n" );
                }
                fflush(stdout);

                //T2.ow_show_id( &gSensorIDs[1][0], OW_ROMCODE_SIZE,text_id );
            }
        }
        //printf("%d",DS18X20_start_meas(DS18X20_POWER_EXTERN, 0 ,T1));
        //wait_us(1000); 
        if (DS18X20_start_meas(DS18X20_POWER_EXTERN, 0 ,T1) == DS18X20_OK &&
        DS18X20_start_meas(DS18X20_POWER_EXTERN, 0 ,T2) == DS18X20_OK &&
        DS18X20_start_meas(DS18X20_POWER_EXTERN, 0 ,T3) == DS18X20_OK &&
        DS18X20_start_meas(DS18X20_POWER_EXTERN, 0 ,T4) == DS18X20_OK){
            wait_us(DS18B20_TCONV_12BIT*1000);
        } else{
             printf("sensor T1 DS18X20 : Start meas. failed \n");
        }

        //printf("%d",DS18X20_read_meas( &gSensorIDs[0][0], &subzero, &cel, &cel_frac_bits,T1));
        
        
        if(DS18X20_read_meas( &gSensorIDs[0][0], &subzero, &cel, &cel_frac_bits,T1) == DS18X20_OK){
            char text[25];
            DS18X20_show_temp(subzero, cel, cel_frac_bits,text);
            printf("sensor T1 %d : %s\n",num_sensor+1,text);
            buffer[2]= cel;
            buffer[3]= cel_frac_bits;
        }
        if(DS18X20_read_meas( &gSensorIDs[1][0], &subzero, &cel, &cel_frac_bits,T2) == DS18X20_OK){
            char text[25];
            DS18X20_show_temp(subzero, cel, cel_frac_bits,text);
            printf("sensor T2 %d : %s\n",num_sensor+1,text);
            buffer[4]= cel;
            buffer[5]= cel_frac_bits;
        }
        if(DS18X20_read_meas( &gSensorIDs[2][0], &subzero, &cel, &cel_frac_bits,T3) == DS18X20_OK){
            char text[25];
            DS18X20_show_temp(subzero, cel, cel_frac_bits,text);
            printf("sensor T3 %d : %s\n",num_sensor+1,text);
            buffer[6]= cel;
            buffer[7]= cel_frac_bits;
        }
        if(DS18X20_read_meas( &gSensorIDs[3][0], &subzero, &cel, &cel_frac_bits,T4) == DS18X20_OK){
            char text[25];
            DS18X20_show_temp(subzero, cel, cel_frac_bits,text);
            printf("sensor T4 %d : %s\n",num_sensor+1,text);
            buffer[8]= cel;
            buffer[9]= cel_frac_bits;
        }

        buffer[0] = 80;  // need start values
        buffer[1] = 0x06; // Node name

        buffer[10] = 0x0A;

        pc.write(buffer, sizeof(buffer));

        /*
        switch (DS18X20_read_meas( &gSensorIDs[0][0], &subzero, &cel, &cel_frac_bits,T1)) {
            case DS18X20_OK :
                char text[25];
                DS18X20_show_temp(subzero, cel, cel_frac_bits,text);
                printf("sensor %d : %s\n",num_sensor+1,text);
                break;
            case OW_ERROR:
                printf("sensor %d : BUS Error\n",num_sensor+1);
                break;
            case DS18X20_ERROR_CRC:
    
                printf("sensor %d : CRC Error\n",num_sensor+1);
                break;
        }
        */
        //wait_us(1000);
        

        


    //wait_us(1000000);
    }

}