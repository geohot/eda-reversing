// templateARM.h -- Apr 13, 2009
//    by geohot

#ifndef EDA_TEMPLATEARM_H_
#define EDA_TEMPLATEARM_H_

#include <string>
#include "../../Model/ParsedInstruction.h"

typedef union
{
  struct          //Misc instructions
  {
    unsigned immed : 8;             //could be register too
    unsigned rotate : 4;
    unsigned Rd : 4;        //0000
    unsigned mb_c : 1;
    unsigned mb_x : 1;
    unsigned mb_s : 1;
    unsigned mb_f : 1;
    unsigned : 1;
    unsigned L : 1;
    unsigned R : 1;
    unsigned : 2;   //10
    unsigned I : 1;
    unsigned : 2;   //00
    unsigned cond : 4;
  } mi;
  struct          //Multiply extra loads stores
  {
    unsigned addr_mode_lo : 4;
    unsigned : 1;   //1
    unsigned H : 1;
    unsigned S : 1;
    unsigned : 1;   //1
    unsigned addr_mode_hi : 4;
    unsigned Rd : 4;
    unsigned Rn : 4;
    unsigned L : 1;
    unsigned W : 1;
    unsigned I : 1;
    unsigned U : 1;
    unsigned P : 1;
    unsigned : 3;   //000
    unsigned cond : 4;
  } mels;
  struct          //Data processing immediate shift
  {
    unsigned Rm : 4;
    unsigned : 1;   //0
    unsigned shift : 2;
    unsigned shift_imm : 5;
    unsigned Rd : 4;
    unsigned Rn : 4;
    unsigned S : 1;
    unsigned opcode : 4;
    unsigned : 3;   //000
    unsigned cond : 4;
  } dpis;
  struct          //Data processing register shift
  {
    unsigned Rm : 4;
    unsigned : 1;   //1
    unsigned shift : 2;
    unsigned : 1;   //0
    unsigned Rs : 4;
    unsigned Rd : 4;
    unsigned Rn : 4;
    unsigned S : 1;
    unsigned opcode : 4;
    unsigned : 3;   //000
    unsigned cond : 4;
  } dprs;
  struct          //Data processing immediate
  {
    unsigned immed : 8;
    unsigned rotate : 4;
    unsigned Rd : 4;
    unsigned Rn : 4;
    unsigned S : 1;
    unsigned opcode : 4;
    unsigned : 3;   //001
    unsigned cond : 4;
  } dpi;
  struct          //Move immediate to status register
  {
    unsigned immed : 8;
    unsigned rotate : 4;
    unsigned SBO : 4;
    unsigned Mask : 4;
    unsigned : 2;   //10
    unsigned R : 1;
    unsigned : 2;   //10
    unsigned : 3;   //001
    unsigned cond : 4;
  } misr;
  struct          //Load/store immediate offset
  {
    unsigned immed : 12;
    unsigned Rd : 4;
    unsigned Rn : 4;
    unsigned L : 1;
    unsigned W : 1;
    unsigned B : 1;
    unsigned U : 1;
    unsigned P : 1;
    unsigned : 3;   //010
    unsigned cond : 4;
  } lsio;
  struct          //Load/store register offset
  {
    unsigned Rm : 4;
    unsigned : 1;   //0
    unsigned shift : 2;
    unsigned shift_imm : 5;
    unsigned Rd : 4;
    unsigned Rn : 4;
    unsigned L : 1;
    unsigned W : 1;
    unsigned B : 1;
    unsigned U : 1;
    unsigned P : 1;
    unsigned : 3;   //011
    unsigned cond : 4;
  } lsro;
  struct          //Load/store multiple
  {
    unsigned register_list : 16;
    unsigned Rn : 4;
    unsigned L : 1;
    unsigned W : 1;
    unsigned S : 1;
    unsigned U : 1;
    unsigned P : 1;
    unsigned : 3;   //100
    unsigned cond : 4;
  } lsm;
  struct          //Branch and branch with link
  {
    unsigned offset : 24;
    unsigned L : 1;
    unsigned : 3;   //101
    unsigned cond : 4;
  } bbl;
  struct          //Coprocessor load/store and double register transfers
  {
    unsigned offset : 8;
    unsigned cp_num : 4;
    unsigned CRd : 4;
    unsigned Rn : 4;
    unsigned L : 1;
    unsigned W : 1;
    unsigned N : 1;
    unsigned U : 1;
    unsigned P : 1;
    unsigned : 3;   //110
    unsigned cond : 4;
  } cls;
  struct          //Coprocessor data processing
  {
    unsigned CRm : 4;
    unsigned : 1;   //0
    unsigned opcode2 : 3;
    unsigned cp_num : 4;
    unsigned CRd : 4;
    unsigned CRn : 4;
    unsigned opcode1 : 4;
    unsigned : 4;   //1110
    unsigned cond : 4;
  } cdp;
  struct          //Coprocessor register transfers
  {
    unsigned CRm : 4;
    unsigned : 1;   //1
    unsigned opcode2 : 3;
    unsigned cp_num : 4;
    unsigned Rd : 4;
    unsigned CRn : 4;
    unsigned L : 1;
    unsigned opcode1 : 3;
    unsigned : 4;   //1110
    unsigned cond : 4;
  } crt;
  struct          //Software interrupt
  {
    unsigned swi_number : 24;
    unsigned : 4;   //1111
    unsigned cond : 4;
  } swi;
  struct          //generic
  {
    unsigned : 12;
    unsigned Rd : 4;
    unsigned Rn : 4;
    unsigned : 8;
    unsigned cond : 4;
  } generic;
  Data opcode;
} templateInstructionARM;

