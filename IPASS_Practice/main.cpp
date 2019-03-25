#include "hwlib.hpp"
#include "MAX7221.hpp"

void Shift(const uint16_t Shiftable) {
    // This function is for testing how shifting works.
    
    // LSB | If MSB is first, use this.
    // 0x0f = 11110000
    int mask = 1;
    for( int i = 0; i < 16; i++ ) {
        //hwlib::cout << ((Shiftable >> i) & mask);
    }
    
    hwlib::cout << '\n';
    
    // MSB | If LSB is first, use this.
    // 0x0f = 00001111
    for( int i = 15; i >= 0; i-- ) {
        hwlib::cout << ((Shiftable >> i) & mask);
    }
}

void ShiftTemp(const uint8_t Shiftable) {
    // This function is for testing how shifting works.
    
    // LSB | If MSB is first, use this.
    // 0x0f = 11110000
    int mask = 1;
    for( int i = 0; i < 8; i++ ) {
        //hwlib::cout << ((Shiftable >> i) & mask);
    }
    
    hwlib::cout << '\n';
    
    // MSB | If LSB is first, use this.
    // 0x0f = 00001111
    for( int i = 7; i >= 0; i-- ) {
        hwlib::cout << ((Shiftable >> i) & mask);
    }
}

//struct Coordinates {
//    uint8_t x, y;
//};


