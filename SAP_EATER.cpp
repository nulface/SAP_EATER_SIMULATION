#include <iostream>

constexpr auto NOP = 0b00000000;
constexpr auto LDA = 0b00010000;
constexpr auto ADD = 0b00100000;
constexpr auto SUB = 0b00110000;
constexpr auto STA = 0b01000000;
constexpr auto LDI = 0b01010000;
constexpr auto JMP = 0b01100000;
constexpr auto JC  = 0b01110000;
constexpr auto JZ  = 0b10000000;
constexpr auto OUT = 0b11100000;
constexpr auto HLT = 0b11110000;

int main()
{
    //16 bytes of ram
    char RAM[16] = {};

    //program portion of RAM
    RAM[0] = LDA | 0xD;
    RAM[1] = SUB | 0xE;
    RAM[2] = JZ  | 0x7;
    RAM[3] = LDA | 0xD;
    RAM[4] = ADD | 0xF;
    RAM[5] = STA | 0xD;
    RAM[6] = JMP | 0x0;
    RAM[7] = OUT;
    RAM[8] = HLT;

    //data portion of RAM
    RAM[0xD] = 0x01;
    RAM[0xE] = 0x0A;
    RAM[0xF] = 0x01;


    //A, B, and SUM registers
    int A = 0;
    int B = 0;
    int SUM;

    //output register
    int out = 0;

    //program counter
    int PC = 0;

    //instruction register
    int IR = 0;

    //Data bus
    //data cannot move directly between registers, it must move through the bus
    int BUS = 0;

    //flagged when result of arithmetic carrys
    bool carryFlag = false;

    //flagged when result of arithmetic is zero
    bool zeroFlag = false;

    //breaks loop
    bool halt = false;

    //need to set the first instruction
    IR = (RAM[PC] & 0xF0);

    std::cout << std::dec;

    while (!halt) {

        switch (IR) {
        case NOP:
            //do nothing
            std::cout << "NOP" << std::endl;
            BUS = (RAM[++PC] & 0xF0);
            IR = BUS;
            break;

        case LDA:

            BUS = RAM[RAM[PC] & 0x0F];
            A = BUS;


            BUS = (RAM[++PC] & 0xF0);
            IR = BUS;
            std::cout << "LDA: " << A << std::endl;
            break;

        case ADD:

            carryFlag = false;
            zeroFlag = false;

            BUS = RAM[RAM[PC] & 0x0F];
            B = BUS;

            SUM = A + B;

            if (SUM == 0) zeroFlag = true;

            if (SUM & 0x100) { 
                carryFlag = true;
                SUM = SUM & 0xFF;
            }

            BUS = SUM;
            A = BUS;

            std::cout << "ADD: " << SUM << std::endl;
            BUS = (RAM[++PC] & 0xF0);
            IR = BUS;
            break;

        case SUB:

            zeroFlag = false;
            B = RAM[RAM[PC] & 0x0F];
            
            SUM = A - B;

            if (SUM == 0) zeroFlag = true;

            BUS = SUM;
            A = BUS;

            std::cout << "SUB: " << SUM << std::endl;
            BUS = (RAM[++PC] & 0xF0);
            IR = BUS;
            break;

        case STA:

            RAM[RAM[PC] & 0x0F] = A;
            
            std::cout << "STA: " << A << " @ " << (RAM[PC] & 0x0F) << std::endl;
            BUS = (RAM[++PC] & 0xF0);
            IR = BUS;
            break;

        case LDI:

            A = RAM[RAM[PC] & 0x0F];

            std::cout << "LDI: " << A << " from: " << (int)(RAM[PC] & 0x0F) << std::endl;
            BUS = (RAM[++PC] & 0xF0);
            IR = BUS;
            break;

        case JMP:

            BUS = RAM[PC] & 0x0F;
            PC = BUS;

            std::cout << "JMP: " << (int)PC << std::endl;
            BUS = (RAM[PC] & 0xF0);
            IR = BUS;
            break;

        case JC:

            if (carryFlag) {
                PC = RAM[PC] & 0x0F;
                carryFlag = false;
                BUS = (RAM[PC] & 0xF0);
            }
            else
                BUS = (RAM[++PC] & 0xF0);

            IR = BUS;

            std::cout << "JC: " << (int)PC << std::endl;
            break;

        case JZ:
            if (zeroFlag) {
                BUS = RAM[PC] & 0x0F;
                PC = BUS;
                zeroFlag = false;
                BUS = (RAM[PC] & 0xF0);
            }
            else
                BUS = (RAM[++PC] & 0xF0);

            IR = BUS;
            std::cout << "JZ: " << (int)PC << std::endl;
            break;

        case OUT:
            
            BUS = A;
            out = BUS;

            std::cout << "OUT: " << (int)out << std::endl;
            BUS = (RAM[++PC] & 0xF0);
            IR = BUS;
            break;

        case HLT:

            std::cout << "HLT" << std::endl;
            halt = true;
            break;

        }
    }

}