#define conditionsARM(x) std::make_pair((std::string)conditionsARM_s[x], DT_CONDITION)
#define registersARM(x) std::make_pair((std::string)registersARM_s[x], DT_REGISTER)
#define opcodesARM(x) std::make_pair((std::string)opcodesARM_s[x], DT_OPCODE)
#define shiftsARM(x) std::make_pair((std::string)shiftsARM_s[x], DT_SUBOPCODE)

const char conditionsARM_s[16][3]={"EQ","NE","HS","LO","MI","PL","VS","VC","HI","LS","GE","LT","GT","LE","",""};
const char registersARM_s[18][5]={"R0","R1","R2","R3","R4","R5","R6","R7","R8","R9","R10","R11","R12","SP","LR","PC","CPSR","SPSR"};
const char opcodesARM_s[16][4]={"AND","EOR","SUB","RSB","ADD","ADC","SBC","RSC","TST","TEQ","CMP","CMN","ORR","MOV","BIC","MVN"};
const char shiftsARM_s[4][4]={"LSL","LSR","ASR","ROR"};

//execution modes
#define M_USER 0x10
#define M_FIQ 0x11
#define M_IRQ 0x12
#define M_SUPERVISOR 0x13
#define M_ABORT 0x17
#define M_UNDEFINED 0x1B
#define M_SYSTEM 0x1F

#define SHIFT_LSL 0
#define SHIFT_LSR 1
#define SHIFT_ASR 2
#define SHIFT_ROR 3

typedef struct
{
  //byte
  unsigned M : 5; //Mode
  unsigned T : 1; //Thumb
  unsigned F : 1; //FIQ interrupts
  unsigned I : 1; //IRQ interrupts
  //byte
  unsigned A : 1;         //v6 Disable inprecise data aborts
  unsigned E : 1;         //v6 Endianness
  unsigned : 6;
  //byte
  unsigned GE : 4;        //v6 greater than or equal
  unsigned : 4;
  //byte
  unsigned J : 1;         //v6 Jazelle
  unsigned : 2;
  unsigned Q : 1;
  unsigned V : 1; //oVerflow
  unsigned C : 1; //Carry
  unsigned Z : 1; //Zero
  unsigned N : 1; //Negative
} templateRegisterCPSR;

const char encodingsARM[15][0x10]={
        "ARM_UNKNOWN",
        "ARM_MI",
        "ARM_MELS",
        "ARM_DPIS",
        "ARM_DPRS",
        "ARM_DPI",
        "ARM_MISR",
        "ARM_LSIO",
        "ARM_LSRO",
        "ARM_LSM",
        "ARM_BBL",
        "ARM_CLS",
        "ARM_CDP",
        "ARM_CRT",
        "ARM_SWI"};

#define OPCODE_AND 0x0
#define OPCODE_EOR 0x1
#define OPCODE_SUB 0x2
#define OPCODE_RSB 0x3
#define OPCODE_ADD 0x4
#define OPCODE_ADC 0x5
#define OPCODE_SBC 0x6
#define OPCODE_RSC 0x7
#define OPCODE_TST 0x8
#define OPCODE_TEQ 0x9
#define OPCODE_CMP 0xA
#define OPCODE_CMN 0xB
#define OPCODE_ORR 0xC
#define OPCODE_MOV 0xD
#define OPCODE_BIC 0xE
#define OPCODE_MVN 0xF

//encodings
#define ARM_UNKNOWN 0
#define ARM_MI 1
#define ARM_MELS 2
#define ARM_DPIS 3
#define ARM_DPRS 4
#define ARM_DPI 5
#define ARM_MISR 6
#define ARM_LSIO 7
#define ARM_LSRO 8
#define ARM_LSM 9
#define ARM_BBL 10
#define ARM_CLS 11
#define ARM_CDP 12
#define ARM_CRT 13
#define ARM_SWI 14

#define REG_SP 13
#define REG_LR 14
#define REG_PC 15
#define REG_CPSR 16
#define REG_SPSR 17




#endif /* EDA_TEMPLATEARM_H_ */
