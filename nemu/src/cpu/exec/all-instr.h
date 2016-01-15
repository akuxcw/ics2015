#include "prefix/prefix.h"

#include "data-mov/movsx.h"
#include "data-mov/movzx.h"
#include "data-mov/mov.h"
#include "data-mov/movsp.h"
#include "data-mov/cmovcc.h"
#include "data-mov/xchg.h"
#include "data-mov/push.h"
#include "data-mov/pusha.h"
#include "data-mov/pop.h"
#include "data-mov/popa.h"
#include "data-mov/leave.h"
#include "data-mov/ret.h"
#include "data-mov/iret.h"
#include "data-mov/cltd.h"
#include "data-mov/cwtl.h"
#include "data-mov/cld.h"
#include "data-mov/cli.h"
#include "data-mov/lgdt.h"
#include "data-mov/lidt.h"
#include "data-mov/std.h"
#include "data-mov/sti.h"
#include "data-mov/in.h"
#include "data-mov/out.h"
#include "data-mov/bt.h"

#include "control/jcc.h"
#include "control/call.h"
#include "control/jmp.h"
#include "control/int.h"
#include "control/hlt.h"

#include "arith/add.h"
#include "arith/adc.h"
#include "arith/sub.h"
#include "arith/sbb.h"
#include "arith/cmp.h"
#include "arith/dec.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"

#include "logic/test.h"
#include "logic/and.h"
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"
#include "logic/setcc.h"

#include "string/rep.h"
#include "string/stos.h"
#include "string/movs.h"
#include "string/cmps.h"

#include "misc/misc.h"

#include "special/special.h"
