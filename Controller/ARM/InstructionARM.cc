// InstructionARM.cc -- Apr 13, 2009
//    by geohot

//this is the ARM disassembler

#include "InstructionARM.h"
#include "templateARM.h"        //just included here

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
  templateInstructionARM *i=(templateInstructionARM *)&mOpcode;
  StatelessData *right;

//********************Instruction********************
  mString << opcodesARM(i->dpis.opcode);
  if(i->dpis.S && (i->dpis.opcode<8 || i->dpis.opcode>=12))
    mString.add("S",DT_FLAGS);
  mString << conditionsARM(i->generic.cond) << " ";

//********************Data********************
//  **First**
  if(i->dpis.opcode<8 || i->dpis.opcode>=12)
  {
    mString << registersARM(i->generic.Rd) << ", ";
  }
//  **Second**
  if(i->dpis.opcode!=OPCODE_MOV && i->dpis.opcode!=OPCODE_MVN)
  {
    mString << registersARM(i->generic.Rn) << ", ";
  }

  if(mEncodingARM==ARM_DPI)                 //Data processing immediate
  {
    Data t=ror(i->dpi.immed, i->dpi.rotate*2);
    right=new StatelessData(t);
    mString << t;       //immediate
  }
  else if(mEncodingARM==ARM_DPIS)           //Data processing immediate shift
    //R3 LSL 4
  {
    mString << registersARM(i->dpis.Rm);
    if(i->dpis.shift_imm!=0 || i->dpis.shift!=0)        //this shift is real
    {
      mString << " " << shiftsARM(i->dpis.shift) << " " << i->dpis.shift_imm;
      right=new StatelessData((int)i->dpis.Rm,
          OPERATION_LSL+(i->dpis.shift),
          new StatelessData((Data)i->dpis.shift_imm));
    }
    else        //just the register
    {
      right=new StatelessData((int)i->dpis.Rm);
    }
  }
  else if(mEncodingARM==ARM_DPRS)             //Data processing register shift
  {
    mString << registersARM(i->dprs.Rm) << " " << shiftsARM(i->dprs.shift) << " " << registersARM(i->dprs.Rs);
    right=new StatelessData((int)i->dprs.Rm, OPERATION_LSL+(i->dprs.shift), new StatelessData((int)i->dprs.Rs));
  }
//wrapping right in operator
  if(i->dpis.opcode!=OPCODE_MOV && i->dpis.opcode!=OPCODE_MVN)
    right=new StatelessData((int)i->generic.Rn, i->dpis.opcode, right);
//adding the action to the changelist
  mAction.addChange(StatelessData((int)i->generic.Rd), *right);
  delete right;         //done with this, should've been copied into the stateless_changelist
  return true;
}

bool InstructionARM::initLoadStore()
{
  return false;
}

bool InstructionARM::initMiscellaneous()
{
  return false;
}

bool InstructionARM::initBranches()
{
  return false;
}

int InstructionARM::getEncodingARM(Data opcode)
{
  if((opcode&0x0E000090)==0x00000090) return ARM_MELS;

  if((opcode&0x0F900000)==0x01000000) return ARM_MI;

  if((opcode&0x0E000010)==0x00000000) return ARM_DPIS;
  if((opcode&0x0E000090)==0x00000010) return ARM_DPRS;

  if((opcode&0x0FB00000)==0x03200000) return ARM_MISR;
  if((opcode&0x0E000000)==0x02000000) return ARM_DPI;

  if((opcode&0x0E000000)==0x04000000) return ARM_LSIO;
  if((opcode&0x0E000010)==0x06000000) return ARM_LSRO;

  if((opcode&0x0E000000)==0x08000000) return ARM_LSM;

  if((opcode&0x0E000000)==0x0A000000) return ARM_BBL;

  if((opcode&0x0E000000)==0x0C000000) return ARM_CLS;
  if((opcode&0x0F000010)==0x0E000000) return ARM_CDP;
  if((opcode&0x0F000010)==0x0E000010) return ARM_CRT;

  if((opcode&0x0F000000)==0x0F000000) return ARM_SWI;

  return ARM_UNKNOWN;
}
