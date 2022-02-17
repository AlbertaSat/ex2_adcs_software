#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <redconf.h>
#include <redfs.h>
#include <redfse.h>
#include <redposix.h>
#include <redtests.h>
#include <redvolume.h>

#include "adcs_handler.h"

void binaryTest(void);

void CubeMag_Common_Test(bool);
void CubeTorquers_Common_Test(void);
void binaryTest_CubeSense1(void);
void ReactionWheels_Common_Test(uint8_t wheel_number);
void binaryTest_CubeACP();
