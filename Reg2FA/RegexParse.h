#pragma once

#include "Reg2FA.h"
#include "Lex.h"

NFAPack *ParseStmt();
NFAPack *ParseItem();
NFAPack *ParseReplay(NFAPack *p0);