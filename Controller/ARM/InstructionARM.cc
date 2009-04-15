// InstructionARM.cc -- Apr 13, 2009
//    by geohot
//  released under GPLv3, see http://gplv3.fsf.org/

//this is the ARM disassembler

#include "InstructionARM.h"     //just included here
#include "templateARM.h"        //just included here

using namespace eda;

InstructionARM::InstructionARM(Data opcode)
{
  mOpcode=opcode;
  if(!init())
    mString << "UNDEFINED"; //keep work out of the constructor
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
    mString.add("S",DT_FLAG);
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
  //else right stays the same
//adding the action to the changelist
  mAction.addChange(StatelessData((int)i->generic.Rd), *right);
  delete right;         //done with this, should've been copied into the stateless_changelist
  return true;
}

bool InstructionARM::initLoadStore()
{
  templateInstructionARM *i=(templateInstructionARM *)&mOpcode;
//********************Instruction********************
  mString.add( (i->lsio.L)?"LD":"ST", DT_FLAG);
  mString.add( (mEncodingARM==ARM_LSM)?"M":"R" , DT_OPCODE);

  if( (mEncodingARM==ARM_LSIO || mEncodingARM==ARM_LSRO) && i->lsio.B )
    mString.add("B",DT_FLAG);
  else if(mEncodingARM==ARM_LSM)
  {
    mString.add( (i->lsm.U)?"I":"D", DT_FLAG);
    mString.add( (i->lsm.P)?"B":"A", DT_FLAG);
  }
  else if(mEncodingARM==ARM_MELS)
    if(i->mels.H) mString.add("H",DT_FLAG);

  mString << conditionsARM(i->generic.cond) << " ";
//********************Data********************
//  **First**
  if(mEncodingARM==ARM_LSM)
  {
    mString << registersARM(i->generic.Rn);
    if(i->lsm.W) mString.add("!",DT_FLAG);
    mString << ", ";
  }
  else
    mString << registersARM(i->generic.Rd) << ", ";
//  **Second**
  if(mEncodingARM==ARM_LSM)
  {
    mString.add("{",DT_SYMBOL);

    int reg=i->lsm.register_list;
    int oper=(i->lsm.U)?OPERATION_ADD:OPERATION_SUB;
    int rcount=0;
    StatelessData *offset;
    for(int a=0;a<16;a++)       //the registers
    {
      if(reg&0x1)
      {
        //add the change
        if(i->lsm.P) rcount++;
        offset=new StatelessData((Data)rcount*4);
        if(!(i->lsm.P)) rcount++;

        offset=new StatelessData(i->generic.Rn, oper, offset);

        mAction.addChange(
            StatelessData(OPERATION_DEREF, offset),
            StatelessData(a));
        //print it
        mString << registersARM(a);
        reg>>=1;
        if(reg>0)
          mString << ", ";
      }
      else reg>>=1;
    }
    mString.add("}",DT_SYMBOL);
    if(i->lsm.W)        //handle writeback
      mAction.addChange(StatelessData((int)i->generic.Rn),
          StatelessData(i->generic.Rn, oper, new StatelessData((Data)rcount*4) ) );
  }
  else
  {
    mString.add("[",DT_SYMBOL);
    mString << registersARM(i->generic.Rn);
    if(mEncodingARM==ARM_LSIO)
    {
      if(i->lsio.immed!=0)
      {
        mString << ", ";
        mString.add(((i->lsio.U)?(1):(-1))*(i->lsio.immed), DT_SIGNED);
        StatelessData *g;
        if(i->lsio.U)
          g=new StatelessData(i->generic.Rn, OPERATION_ADD, new StatelessData((Data)i->lsio.immed));
        else
          g=new StatelessData(i->generic.Rn, OPERATION_SUB, new StatelessData((Data)i->lsio.immed));
        mAction.addChange(StatelessData(i->generic.Rd), StatelessData(OPERATION_DEREF, g));
      }
      else
        mAction.addChange(StatelessData(i->generic.Rd), StatelessData(OPERATION_DEREF, new StatelessData(i->generic.Rn)));
    }
    else if(mEncodingARM==ARM_LSRO)
    {
      StatelessData *right;
      mString << ", " << registersARM(i->lsro.Rm);
      if(i->lsro.shift_imm!=0 || i->lsro.shift!=0)
      {
        mString << " " << shiftsARM(i->lsro.shift) << " " << i->lsro.shift_imm;
        right=new StatelessData((int)i->lsro.Rm,
          OPERATION_LSL+(i->lsro.shift),
          new StatelessData((Data)i->lsro.shift_imm));
      }
      else right=new StatelessData((int)i->lsro.Rm);
      right=new StatelessData((int)i->generic.Rn, OPERATION_ADD, right);
      mAction.addChange(StatelessData(i->generic.Rd), StatelessData(OPERATION_DEREF, right));
    }
    mString.add("]",DT_SYMBOL);
  }
  return true;
}

bool InstructionARM::initBranches()
{
  templateInstructionARM *i=(templateInstructionARM *)&mOpcode;
//********************Instruction********************
  mString.add("B",DT_OPCODE);
  if(i->bbl.L) mString.add("L",DT_FLAG);
  mString << conditionsARM(i->generic.cond) << " ";
//********************Data********************
//  **First**
  int bl=((i->bbl.offset&0x800000)?(0xFC000000):0)|(i->bbl.offset<<2);
  if(bl<0) mString << registersARM(REG_PC) << "-" << (Data)bl;
  else if(bl>0) mString << registersARM(REG_PC) << "+" << (Data)bl;
  else mString << registersARM(REG_PC);

  mAction.addChange(StatelessData((int)REG_PC),
      StatelessData((int)REG_PC, OPERATION_ADD, new StatelessData((Data)bl) ) );

  if(i->bbl.L)
    mAction.addChange(StatelessData((int)REG_LR), StatelessData((int)REG_PC));   //LR=PC

  return true;
}

bool InstructionARM::initMiscellaneous()
{
  //i don't know what these do
  templateInstructionARM *i=(templateInstructionARM *)&mOpcode;
//********************Instruction********************
  mString.add("M",DT_OPCODE);
  mString.add((i->mi.L)?"SR":"RS",DT_FLAG);
  mString << conditionsARM(i->generic.cond) << " ";
//********************Data********************
//  **First**
  if(!(i->mi.L))
  {
    mString << registersARM(i->generic.Rd) << ", ";
  }
//  **Second**
  mString << registersARM(16+i->mi.R);
  if(i->mi.L)
  {
    mString.add("_",DT_SYMBOL);
    if(i->mi.mb_c) mString.add("c",DT_SYMBOL);
    if(i->mi.mb_x) mString.add("x",DT_SYMBOL);
    if(i->mi.mb_s) mString.add("s",DT_SYMBOL);
    if(i->mi.mb_f) mString.add("f",DT_SYMBOL);
  }

  return true;
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
