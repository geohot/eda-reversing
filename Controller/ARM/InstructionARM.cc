// InstructionARM.cc -- Apr 13, 2009
//    by geohot

//this is the ARM disassembler

#include "InstructionARM.h"             //just included here

using namespace eda;

InstructionARM::InstructionARM(Data opcode)
{
  mOpcode=opcode;
  init(); //keep work out of the constructor
}

bool InstructionARM::init()
{
  mEncodingARM=getEncodingARM(mOpcode);
  switch(mEncodingARM)
  {
    case ARM_DPIS:      //Data processing immediate shift
    case ARM_DPRS:      //Data processing register shift
    case ARM_DPI:       //Data processing immediate
      return initDataProcessing();
    case ARM_LSIO:      //Load/store immediate offset
    case ARM_LSRO:      //Load/store register offset
    case ARM_LSM:       //Load/store multiple
    case ARM_MELS:
      return initLoadStore();
    case ARM_MI:        //Miscellaneous instructions
      return initMiscellaneous();
    case ARM_BBL:       //Branch and branch with link
      return initBranches();
    default:
      return false;
  }
}

bool InstructionARM::initDataProcessing()
{
  templateInstructionARM *i=&mOpcode;
//********************Instruction********************
  mString.add(opcodesARM[i->dpis.opcode],DT_INSTRUCTION);
  if(i->dpis.S && (i->dpis.opcode<8 || i->dpis.opcode>=12))
    mString.add("S",DT_FLAGS);
  mString.add(arm_cond[i->generic.cond], DT_CONDITION);

//********************Data********************
//  **First**
  if(i->dpis.opcode<8 || i->dpis.opcode>=12)
  {
    mString.add(arm_reg[i->generic.Rd], DT_REGISTER);
    mString.add(", ", DT_FORMATTING);
  }
//  **Second**
  if(i->dpis.opcode!=OPCODE_MOV && i->dpis.opcode!=OPCODE_MVN)
  {
    mString.add(arm_reg[i->generic.Rn], DT_REGISTER);
    mString.add(", ", DT_FORMATTING);
  }

  if(encoding==ARM_DPI)
  {
    mString.add("#", DT_SYMBOL);
    eda_add_numeric( buf, ((i->dpi.immed) >> (i->dpi.rotate * 2)) | ((i->dpi.immed) << (32-(i->dpi.rotate * 2))) , EDA_IMMED_UNSIGNED );
  }

}
