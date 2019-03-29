#include "hwlib.hpp"
#include "MAX7221.hpp"



int main(void){
	// kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(500);
    
    auto Chip_Select = hwlib::target::pin_out(hwlib::target::pins::d7);
    auto Clock = hwlib::target::pin_out(hwlib::target::pins::d6);
    auto Data_Output = hwlib::target::pin_out(hwlib::target::pins::d8);
    auto SPI_Bus = hwlib::spi_bus_bit_banged_sclk_mosi_miso(Clock, Data_Output, hwlib::pin_in_dummy);
    int Matrixes = 4;
    auto ButtonRed = hwlib::target::pin_in(hwlib::target::pins::d9);
    auto ButtonYellow = hwlib::target::pin_in(hwlib::target::pins::d11);
    
    MAX7221 MAX(Chip_Select, SPI_Bus, Matrixes);
    
    /*
     * Pins are as follows:
     * Pin d6 : CS (SS, Slave Select)
     * Pin d7 : Clock (SCLK)
     * Pin d8 : Dout (MOSI)
     * Pin D9 : Button pin_in
     * Pin D11 : Button pin_in
     */
    
    char Text[] = "Wacked ";
    uint8_t Storage[7][8];
    int Size = 7;
     
    MAX.Clear();
    hwlib::wait_ms(100);
    MAX.Fill();
    hwlib::wait_ms(500);
    MAX.Clear();
    MAX.Setup();

    MAX.Translate(Text, Storage);

    MAX.WriteAll(Storage);
    for(;;){
        
        if(!ButtonRed.get()){
            
            MAX.ShiftLeftCircular(Storage, Size);
            MAX.Clear();
            MAX.Setup();
            MAX.WriteAll(Storage);
            
            while(!ButtonRed.get()){
                
            }
        }
        
        if(!ButtonYellow.get()){
            
            
            MAX.ShiftRightCircular(Storage, Size);
            MAX.Clear();
            MAX.Setup();
            MAX.WriteAll(Storage);
            
            while(!ButtonYellow.get()){
                
            }
        }
        
        hwlib::wait_ms(20);
    }

}
