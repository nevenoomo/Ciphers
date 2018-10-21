#pragma once
#include "basic_block_cypher.h"
#include "mode.h"
#include "padder.h"

class ECB: public Mode{
    void run() override;
};