int main(void){
	// kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(500);
    
    auto Chip_Select = hwlib::target::pin_out(hwlib::target::pins::d7);
    auto Clock = hwlib::target::pin_out(hwlib::target::pins::d6);
    auto Data_Output = hwlib::target::pin_out(hwlib::target::pins::d8);
    auto SPI_Bus = hwlib::spi_bus_bit_banged_sclk_mosi_miso(Clock, Data_Output, hwlib::pin_in_dummy);
    int Matrixes = 4;
    MAX7221 MAX(Chip_Select, SPI_Bus, Matrixes);
    
    /*
     * Pins are as follows : Chip pin name / SPI pin name
     * Pin d6 : CS / SS (Slave Select)
     * Pin d7 : Clock / SCLK
     * Pin d8 : Dout / MOSI
     * 
     */
     
     
    // Coordinates are through {x,y};
//    Coordinates a = {25,2};
//    Coordinates b = {26,2};
//    Coordinates c = {27,2};
//    Coordinates d = {28,2};
//    Coordinates e = {28,3};
//    uint32_t lhs = 0x0f0f0f0f;
//    uint32_t rhs = 0xf0f0f0f0;
//    uint32_t Map[8] = {lhs, rhs, lhs, rhs, lhs, rhs, lhs, rhs};
//    uint32_t Map2[8] = {rhs, lhs, rhs, lhs, rhs, lhs, rhs, lhs};

// From A-N was made at home, still check if they are correct. The old ones are above.

//    uint8_t A[8] = {102, 102, 126, 126, 102, 102, 126, 60};
//    uint8_t B[8] = {62, 102, 102, 62, 62, 102, 102, 62};
//    uint8_t C[8] = {124, 126, 6, 6, 6, 6, 126, 124};
//    uint8_t D[8] = {62, 126, 102, 102, 102, 102, 126, 62};
//    uint8_t E[8] = {126, 126, 6, 126, 126, 6, 126, 126};
//    uint8_t F[8] = {6, 6, 6, 30, 30, 6, 126, 126};
//    uint8_t G[8] = {60, 126, 102, 118, 6, 6, 124, 120};
//    uint8_t H[8] = {102, 102, 102, 126, 126, 102, 102, 102};
//    uint8_t I[8] = {126, 126, 24, 24, 24, 24, 126, 126};
//    uint8_t J[8] = {12, 26, 26, 24, 24, 24, 126, 126};
//    uint8_t K[8] = {102, 118, 62, 30, 30, 62, 118, 102};
//    uint8_t L[8] = {126, 126, 6, 6, 6, 6, 6, 6};
//    uint8_t M[8] = {66, 66, 66, 90, 90, 126, 102, 66};
//    uint8_t N[8] = {102, 102, 118, 126, 126, 126, 110, 102};
//    uint8_t O[8] = {60, 126, 102, 102, 102, 102, 126, 60};
//    uint8_t P[8] = {6, 6, 62, 126, 102, 102, 126, 62};
//    uint8_t Q[8] = {92, 62, 50, 42, 34, 34, 62, 28};
//    uint8_t R[8] = {102, 54, 30, 30, 54, 102, 126, 126};
//    uint8_t S[8] = {126, 126, 96, 126, 126, 6, 126, 126};
//    uint8_t T[8] = {24, 24, 24, 24, 24, 24, 126, 126};
//    uint8_t U[8] = {60, 126, 102, 102, 102, 102, 102, 102};
//    uint8_t V[8] = {24, 60, 126, 126, 102, 102, 66, 66};
//    uint8_t W[8] = {126, 90, 90, 90, 90, 90, 90, 90};
//    uint8_t X[8] = {102, 126, 126, 60, 24, 60, 126, 102};
//    uint8_t Y[8] = {24, 24, 24, 24, 60, 126, 102, 66};
//    uint8_t Z[8] = {126, 126, 12, 24, 48, 96, 126, 126};

//    uint8_t 0[8] = {126, 126, 102, 110, 118, 102, 126, 126};
//    uint8_t 1[8] = {126, 126, 24, 24, 24, 30, 28, 24};
//    uint8_t 2[8] = {126, 126, 12, 24, 48, 98, 126, 60};
//    uint8_t 3[8] = {60, 126, 98, 120, 120, 98, 126, 60};
//    uint8_t 4[8] = {96, 96, 96, 126, 126, 102, 102, 102};
//    uint8_t 5[8] = {126, 126, 112, 126, 126, 6, 126, 126};
//    uint8_t 6[8] = {126, 126, 102, 126, 126, 6, 126, 126};
//    uint8_t 7[8] = {96, 96, 96, 96, 96, 96, 126, 126};
//    uint8_t 8[8] = {126, 126, 102, 126, 126, 102, 126, 126};
//    uint8_t 9[8] = {126, 126, 96, 126, 126, 102, 126, 126};
    
    uint8_t A[Matrixes][8] = {{102, 102, 126, 126, 102, 102, 126, 60}, {62, 102, 102, 62, 62, 102, 102, 62}, {124, 126, 6, 6, 6, 6, 126, 124}, {62, 126, 102, 102, 102, 102, 126, 62}};
    //uint8_t B[8] = {62, 102, 102, 62, 62, 102, 102, 62};
    //uint8_t C[8] = {124, 126, 6, 6, 6, 6, 126, 124};
    //uint8_t D[8] = {62, 126, 102, 102, 102, 102, 126, 62};
    hwlib::wait_us(250); // It takes 250 us for the MAX7221 to leave shutdown mode.
    MAX.Clear();
    hwlib::wait_ms(100);
    MAX.Fill();
    hwlib::wait_ms(500);
    MAX.Clear();
    hwlib::wait_ms(20);
    MAX.Setup();
    MAX.Test(A);
//    MAX.Test2(B, 2);
//    hwlib::wait_ms(1000);
//    MAX.Clear();
//    hwlib::wait_ms(500);
//    MAX.Setup();
//    MAX.Test2(B, 1);
//    MAX.Test2(A, 2);
    //MAX.Test2(C, 3);
    //MAX.Test2(D, 4);
    
//    for(;;){
//        MAX.Translate(Map);
//        hwlib::wait_ms(100);
//        MAX.Clear();
//        MAX.Setup();
//        MAX.Translate(Map2);
//        hwlib::wait_ms(100);
//        MAX.Clear();
//        MAX.Setup();
//    }
//    MAX.WriteTest(126);
    
    //SPI_Bus.write_and_read(Chip_Select, 8, Shutdown, Dummy);
    //SerialAddressingTest(Data_Output, Chip_Select, Clock, Data);
    //hwlib::wait_ms(3);
}
