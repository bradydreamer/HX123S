//��������ƽ̨�õ�AID
const SDK_ICC_EmvSysIni_STRUCT CMBAid[] = {
//Master AID
        {6, "\xA0\x00\x00\x00\x04\x10",         0, "\x00\x02",
                {0xFC, 0x50, 0xAC, 0xA0, 0x00},
                {0xF8, 0x50, 0xAC, 0xF8, 0x00},
                {0x04, 0x00, 0x00, 0x00, 0x00},
                {0}, {0}, 0, 0, 3, {0x9F, 0x37, 0x04}, 1,
                {0}, {0}, {0}, {0}, 0, 0, 0, {0}, 0},

        {6, "\xA0\x00\x00\x00\x04\x30",         0, "\x00\x02",
                {0xFC, 0x50, 0xAC, 0xA0, 0x00},
                {0xF8, 0x50, 0xAC, 0xF8, 0x00},
                {0x04, 0x00, 0x00, 0x00, 0x00},
                {0}, {0}, 0, 0, 3, {0x9F, 0x37, 0x04}, 1,
                {0}, {0}, {0}, {0}, 0, 0, 0, {0}, 0},
//Visa AID
        {5, "\xA0\x00\x00\x00\x03",             0, "\x00\x8c",
                {0xD8, 0x40, 0x00, 0xA8, 0x00}, //default
                {0xD8, 0x40, 0x04, 0xF8, 0x00}, //online//D84004F800
                {0x00, 0x10, 0x00, 0x00, 0x00}, //decline//D84000A800
                {0}, {0}, 0, 0, 3, {0x9F, 0x37, 0x04}, 1,
                {0}, {0}, {0}, {0}, 0, 0, 0, {0}, 0},

//JCB AID
        {7, "\xA0\x00\x00\x00\x65\x10\x10",     0, "\x00\x21",
                {0xfc, 0x60, 0x24, 0x28, 0x00},
                {0xfc, 0x60, 0xac, 0xf8, 0x00},
                {0x00, 0x10, 0x00, 0x00, 0x00},
                {0}, {0}, 0, 0, 0, {0},                1,
                {0}, {0}, {0}, {0}, 0, 0, 0, {0}, 0},
// PBOC
        {8, "\xA0\x00\x00\x03\x33\x01\x01\x06", 0, "\x00\x20", //AidLen, AID, Asi, AppVerNum
                {0x08, 0x40, 0x00, 0xA8, 0x00}, //TacDefault   DF11
                {0xD8, 0x40, 0x04, 0xF8, 0x00}, //TacOnline    DF12
                {0x00, 0x10, 0x00, 0x00, 0x00}, //TacDecline   DF13
                {0}, {0}, 0, 0, 0, {0x9F, 0x37, 0x04}, 0, //���в�֧��ONLINEPIN
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                     {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                          {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                               {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                                    0, 0, 0, {0}, 0},


        {8, "\xA0\x00\x00\x03\x33\x01\x01\x01", 0, "\x00\x20", //AidLen, AID, Asi, AppVerNum
                {0x08, 0x40, 0x00, 0xA8, 0x00}, //TacDefault   DF11
                {0xD8, 0x40, 0x04, 0xF8, 0x00}, //TacOnline    DF12
                {0x00, 0x10, 0x00, 0x00, 0x00}, //TacDecline   DF13
                {0}, {0}, 0, 0, 0, {0x9F, 0x37, 0x04}, 1, //���в�֧��ONLINEPIN
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                     {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                          {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                               {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                                    0, 0, 0, {0}, 0},
        {8, "\xA0\x00\x00\x03\x33\x01\x01\x02", 0, "\x00\x20", //AidLen, AID, Asi, AppVerNum
                {0x08, 0x40, 0x00, 0xA8, 0x00}, //TacDefault   DF11
                {0xD8, 0x40, 0x04, 0xF8, 0x00}, //TacOnline    DF12
                {0x00, 0x10, 0x00, 0x00, 0x00}, //TacDecline   DF13
                {0}, {0}, 0, 0, 0, {0x9F, 0x37, 0x04}, 1,
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                     {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                          {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                               {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                                    0, 0, 0, {0}, 0},
        {8, "\xA0\x00\x00\x03\x33\x01\x01\x03", 0, "\x00\x20", //AidLen, AID, Asi, AppVerNum
                {0x08, 0x40, 0x00, 0xA8, 0x00}, //TacDefault   DF11
                {0xD8, 0x40, 0x04, 0xF8, 0x00}, //TacOnline    DF12
                {0x00, 0x10, 0x00, 0x00, 0x00}, //TacDecline   DF13
                {0}, {0}, 0, 0, 0, {0x9F, 0x37, 0x04}, 1,
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                     {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                          {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                               {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                                    0, 0, 0, {0}, 0},
};

//��������ƽ̨�õ�CAPK
const SDK_ICC_CAPK_STRUCT CMBCapk[] = {
//VISA ��CAPK===============================
//��Ӧ�ṹCAPK_STRUCT,VSIA��1�鹫Կ
        {{0xA0, 0x00, 0x00, 0x00, 0x03},
                0x01,
                0x01,
                0x01,
                128,
                {0xC6, 0x96, 0x03, 0x42, 0x13, 0xD7, 0xD8, 0x54, 0x69, 0x84, 0x57, 0x9D, 0x1D, 0x0F, 0x0E, 0xA5,
                                                                                                                 0x19, 0xCF, 0xF8, 0xDE, 0xFF, 0xC4, 0x29, 0x35, 0x4C, 0xF3, 0xA8, 0x71, 0xA6, 0xF7, 0x18, 0x3F,
                                                                                                                                                                                                                 0x12, 0x28, 0xDA, 0x5C, 0x74, 0x70, 0xC0, 0x55, 0x38, 0x71, 0x00, 0xCB, 0x93, 0x5A, 0x71, 0x2C,
                                                                                                                                                                                                                                                                                                                 0x4E, 0x28, 0x64, 0xDF, 0x5D, 0x64, 0xBA, 0x93, 0xFE, 0x7E, 0x63, 0xE7, 0x1F, 0x25, 0xB1, 0xE5,
                                                                                                                                                                                                                                                                                                                                                                                                                 0xF5, 0x29, 0x85, 0x75, 0xEB, 0xE1, 0xC6, 0x3A, 0xA6, 0x17, 0x70, 0x69, 0x17, 0x91, 0x1D, 0xC2,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xA7, 0x5A, 0xC2, 0x8B, 0x25, 0x1C, 0x7E, 0xF4, 0x0F, 0x23, 0x65, 0x91, 0x24, 0x90, 0xB9, 0x39,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xBC, 0xA2, 0x12, 0x4A, 0x30, 0xA2, 0x8F, 0x54, 0x40, 0x2C, 0x34, 0xAE, 0xCA, 0x33, 0x1A, 0xB6,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x7E, 0x1E, 0x79, 0xB2, 0x85, 0xDD, 0x57, 0x71, 0xB5, 0xD9, 0xFF, 0x79, 0xEA, 0x63, 0x0B, 0x75},
                1,
                {0x03},
                0x14,
                {0xD3, 0x4A, 0x6A, 0x77, 0x60, 0x11, 0xC7, 0xE7, 0xCE, 0x3A, 0xEC, 0x5F, 0x03, 0xAD, 0x2F, 0x8C,
                        0xFC, 0x55, 0x03, 0xCC},
                {0x20, 0x16, 0x12, 0x31}},

//��Ӧ�ṹCAPK_STRUCT,VSIA��2�鹫Կ
        {{0xA0, 0x00, 0x00, 0x00, 0x03},
                0x07,
                0x01,
                0x01,
                144,
                {0xA8, 0x9F, 0x25, 0xA5, 0x6F, 0xA6, 0xDA, 0x25, 0x8C, 0x8C, 0xA8, 0xB4, 0x04, 0x27, 0xD9, 0x27,
                                                                                                                 0xB4, 0xA1, 0xEB, 0x4D, 0x7E, 0xA3, 0x26, 0xBB, 0xB1, 0x2F, 0x97, 0xDE, 0xD7, 0x0A, 0xE5, 0xE4,
                                                                                                                                                                                                                 0x48, 0x0F, 0xC9, 0xC5, 0xE8, 0xA9, 0x72, 0x17, 0x71, 0x10, 0xA1, 0xCC, 0x31, 0x8D, 0x06, 0xD2,
                                                                                                                                                                                                                                                                                                                 0xF8, 0xF5, 0xC4, 0x84, 0x4A, 0xC5, 0xFA, 0x79, 0xA4, 0xDC, 0x47, 0x0B, 0xB1, 0x1E, 0xD6, 0x35,
                                                                                                                                                                                                                                                                                                                                                                                                                 0x69, 0x9C, 0x17, 0x08, 0x1B, 0x90, 0xF1, 0xB9, 0x84, 0xF1, 0x2E, 0x92, 0xC1, 0xC5, 0x29, 0x27,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x6D, 0x8A, 0xF8, 0xEC, 0x7F, 0x28, 0x49, 0x20, 0x97, 0xD8, 0xCD, 0x5B, 0xEC, 0xEA, 0x16, 0xFE,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x40, 0x88, 0xF6, 0xCF, 0xAB, 0x4A, 0x1B, 0x42, 0x32, 0x8A, 0x1B, 0x99, 0x6F, 0x92, 0x78, 0xB0,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xB7, 0xE3, 0x31, 0x1C, 0xA5, 0xEF, 0x85, 0x6C, 0x2F, 0x88, 0x84, 0x74, 0xB8, 0x36, 0x12, 0xA8,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x2E, 0x4E, 0x00, 0xD0, 0xCD, 0x40, 0x69, 0xA6, 0x78, 0x31, 0x40, 0x43, 0x3D, 0x50, 0x72, 0x5F},
                1,
                {0x03},
                0x14,
                {0xB4, 0xBC, 0x56, 0xCC, 0x4E, 0x88, 0x32, 0x49, 0x32, 0xCB, 0xC6, 0x43, 0xD6, 0x89, 0x8F, 0x6F,
                        0xE5, 0x93, 0xB1, 0x72},
                {0x20, 0x16, 0x12, 0x31}},

//��Ӧ�ṹCAPK_STRUCT,VSIA��3�鹫Կ
        {{0xA0, 0x00, 0x00, 0x00, 0x03},
                0x08,
                0x01,
                0x01,
                176,
                {0xD9, 0xFD, 0x6E, 0xD7, 0x5D, 0x51, 0xD0, 0xE3, 0x06, 0x64, 0xBD, 0x15, 0x70, 0x23, 0xEA, 0xA1,
                                                                                                                 0xFF, 0xA8, 0x71, 0xE4, 0xDA, 0x65, 0x67, 0x2B, 0x86, 0x3D, 0x25, 0x5E, 0x81, 0xE1, 0x37, 0xA5,
                                                                                                                                                                                                                 0x1D, 0xE4, 0xF7, 0x2B, 0xCC, 0x9E, 0x44, 0xAC, 0xE1, 0x21, 0x27, 0xF8, 0x7E, 0x26, 0x3D, 0x3A,
                                                                                                                                                                                                                                                                                                                 0xF9, 0xDD, 0x9C, 0xF3, 0x5C, 0xA4, 0xA7, 0xB0, 0x1E, 0x90, 0x70, 0x00, 0xBA, 0x85, 0xD2, 0x49,
                                                                                                                                                                                                                                                                                                                                                                                                                 0x54, 0xC2, 0xFC, 0xA3, 0x07, 0x48, 0x25, 0xDD, 0xD4, 0xC0, 0xC8, 0xF1, 0x86, 0xCB, 0x02, 0x0F,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x68, 0x3E, 0x02, 0xF2, 0xDE, 0xAD, 0x39, 0x69, 0x13, 0x3F, 0x06, 0xF7, 0x84, 0x51, 0x66, 0xAC,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xEB, 0x57, 0xCA, 0x0F, 0xC2, 0x60, 0x34, 0x45, 0x46, 0x98, 0x11, 0xD2, 0x93, 0xBF, 0xEF, 0xBA,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xFA, 0xB5, 0x76, 0x31, 0xB3, 0xDD, 0x91, 0xE7, 0x96, 0xBF, 0x85, 0x0A, 0x25, 0x01, 0x2F, 0x1A,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xE3, 0x8F, 0x05, 0xAA, 0x5C, 0x4D, 0x6D, 0x03, 0xB1, 0xDC, 0x2E, 0x56, 0x86, 0x12, 0x78, 0x59,
                        0x38, 0xBB, 0xC9, 0xB3, 0xCD, 0x3A, 0x91, 0x0C, 0x1D, 0xA5, 0x5A, 0x5A, 0x92, 0x18, 0xAC, 0xE0,
                        0xF7, 0xA2, 0x12, 0x87, 0x75, 0x26, 0x82, 0xF1, 0x58, 0x32, 0xA6, 0x78, 0xD6, 0xE1, 0xED, 0x0B},
                1,
                {0x03},
                0x14,
                {0x20, 0xD2, 0x13, 0x12, 0x69, 0x55, 0xDE, 0x20,
                                                                 0x5A, 0xDC, 0x2F, 0xD2, 0x82, 0x2B, 0xD2, 0x2D,
                        0xE2, 0x1C, 0xF9, 0xA8},
                {0x20, 0x16, 0x12, 0x31}},

//��Ӧ�ṹCAPK_STRUCT,VSIA��4�鹫Կ
        {{0xA0, 0x00, 0x00, 0x00, 0x03},
                0x09,
                0x01,
                0x01,
                248,
                {0x9D, 0x91, 0x22, 0x48, 0xDE, 0x0A, 0x4E, 0x39, 0xC1, 0xA7, 0xDD, 0xE3, 0xF6, 0xD2, 0x58, 0x89,
                                                                                                                 0x92, 0xC1, 0xA4, 0x09, 0x5A, 0xFB, 0xD1, 0x82, 0x4D, 0x1B, 0xA7, 0x48, 0x47, 0xF2, 0xBC, 0x49,
                                                                                                                                                                                                                 0x26, 0xD2, 0xEF, 0xD9, 0x04, 0xB4, 0xB5, 0x49, 0x54, 0xCD, 0x18, 0x9A, 0x54, 0xC5, 0xD1, 0x17,
                                                                                                                                                                                                                                                                                                                 0x96, 0x54, 0xF8, 0xF9, 0xB0, 0xD2, 0xAB, 0x5F, 0x03, 0x57, 0xEB, 0x64, 0x2F, 0xED, 0xA9, 0x5D,
                                                                                                                                                                                                                                                                                                                                                                                                                 0x39, 0x12, 0xC6, 0x57, 0x69, 0x45, 0xFA, 0xB8, 0x97, 0xE7, 0x06, 0x2C, 0xAA, 0x44, 0xA4, 0xAA,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x06, 0xB8, 0xFE, 0x6E, 0x3D, 0xBA, 0x18, 0xAF, 0x6A, 0xE3, 0x73, 0x8E, 0x30, 0x42, 0x9E, 0xE9,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xBE, 0x03, 0x42, 0x7C, 0x9D, 0x64, 0xF6, 0x95, 0xFA, 0x8C, 0xAB, 0x4B, 0xFE, 0x37, 0x68, 0x53,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xEA, 0x34, 0xAD, 0x1D, 0x76, 0xBF, 0xCA, 0xD1, 0x59, 0x08, 0xC0, 0x77, 0xFF, 0xE6, 0xDC, 0x55,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x21, 0xEC, 0xEF, 0x5D, 0x27, 0x8A, 0x96, 0xE2, 0x6F, 0x57, 0x35, 0x9F, 0xFA, 0xED, 0xA1, 0x94,
                        0x34, 0xB9, 0x37, 0xF1, 0xAD, 0x99, 0x9D, 0xC5, 0xC4, 0x1E, 0xB1, 0x19, 0x35, 0xB4, 0x4C, 0x18,
                        0x10, 0x0E, 0x85, 0x7F, 0x43, 0x1A, 0x4A, 0x5A, 0x6B, 0xB6, 0x51, 0x14, 0xF1, 0x74, 0xC2, 0xD7,
                        0xB5, 0x9F, 0xDF, 0x23, 0x7D, 0x6B, 0xB1, 0xDD, 0x09, 0x16, 0xE6, 0x44, 0xD7, 0x09, 0xDE, 0xD5,
                        0x64, 0x81, 0x47, 0x7C, 0x75, 0xD9, 0x5C, 0xDD, 0x68, 0x25, 0x46, 0x15, 0xF7, 0x74, 0x0E, 0xC0,
                        0x7F, 0x33, 0x0A, 0xC5, 0xD6, 0x7B, 0xCD, 0x75, 0xBF, 0x23, 0xD2, 0x8A, 0x14, 0x08, 0x26, 0xC0,
                        0x26, 0xDB, 0xDE, 0x97, 0x1A, 0x37, 0xCD, 0x3E, 0xF9, 0xB8, 0xDF, 0x64, 0x4A, 0xC3, 0x85, 0x01,
                        0x05, 0x01, 0xEF, 0xC6, 0x50, 0x9D, 0x7A, 0x41},
                1,
                {0x03},
                0x14,
                {0x1F, 0xF8, 0x0A, 0x40, 0x17, 0x3F, 0x52, 0xD7,
                                                                 0xD2, 0x7E, 0x0F, 0x26, 0xA1, 0x46, 0xA1, 0xC8,
                        0xCC, 0xB2, 0x90, 0x46},
                {0x20, 0x16, 0x12, 0x31}},

//MASTER CAPK==========================
//��Ӧ�ṹCAPK_STRUCT,MASTER��1�鹫Կ
        {{0xA0, 0x00, 0x00, 0x00, 0x04},
                0x03,
                0x01,
                0x01,
                128,
                {0xC2, 0x49, 0x07, 0x47, 0xFE, 0x17, 0xEB, 0x05, 0x84, 0xC8, 0x8D, 0x47, 0xB1, 0x60, 0x27, 0x04,
                                                                                                                 0x15, 0x0A, 0xDC, 0x88, 0xC5, 0xB9, 0x98, 0xBD, 0x59, 0xCE, 0x04, 0x3E, 0xDE, 0xBF, 0x0F, 0xFE,
                                                                                                                                                                                                                 0xE3, 0x09, 0x3A, 0xC7, 0x95, 0x6A, 0xD3, 0xB6, 0xAD, 0x45, 0x54, 0xC6, 0xDE, 0x19, 0xA1, 0x78,
                                                                                                                                                                                                                                                                                                                 0xD6, 0xDA, 0x29, 0x5B, 0xE1, 0x5D, 0x52, 0x20, 0x64, 0x5E, 0x3C, 0x81, 0x31, 0x66, 0x6F, 0xA4,
                                                                                                                                                                                                                                                                                                                                                                                                                 0xBE, 0x5B, 0x84, 0xFE, 0x13, 0x1E, 0xA4, 0x4B, 0x03, 0x93, 0x07, 0x63, 0x8B, 0x9E, 0x74, 0xA8,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xC4, 0x25, 0x64, 0xF8, 0x92, 0xA6, 0x4D, 0xF1, 0xCB, 0x15, 0x71, 0x2B, 0x73, 0x6E, 0x33, 0x74,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xF1, 0xBB, 0xB6, 0x81, 0x93, 0x71, 0x60, 0x2D, 0x89, 0x70, 0xE9, 0x7B, 0x90, 0x07, 0x93, 0xC7,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xC2, 0xA8, 0x9A, 0x4A, 0x16, 0x49, 0xA5, 0x9B, 0xE6, 0x80, 0x57, 0x4D, 0xD0, 0xB6, 0x01, 0x45},
                1,
                {0x03},
                0x14,
                {0x5A, 0xDD, 0xF2, 0x1D, 0x09, 0x27, 0x86, 0x61,
                                                                 0x14, 0x11, 0x79, 0xCB, 0xEF, 0xF2, 0x72, 0xEA,
                        0x38, 0x4B, 0x13, 0xBB},
                {0x20, 0x16, 0x12, 0x31}},
//��Ӧ�ṹCAPK_STRUCT,MASTER��2�鹫Կ
        {{0xA0, 0x00, 0x00, 0x00, 0x04},
                0x04,
                0x01,
                0x01,
                144,
                {0xA6, 0xDA, 0x42, 0x83, 0x87, 0xA5, 0x02, 0xD7, 0xDD, 0xFB, 0x7A, 0x74, 0xD3, 0xF4, 0x12, 0xBE,
                                                                                                                 0x76, 0x26, 0x27, 0x19, 0x7B, 0x25, 0x43, 0x5B, 0x7A, 0x81, 0x71, 0x6A, 0x70, 0x01, 0x57, 0xDD,
                                                                                                                                                                                                                 0xD0, 0x6F, 0x7C, 0xC9, 0x9D, 0x6C, 0xA2, 0x8C, 0x24, 0x70, 0x52, 0x7E, 0x2C, 0x03, 0x61, 0x6B,
                                                                                                                                                                                                                                                                                                                 0x9C, 0x59, 0x21, 0x73, 0x57, 0xC2, 0x67, 0x4F, 0x58, 0x3B, 0x3B, 0xA5, 0xC7, 0xDC, 0xF2, 0x83,
                                                                                                                                                                                                                                                                                                                                                                                                                 0x86, 0x92, 0xD0, 0x23, 0xE3, 0x56, 0x24, 0x20, 0xB4, 0x61, 0x5C, 0x43, 0x9C, 0xA9, 0x7C, 0x44,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xDC, 0x9A, 0x24, 0x9C, 0xFC, 0xE7, 0xB3, 0xBF, 0xB2, 0x2F, 0x68, 0x22, 0x8C, 0x3A, 0xF1, 0x33,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x29, 0xAA, 0x4A, 0x61, 0x3C, 0xF8, 0xDD, 0x85, 0x35, 0x02, 0x37, 0x3D, 0x62, 0xE4, 0x9A, 0xB2,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x56, 0xD2, 0xBC, 0x17, 0x12, 0x0E, 0x54, 0xAE, 0xDC, 0xED, 0x6D, 0x96, 0xA4, 0x28, 0x7A, 0xCC,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x5C, 0x04, 0x67, 0x7D, 0x4A, 0x5A, 0x32, 0x0D, 0xB8, 0xBE, 0xE2, 0xF7, 0x75, 0xE5, 0xFE, 0xC5},
                1,
                {0x03},
                0x14,
                {0x38, 0x1A, 0x03, 0x5D, 0xA5, 0x8B, 0x48, 0x2E,
                                                                 0xE2, 0xAF, 0x75, 0xF4, 0xC3, 0xF2, 0xCA, 0x46,
                        0x9B, 0xA4, 0xAA, 0x6C},
                {0x20, 0x17, 0x12, 0x31}},

//��Ӧ�ṹCAPK_STRUCT,MASTER��3�鹫Կ
        {{0xA0, 0x00, 0x00, 0x00, 0x04},
                0x05,
                0x01,
                0x01,
                176,
                {0xB8, 0x04, 0x8A, 0xBC, 0x30, 0xC9, 0x0D, 0x97, 0x63, 0x36, 0x54, 0x3E, 0x3F, 0xD7, 0x09, 0x1C,
                                                                                                                 0x8F, 0xE4, 0x80, 0x0D, 0xF8, 0x20, 0xED, 0x55, 0xE7, 0xE9, 0x48, 0x13, 0xED, 0x00, 0x55, 0x5B,
                                                                                                                                                                                                                 0x57, 0x3F, 0xEC, 0xA3, 0xD8, 0x4A, 0xF6, 0x13, 0x1A, 0x65, 0x1D, 0x66, 0xCF, 0xF4, 0x28, 0x4F,
                                                                                                                                                                                                                                                                                                                 0xB1, 0x3B, 0x63, 0x5E, 0xDD, 0x0E, 0xE4, 0x01, 0x76, 0xD8, 0xBF, 0x04, 0xB7, 0xFD, 0x1C, 0x7B,
                                                                                                                                                                                                                                                                                                                                                                                                                 0xAC, 0xF9, 0xAC, 0x73, 0x27, 0xDF, 0xAA, 0x8A, 0xA7, 0x2D, 0x10, 0xDB, 0x3B, 0x8E, 0x70, 0xB2,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xDD, 0xD8, 0x11, 0xCB, 0x41, 0x96, 0x52, 0x5E, 0xA3, 0x86, 0xAC, 0xC3, 0x3C, 0x0D, 0x9D, 0x45,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x75, 0x91, 0x64, 0x69, 0xC4, 0xE4, 0xF5, 0x3E, 0x8E, 0x1C, 0x91, 0x2C, 0xC6, 0x18, 0xCB, 0x22,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xDD, 0xE7, 0xC3, 0x56, 0x8E, 0x90, 0x02, 0x2E, 0x6B, 0xBA, 0x77, 0x02, 0x02, 0xE4, 0x52, 0x2A,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x2D, 0xD6, 0x23, 0xD1, 0x80, 0xE2, 0x15, 0xBD, 0x1D, 0x15, 0x07, 0xFE, 0x3D, 0xC9, 0x0C, 0xA3,
                        0x10, 0xD2, 0x7B, 0x3E, 0xFC, 0xCD, 0x8F, 0x83, 0xDE, 0x30, 0x52, 0xCA, 0xD1, 0xE4, 0x89, 0x38,
                        0xC6, 0x8D, 0x09, 0x5A, 0xAC, 0x91, 0xB5, 0xF3, 0x7E, 0x28, 0xBB, 0x49, 0xEC, 0x7E, 0xD5, 0x97},
                1,
                {0x03},
                0x14,
                {0xEB, 0xFA, 0x0D, 0x5D, 0x06, 0xD8, 0xCE, 0x70,
                                                                 0x2D, 0xA3, 0xEA, 0xE8, 0x90, 0x70, 0x1D, 0x45,
                        0xE2, 0x74, 0xC8, 0x45},
                {0x20, 0x20, 0x12, 0x31}},

//��Ӧ�ṹCAPK_STRUCT,MASTER��4�鹫Կ
        {{0xA0, 0x00, 0x00, 0x00, 0x04},
                0x06,
                0x01,
                0x01,
                248,
                {0xCB, 0x26, 0xFC, 0x83, 0x0B, 0x43, 0x78, 0x5B, 0x2B, 0xCE, 0x37, 0xC8, 0x1E, 0xD3, 0x34, 0x62,
                                                                                                                 0x2F, 0x96, 0x22, 0xF4, 0xC8, 0x9A, 0xAE, 0x64, 0x10, 0x46, 0xB2, 0x35, 0x34, 0x33, 0x88, 0x3F,
                                                                                                                                                                                                                 0x30, 0x7F, 0xB7, 0xC9, 0x74, 0x16, 0x2D, 0xA7, 0x2F, 0x7A, 0x4E, 0xC7, 0x5D, 0x9D, 0x65, 0x73,
                                                                                                                                                                                                                                                                                                                 0x36, 0x86, 0x5B, 0x8D, 0x30, 0x23, 0xD3, 0xD6, 0x45, 0x66, 0x76, 0x25, 0xC9, 0xA0, 0x7A, 0x6B,
                                                                                                                                                                                                                                                                                                                                                                                                                 0x7A, 0x13, 0x7C, 0xF0, 0xC6, 0x41, 0x98, 0xAE, 0x38, 0xFC, 0x23, 0x80, 0x06, 0xFB, 0x26, 0x03,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xF4, 0x1F, 0x4F, 0x3B, 0xB9, 0xDA, 0x13, 0x47, 0x27, 0x0F, 0x2F, 0x5D, 0x8C, 0x60, 0x6E, 0x42,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x09, 0x58, 0xC5, 0xF7, 0xD5, 0x0A, 0x71, 0xDE, 0x30, 0x14, 0x2F, 0x70, 0xDE, 0x46, 0x88, 0x89,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xB5, 0xE3, 0xA0, 0x86, 0x95, 0xB9, 0x38, 0xA5, 0x0F, 0xC9, 0x80, 0x39, 0x3A, 0x9C, 0xBC, 0xE4,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x4A, 0xD2, 0xD6, 0x4F, 0x63, 0x0B, 0xB3, 0x3A, 0xD3, 0xF5, 0xF5, 0xFD, 0x49, 0x5D, 0x31, 0xF3,
                        0x78, 0x18, 0xC1, 0xD9, 0x40, 0x71, 0x34, 0x2E, 0x07, 0xF1, 0xBE, 0xC2, 0x19, 0x4F, 0x60, 0x35,
                        0xBA, 0x5D, 0xED, 0x39, 0x36, 0x50, 0x0E, 0xB8, 0x2D, 0xFD, 0xA6, 0xE8, 0xAF, 0xB6, 0x55, 0xB1,
                        0xEF, 0x3D, 0x0D, 0x7E, 0xBF, 0x86, 0xB6, 0x6D, 0xD9, 0xF2, 0x9F, 0x6B, 0x1D, 0x32, 0x4F, 0xE8,
                        0xB2, 0x6C, 0xE3, 0x8A, 0xB2, 0x01, 0x3D, 0xD1, 0x3F, 0x61, 0x1E, 0x7A, 0x59, 0x4D, 0x67, 0x5C,
                        0x44, 0x32, 0x35, 0x0E, 0xA2, 0x44, 0xCC, 0x34, 0xF3, 0x87, 0x3C, 0xBA, 0x06, 0x59, 0x29, 0x87,
                        0xA1, 0xD7, 0xE8, 0x52, 0xAD, 0xC2, 0x2E, 0xF5, 0xA2, 0xEE, 0x28, 0x13, 0x20, 0x31, 0xE4, 0x8F,
                        0x74, 0x03, 0x7E, 0x3B, 0x34, 0xAB, 0x74, 0x7F},
                1,
                {0x03},
                0x14,
                {0xF9, 0x10, 0xA1, 0x50, 0x4D, 0x5F, 0xFB, 0x79,
                                                                 0x3D, 0x94, 0xF3, 0xB5, 0x00, 0x76, 0x5E, 0x1A,
                        0xBC, 0xAD, 0x72, 0xD9},
                {0x20, 0x20, 0x12, 0x31}},

//===========JCB CAPK��ʼ===========================================================
//��Ӧ�ṹCAPK_STRUCT,JCB��1�鹫Կ
        {{0xA0, 0x00, 0x00, 0x00, 0x65}, //RID
                0x09,            //CAPK����
                0x01,            //HASH�㷨��ʶ
                0x01,            //RSA�㷨��ʶ��Ҫ��HASH�㷨��ʶ��ͬ
                0x80,            //ģ����
                {0xB7, 0x2A, 0x8F, 0xEF, 0x5B, 0x27, 0xF2, 0xB5, 0x50, 0x39, 0x8F, 0xDC, 0xC2, 0x56, 0xF7, 0x14,
                                                                                                                 0xBA, 0xD4, 0x97, 0xFF, 0x56, 0x09, 0x4B, 0x74, 0x08, 0x32, 0x8C, 0xB6, 0x26, 0xAA, 0x6F, 0x0E,
                                                                                                                                                                                                                 0x6A, 0x9D, 0xF8, 0x38, 0x8E, 0xB9, 0x88, 0x7B, 0xC9, 0x30, 0x17, 0x0B, 0xCC, 0x12, 0x13, 0xE9,
                                                                                                                                                                                                                                                                                                                 0x0F, 0xC0, 0x70, 0xD5, 0x2C, 0x8D, 0xCD, 0x0F, 0xF9, 0xE1, 0x0F, 0xAD, 0x36, 0x80, 0x1F, 0xE9,
                                                                                                                                                                                                                                                                                                                                                                                                                 0x3F, 0xC9, 0x98, 0xA7, 0x21, 0x70, 0x50, 0x91, 0xF1, 0x8B, 0xC7, 0xC9, 0x82, 0x41, 0xCA, 0xDC,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x15, 0xA2, 0xB9, 0xDA, 0x7F, 0xB9, 0x63, 0x14, 0x2C, 0x0A, 0xB6, 0x40, 0xD5, 0xD0, 0x13, 0x5E,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x77, 0xEB, 0xAE, 0x95, 0xAF, 0x1B, 0x4F, 0xEF, 0xAD, 0xCF, 0x9C, 0x01, 0x23, 0x66, 0xBD, 0xDA,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x04, 0x55, 0xC1, 0x56, 0x4A, 0x68, 0x81, 0x0D, 0x71, 0x27, 0x67, 0x6D, 0x49, 0x38, 0x90, 0xBD},
                0x01, //ָ������
                {0x03}, //ָ��
                0x14, //CHECKSUM ����
                {0x44, 0x10, 0xC6, 0xD5, 0x1C, 0x2F, 0x83, 0xAD, 0xFD, 0x92, 0x52, 0x8F, 0xA6, 0xE3, 0x8A, 0x32,
                        0xDF, 0x04, 0x8D, 0x0A}, //CHECKSUM
                {0x20, 0x12, 0x12, 0x31}, //��Ч��
        },
//��Ӧ�ṹCAPK_STRUCT,JCB��2�鹫Կ
        {{0xA0, 0x00, 0x00, 0x00, 0x65}, //RID
                0x10,            //CAPK����
                0x01,            //HASH�㷨��ʶ
                0x01,            //RSA�㷨��ʶ��Ҫ��HASH�㷨��ʶ��ͬ
                0x90,            //ģ����
                {0x99, 0xB6, 0x34, 0x64, 0xEE, 0x0B, 0x49, 0x57, 0xE4, 0xFD, 0x23, 0xBF, 0x92, 0x3D, 0x12, 0xB6,
                                                                                                                 0x14, 0x69, 0xB8, 0xFF, 0xF8, 0x81, 0x43, 0x46, 0xB2, 0xED, 0x6A, 0x78, 0x0F, 0x89, 0x88, 0xEA,
                                                                                                                                                                                                                 0x9C, 0xF0, 0x43, 0x3B, 0xC1, 0xE6, 0x55, 0xF0, 0x5E, 0xFA, 0x66, 0xD0, 0xC9, 0x80, 0x98, 0xF2,
                                                                                                                                                                                                                                                                                                                 0x5B, 0x65, 0x9D, 0x7A, 0x25, 0xB8, 0x47, 0x8A, 0x36, 0xE4, 0x89, 0x76, 0x0D, 0x07, 0x1F, 0x54,
                                                                                                                                                                                                                                                                                                                                                                                                                 0xCD, 0xF7, 0x41, 0x69, 0x48, 0xED, 0x73, 0x3D, 0x81, 0x63, 0x49, 0xDA, 0x2A, 0xAD, 0xDA, 0x22,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x7E, 0xE4, 0x59, 0x36, 0x20, 0x3C, 0xBF, 0x62, 0x8C, 0xD0, 0x33, 0xAA, 0xBA, 0x5E, 0x5A, 0x6E,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x4A, 0xE3, 0x7F, 0xBA, 0xCB, 0x46, 0x11, 0xB4, 0x11, 0x3E, 0xD4, 0x27, 0x52, 0x9C, 0x63, 0x6F,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x6C, 0x33, 0x04, 0xF8, 0xAB, 0xDD, 0x6D, 0x9A, 0xD6, 0x60, 0x51, 0x6A, 0xE8, 0x7F, 0x7F, 0x2D,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xDF, 0x1D, 0x2F, 0xA4, 0x4C, 0x16, 0x47, 0x27, 0xE5, 0x6B, 0xBC, 0x9B, 0xA2, 0x3C, 0x02, 0x85},
                0x01, //ָ������
                {0x03}, //ָ��
                0x14, //CHECKSUM ����
                {0xC7, 0x5E, 0x52, 0x10, 0xCB, 0xE6, 0xE8, 0xF0, 0x59, 0x4A, 0x0F, 0x19, 0x11, 0xB0, 0x74, 0x18,
                        0xCA, 0xDB, 0x5B, 0xAB}, //CHECKSUM
                {0x20, 0x17, 0x12, 0x31}, //��Ч��
        },
//��Ӧ�ṹCAPK_STRUCT,JCB��3�鹫Կ
        {{0xA0, 0x00, 0x00, 0x00, 0x65}, //RID
                0x12,            //CAPK����
                0x01,            //HASH�㷨��ʶ
                0x01,            //RSA�㷨��ʶ��Ҫ��HASH�㷨��ʶ��ͬ
                0xB0,            //ģ����
                {0xAD, 0xF0, 0x5C, 0xD4, 0xC5, 0xB4, 0x90, 0xB0, 0x87, 0xC3, 0x46, 0x7B, 0x0F, 0x30, 0x43, 0x75,
                                                                                                                 0x04, 0x38, 0x84, 0x84, 0x61, 0x28, 0x8B, 0xFE, 0xFD, 0x61, 0x98, 0xDD, 0x57, 0x6D, 0xC3, 0xAD,
                                                                                                                                                                                                                 0x7A, 0x7C, 0xFA, 0x07, 0xDB, 0xA1, 0x28, 0xC2, 0x47, 0xA8, 0xEA, 0xB3, 0x0D, 0xC3, 0xA3, 0x0B,
                                                                                                                                                                                                                                                                                                                 0x02, 0xFC, 0xD7, 0xF1, 0xC8, 0x16, 0x79, 0x65, 0x46, 0x36, 0x26, 0xFE, 0xFF, 0x8A, 0xB1, 0xAA,
                                                                                                                                                                                                                                                                                                                                                                                                                 0x61, 0xA4, 0xB9, 0xAE, 0xF0, 0x9E, 0xE1, 0x2B, 0x00, 0x98, 0x42, 0xA1, 0xAB, 0xA0, 0x1A, 0xDB,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x4A, 0x2B, 0x17, 0x06, 0x68, 0x78, 0x1E, 0xC9, 0x2B, 0x60, 0xF6, 0x05, 0xFD, 0x12, 0xB2, 0xB2,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xA6, 0xF1, 0xFE, 0x73, 0x4B, 0xE5, 0x10, 0xF6, 0x0D, 0xC5, 0xD1, 0x89, 0xE4, 0x01, 0x45, 0x1B,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x62, 0xB4, 0xE0, 0x68, 0x51, 0xEC, 0x20, 0xEB, 0xFF, 0x45, 0x22, 0xAA, 0xCC, 0x2E, 0x9C, 0xDC,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x89, 0xBC, 0x5D, 0x8C, 0xDE, 0x5D, 0x63, 0x3C, 0xFD, 0x77, 0x22, 0x0F, 0xF6, 0xBB, 0xD4, 0xA9,
                        0xB4, 0x41, 0x47, 0x3C, 0xC3, 0xC6, 0xFE, 0xFC, 0x8D, 0x13, 0xE5, 0x7C, 0x3D, 0xE9, 0x7E, 0x12,
                        0x69, 0xFA, 0x19, 0xF6, 0x55, 0x21, 0x5B, 0x23, 0x56, 0x3E, 0xD1, 0xD1, 0x86, 0x0D, 0x86, 0x81},
                0x01, //ָ������
                {0x03}, //ָ��
                0x14, //CHECKSUM ����
                {0x87, 0x4B, 0x37, 0x9B, 0x7F, 0x60, 0x7D, 0xC1, 0xCA, 0xF8, 0x7A, 0x19, 0xE4, 0x00, 0xB6, 0xA9,
                        0xE2, 0x51, 0x63, 0xE8}, //CHECKSUM
                {0x20, 0x20, 0x12, 0x31}, //��Ч��
        },
//��Ӧ�ṹCAPK_STRUCT,JCB��4�鹫Կ
        {{0xA0, 0x00, 0x00, 0x00, 0x65}, //RID
                0x14,            //CAPK����
                0x01,            //HASH�㷨��ʶ
                0x01,            //RSA�㷨��ʶ��Ҫ��HASH�㷨��ʶ��ͬ
                0xF8,            //ģ����
                {0xAE, 0xED, 0x55, 0xB9, 0xEE, 0x00, 0xE1, 0xEC, 0xEB, 0x04, 0x5F, 0x61, 0xD2, 0xDA, 0x9A, 0x66,
                                                                                                                 0xAB, 0x63, 0x7B, 0x43, 0xFB, 0x5C, 0xDB, 0xDB, 0x22, 0xA2, 0xFB, 0xB2, 0x5B, 0xE0, 0x61, 0xE9,
                                                                                                                                                                                                                 0x37, 0xE3, 0x82, 0x44, 0xEE, 0x51, 0x32, 0xF5, 0x30, 0x14, 0x4A, 0x3F, 0x26, 0x89, 0x07, 0xD8,
                                                                                                                                                                                                                                                                                                                 0xFD, 0x64, 0x88, 0x63, 0xF5, 0xA9, 0x6F, 0xED, 0x7E, 0x42, 0x08, 0x9E, 0x93, 0x45, 0x7A, 0xDC,
                                                                                                                                                                                                                                                                                                                                                                                                                 0x0E, 0x1B, 0xC8, 0x9C, 0x58, 0xA0, 0xDB, 0x72, 0x67, 0x5F, 0xBC, 0x47, 0xFE, 0xE9, 0xFF, 0x33,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xC1, 0x6A, 0xDE, 0x6D, 0x34, 0x19, 0x36, 0xB0, 0x6B, 0x6A, 0x6F, 0x5E, 0xF6, 0xF6, 0x6A, 0x4E,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xDD, 0x98, 0x1D, 0xF7, 0x5D, 0xA8, 0x39, 0x9C, 0x30, 0x53, 0xF4, 0x30, 0xEC, 0xA3, 0x42, 0x43,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x7C, 0x23, 0xAF, 0x42, 0x3A, 0x21, 0x1A, 0xC9, 0xF5, 0x8E, 0xAF, 0x09, 0xB0, 0xF8, 0x37, 0xDE,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x9D, 0x86, 0xC7, 0x10, 0x9D, 0xB1, 0x64, 0x65, 0x61, 0xAA, 0x5A, 0xF0, 0x28, 0x9A, 0xF5, 0x51,
                        0x4A, 0xC6, 0x4B, 0xC2, 0xD9, 0xD3, 0x6A, 0x17, 0x9B, 0xB8, 0xA7, 0x97, 0x1E, 0x2B, 0xFA, 0x03,
                        0xA9, 0xE4, 0xB8, 0x47, 0xFD, 0x3D, 0x63, 0x52, 0x4D, 0x43, 0xA0, 0xE8, 0x00, 0x35, 0x47, 0xB9,
                        0x4A, 0x8A, 0x75, 0xE5, 0x19, 0xDF, 0x31, 0x77, 0xD0, 0xA6, 0x0B, 0xC0, 0xB4, 0xBA, 0xB1, 0xEA,
                        0x59, 0xA2, 0xCB, 0xB4, 0xD2, 0xD6, 0x23, 0x54, 0xE9, 0x26, 0xE9, 0xC7, 0xD3, 0xBE, 0x41, 0x81,
                        0xE8, 0x1B, 0xA6, 0x0F, 0x82, 0x85, 0xA8, 0x96, 0xD1, 0x7D, 0xA8, 0xC3, 0x24, 0x24, 0x81, 0xB6,
                        0xC4, 0x05, 0x76, 0x9A, 0x39, 0xD5, 0x47, 0xC7, 0x4E, 0xD9, 0xFF, 0x95, 0xA7, 0x0A, 0x79, 0x60,
                        0x46, 0xB5, 0xEF, 0xF3, 0x66, 0x82, 0xDC, 0x29},
                0x01, //ָ������
                {0x03}, //ָ��
                0x14, //CHECKSUM ����
                {0xC0, 0xD1, 0x5F, 0x6C, 0xD9, 0x57, 0xE4, 0x91, 0xDB, 0x56, 0xDC, 0xDD, 0x1C, 0xA8, 0x7A, 0x03,
                        0xEB, 0xE0, 0x6B, 0x7B}, //CHECKSUM
                {0x20, 0x20, 0x12, 0x31}, //��Ч��
        },
//��Ӧ�����ֽ���Թ�Կ huxiang add 100726
        {{0xA0, 0x00, 0x00, 0x03, 0x33}, //RID
                0x08,            //CAPK����
                0x01,            //HASH�㷨��ʶ
                0x01,            //RSA�㷨��ʶ��Ҫ��HASH�㷨��ʶ��ͬ
                0x90,            //ģ����
                {0xB6, 0x16, 0x45, 0xED, 0xFD, 0x54, 0x98, 0xFB, 0x24, 0x64, 0x44, 0x03, 0x7A, 0x0F, 0xA1,
                                                                                                           0x8C, 0x0F, 0x10, 0x1E, 0xBD, 0x8E, 0xFA, 0x54, 0x57, 0x3C, 0xE6, 0xE6, 0xA7, 0xFB, 0xF6,
                                                                                                                                                                                                     0x3E, 0xD2, 0x1D, 0x66, 0x34, 0x08, 0x52, 0xB0, 0x21, 0x1C, 0xF5, 0xEE, 0xF6, 0xA1, 0xCD,
                                                                                                                                                                                                                                                                                               0x98, 0x9F, 0x66, 0xAF, 0x21, 0xA8, 0xEB, 0x19, 0xDB, 0xD8, 0xDB, 0xC3, 0x70, 0x6D, 0x13,
                                                                                                                                                                                                                                                                                                                                                                                         0x53, 0x63, 0xA0, 0xD6, 0x83, 0xD0, 0x46, 0x30, 0x4F, 0x5A, 0x83, 0x6B, 0xC1, 0xBC, 0x63,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   0x28, 0x21, 0xAF, 0xE7, 0xA2, 0xF7, 0x5D, 0xA3, 0xC5, 0x0A, 0xC7, 0x4C, 0x54, 0x5A, 0x75,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             0x45, 0x62, 0x20, 0x41, 0x37, 0x16, 0x96, 0x63, 0xCF, 0xCC, 0x0B, 0x06, 0xE6, 0x7E, 0x21,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       0x09, 0xEB, 0xA4, 0x1B, 0xC6, 0x7F, 0xF2, 0x0C, 0xC8, 0xAC, 0x80, 0xD7, 0xB6, 0xEE, 0x1A,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x95, 0x46, 0x5B, 0x3B, 0x26, 0x57, 0x53, 0x3E, 0xA5, 0x6D, 0x92, 0xD5, 0x39, 0xE5, 0x06,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           0x43, 0x60, 0xEA, 0x48, 0x50, 0xFE, 0xD2, 0xD1, 0xBF},
                0x01, //ָ������
                {0x03}, //ָ��
                0x14, //CHECKSUM ����
                {0xEE, 0x23, 0xB6, 0x16, 0xC9, 0x5C, 0x02, 0x65, 0x2A, 0xD1, 0x88, 0x60, 0xE4, 0x87, 0x87, 0xC0,
                        0x79, 0xE8, 0xE8, 0x5A}, //CHECKSUM
                {0x20, 0x49, 0x12, 0x31}, //��Ч��
        },
//��Ӧ���˻ṫԿ huxiang add 101129
        {{0xA0, 0x00, 0x00, 0x03, 0x33}, //RID
                0x02,               //CAPK����
                0x01,               //HASH�㷨��ʶ
                0x01,               //RSA�㷨��ʶ��Ҫ��HASH�㷨��ʶ��ͬ
                0x90,              //ģ����
                {0xA3, 0x76, 0x7A, 0xBD, 0x1B, 0x6A, 0xA6, 0x9D, 0x7F, 0x3F, 0xBF, 0x28, 0xC0, 0x92, 0xDE, 0x9E,
                                                                                                                 0xD1, 0xE6, 0x58, 0xBA, 0x5F, 0x09, 0x09, 0xAF, 0x7A, 0x1C, 0XCD, 0x90, 0x73, 0x73, 0xB7, 0x21,
                                                                                                                                                                                                                 0x0F, 0xDE, 0xB1, 0x62, 0x87, 0xBA, 0x8E, 0x78, 0xE1, 0x52, 0x9F, 0x44, 0x39, 0x76, 0xFD, 0x27,
                                                                                                                                                                                                                                                                                                                 0xF9, 0x91, 0xEC, 0x67, 0xD9, 0x5E, 0x5F, 0x4E, 0x96, 0xB1, 0x27, 0xCA, 0xB2, 0x39, 0x6A, 0x94,
                                                                                                                                                                                                                                                                                                                                                                                                                 0xD6, 0xE4, 0x5C, 0xDA, 0x44, 0xCA, 0x4C, 0x48, 0x67, 0x57, 0x0D, 0x6B, 0x07, 0x54, 0x2F, 0x8D,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x4B, 0xF9, 0xFF, 0x97, 0x97, 0x5D, 0xB9, 0x89, 0x15, 0x15, 0xE6, 0x6F, 0x52, 0x5D, 0x2B, 0x3C,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xBE, 0xB6, 0xD6, 0x62, 0xBF, 0xB6, 0xC3, 0xF3, 0x38, 0xE9, 0x3B, 0x02, 0x14, 0x2B, 0xFC, 0x44,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x17, 0x3A, 0x37, 0x64, 0xC5, 0x6A, 0xAD, 0xD2, 0x02, 0x07, 0x5B, 0x26, 0xDC, 0x2F, 0x9F, 0x7D,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x7A, 0xE7, 0x4B, 0xD7, 0xD0, 0x0F, 0xD0, 0x5E, 0xE4, 0x30, 0x03, 0x26, 0x63, 0xD2, 0x7A, 0x57,},
                0x01, //ָ������
                {0x03}, //ָ��
                0x14,       //CHECKSUM ����
                {0x03, 0xBB, 0x33, 0x5A, 0x85, 0x49, 0xA0, 0x3B, 0x87, 0xAB, 0x08, 0x9D, 0x00, 0x6F, 0x60, 0x85,
                        0x2E, 0x4B, 0x80, 0x60}, //CHECKSUM
                {0x20, 0x20, 0x12, 0x31}, //��Ч��
        },


        {{0xA0, 0x00, 0x00, 0x03, 0x33}, //RID
                0x03,               //CAPK����
                0x01,               //HASH�㷨��ʶ
                0x01,               //RSA�㷨��ʶ��Ҫ��HASH�㷨��ʶ��ͬ
                0xB0,              //ģ����
                {0xB0, 0x62, 0x7D, 0xEE, 0x87, 0x86, 0x4F, 0x9C, 0x18, 0xC1, 0x3B, 0x9A, 0x1F, 0x02, 0x54, 0x48,
                                                                                                                 0xBF, 0x13, 0xC5, 0x83, 0x80, 0xC9, 0x1F, 0x4C, 0xEB, 0xA9, 0xF9, 0xBC, 0xB2, 0x14, 0xFF, 0x84,
                                                                                                                                                                                                                 0x14, 0xE9, 0xB5, 0x9D, 0x6A, 0xBA, 0x10, 0xF9, 0x41, 0xC7, 0x33, 0x17, 0x68, 0xF4, 0x7B, 0x21,
                                                                                                                                                                                                                                                                                                                 0x27, 0x90, 0x7D, 0x85, 0x7F, 0xA3, 0x9A, 0xAF, 0x8C, 0xE0, 0x20, 0x45, 0xDD, 0x01, 0x61, 0x9D,
                                                                                                                                                                                                                                                                                                                                                                                                                 0x68, 0x9E, 0xE7, 0x31, 0xC5, 0x51, 0x15, 0x9B, 0xE7, 0xEB, 0x2D, 0x51, 0xA3, 0x72, 0xFF, 0x56,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xB5, 0x56, 0xE5, 0xCB, 0x2F, 0xDE, 0x36, 0xE2, 0x30, 0x73, 0xA4, 0x4C, 0xA2, 0x15, 0xD6, 0xC2,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x6C, 0xA6, 0x88, 0x47, 0xB3, 0x88, 0xE3, 0x95, 0x20, 0xE0, 0x02, 0x6E, 0x62, 0x29, 0x4B, 0x55,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x7D, 0x64, 0x70, 0x44, 0x0C, 0xA0, 0xAE, 0xFC, 0x94, 0x38, 0xC9, 0x23, 0xAE, 0xC9, 0xB2, 0x09,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x8D, 0x6D, 0x3A, 0x1A, 0xF5, 0xE8, 0xB1, 0xDE, 0x36, 0xF4, 0xB5, 0x30, 0x40, 0x10, 0x9D, 0x89,
                        0xB7, 0x7C, 0xAF, 0xAF, 0x70, 0xC2, 0x6C, 0x60, 0x1A, 0xBD, 0xF5, 0x9E, 0xEC, 0x0F, 0xDC, 0x8A,
                        0x99, 0x08, 0x91, 0x40, 0xCD, 0x2E, 0x81, 0x7E, 0x33, 0x51, 0x75, 0xB0, 0x3B, 0x7A, 0xA3, 0x3D},
                0x01, //ָ������
                {0x03}, //ָ��
                0x14,       //CHECKSUM ����
                {0x87, 0xF0, 0xCD, 0x7C, 0x0E, 0x86, 0xF3, 0x8F, 0x89, 0xA6, 0x6F, 0x8C, 0x47, 0x07, 0x1A, 0x8B,
                        0x88, 0x58, 0x6F, 0x26}, //CHECKSUM
                {0x20, 0x20, 0x12, 0x31}, //��Ч��
        },


        {{0xA0, 0x00, 0x00, 0x03, 0x33}, //RID
                0x04,               //CAPK����
                0x01,               //HASH�㷨��ʶ
                0x01,               //RSA�㷨��ʶ��Ҫ��HASH�㷨��ʶ��ͬ
                0xF8,              //ģ����
                {0xBC, 0x85, 0x3E, 0x6B, 0x53, 0x65, 0xE8, 0x9E, 0x7E, 0xE9, 0x31, 0x7C, 0x94, 0xB0, 0x2D, 0x0A,
                                                                                                                 0xBB, 0x0D, 0xBD, 0x91, 0xC0, 0x5A, 0x22, 0x4A, 0x25, 0x54, 0xAA, 0x29, 0xED, 0x9F, 0xCB, 0x9D,
                                                                                                                                                                                                                 0x86, 0xEB, 0x9C, 0xCB, 0xB3, 0x22, 0xA5, 0x78, 0x11, 0xF8, 0x61, 0x88, 0xAA, 0xC7, 0x35, 0x1C,
                                                                                                                                                                                                                                                                                                                 0x72, 0xBD, 0x9E, 0xF1, 0x96, 0xC5, 0xA0, 0x1A, 0xCE, 0xF7, 0xA4, 0xEB, 0x0D, 0x2A, 0xD6, 0x3D,
                                                                                                                                                                                                                                                                                                                                                                                                                 0x9E, 0x6A, 0xC2, 0xE7, 0x83, 0x65, 0x47, 0xCB, 0x15, 0x95, 0xC6, 0x8B, 0xCB, 0xAF, 0xD0, 0xF6,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x72, 0x87, 0x60, 0xF3, 0xA7, 0xCA, 0x7B, 0x97, 0x30, 0x1B, 0x7E, 0x02, 0x20, 0x18, 0x4E, 0xFC,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x4F, 0x65, 0x30, 0x08, 0xD9, 0x3C, 0xE0, 0x98, 0xC0, 0xD9, 0x3B, 0x45, 0x20, 0x10, 0x96, 0xD1,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xAD, 0xFF, 0x4C, 0xF1, 0xF9, 0xFC, 0x02, 0xAF, 0x75, 0x9D, 0xA2, 0x7C, 0xD6, 0xDF, 0xD6, 0xD7,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x89, 0xB0, 0x99, 0xF1, 0x6F, 0x37, 0x8B, 0x61, 0x00, 0x33, 0x4E, 0x63, 0xF3, 0xD3, 0x5F, 0x32,
                        0x51, 0xA5, 0xEC, 0x78, 0x69, 0x37, 0x31, 0xF5, 0x23, 0x35, 0x19, 0xCD, 0xB3, 0x80, 0xF5, 0xAB,
                        0x8C, 0x0F, 0x02, 0x72, 0x8E, 0x91, 0xD4, 0x69, 0xAB, 0xD0, 0xEA, 0xE0, 0xD9, 0x3B, 0x1C, 0xC6,
                        0x6C, 0xE1, 0x27, 0xB2, 0x9C, 0x7D, 0x77, 0x44, 0x1A, 0x49, 0xD0, 0x9F, 0xCA, 0x5D, 0x6D, 0x97,
                        0x62, 0xFC, 0x74, 0xC3, 0x1B, 0xB5, 0x06, 0xC8, 0xBA, 0xE3, 0xC7, 0x9A, 0xD6, 0xC2, 0x57, 0x87,
                        0x75, 0xB9, 0x59, 0x56, 0xB5, 0x37, 0x0D, 0x1D, 0x05, 0x19, 0xE3, 0x79, 0x06, 0xB3, 0x84, 0x73,
                        0x62, 0x33, 0x25, 0x1E, 0x8F, 0x09, 0xAD, 0x79, 0xDF, 0xBE, 0x2C, 0x6A, 0xBF, 0xAD, 0xAC, 0x8E,
                        0x4D, 0x86, 0x24, 0x31, 0x8C, 0x27, 0xDA, 0xF1},
                0x01, //ָ������
                {0x03}, //ָ��
                0x14,       //CHECKSUM ����
                {0xF5, 0x27, 0x08, 0x1C, 0xF3, 0x71, 0xDD, 0x7E, 0x1F, 0xD4, 0xFA, 0x41, 0x4A, 0x66, 0x50, 0x36,
                        0xE0, 0xF5, 0xE6, 0xE5}, //CHECKSUM
                {0x20, 0x20, 0x12, 0x31}, //��Ч��
        },


        //���Ӳ��Թ�Կ
        //PBOC++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        {{0xA0, 0x00, 0x00, 0x03, 0x33},  //RID
                0x14,                      //CAPK����
                0x01,                      //HASH�㷨��ʶ
                0x01,                      //RSA�㷨��ʶ��Ҫ��HASH�㷨��ʶ��ͬ
                0x90,                      //ģ����
                {0xC8, 0xBB, 0x08, 0x2B, 0xB6, 0xAC, 0x8D, 0x37, 0x40, 0x3F, 0x26, 0xE9, 0xFE, 0xE8, 0x8A, 0xEC,
                                                                                                                 0xB2, 0x84, 0x6E, 0x2F, 0xBD, 0x0F, 0x5E, 0xC6, 0x23, 0xD3, 0x77, 0x22, 0xC7, 0xA0, 0x31, 0x43,
                                                                                                                                                                                                                 0xC6, 0xF0, 0xDF, 0x25, 0xBC, 0xD9, 0xC6, 0x00, 0x3A, 0x93, 0xCD, 0x97, 0xBD, 0xFA, 0x77, 0x46,
                                                                                                                                                                                                                                                                                                                 0x14, 0x27, 0xA6, 0xE4, 0x93, 0xEF, 0x2D, 0x2E, 0x75, 0x57, 0x1B, 0x53, 0xD0, 0xC0, 0x03, 0xB1,
                                                                                                                                                                                                                                                                                                                                                                                                                 0xF4, 0x3A, 0xA9, 0xEC, 0xFB, 0xBB, 0x97, 0x56, 0xD8, 0x34, 0x58, 0x0B, 0xAA, 0x9F, 0xBF, 0x92,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x6A, 0x89, 0x43, 0x64, 0xA5, 0x31, 0xEF, 0x45, 0x01, 0xDA, 0xFF, 0xB2, 0x91, 0x5E, 0xF4, 0x78,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xE0, 0xF0, 0x80, 0x87, 0xA0, 0x45, 0x10, 0xCB, 0x67, 0x79, 0x87, 0x7D, 0x74, 0x8A, 0xEC, 0x25,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x60, 0xC5, 0x72, 0x20, 0xF4, 0x8D, 0xC6, 0x45, 0x71, 0x60, 0x0E, 0x88, 0x00, 0x02, 0x33, 0x1D,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xDF, 0x80, 0xC5, 0x10, 0xFE, 0xF4, 0xDF, 0xA0, 0x65, 0x44, 0x7B, 0xA9, 0xFA, 0xD4, 0xCE, 0x0F,
                },
                0x01,     //ָ������
                {0x03},  //ָ��
                0x14,     //CHECKSUM ����
                {0xD9, 0x11, 0x19, 0xE9, 0x07, 0xE1, 0x97, 0xD4,
                                                                 0xAC, 0x08, 0x91, 0xC8, 0x85, 0x0B, 0xD8, 0x73,
                        0x55, 0xC3, 0xE8, 0x82}, //CHECKSUM
                {0x20, 0x49, 0x12, 0x31}, //��Ч��
        },

        {{0xA0, 0x00, 0x00, 0x03, 0x33},  //RID
                0x09,                      //CAPK����
                0x01,                      //HASH�㷨��ʶ
                0x01,                      //RSA�㷨��ʶ��Ҫ��HASH�㷨��ʶ��ͬ
                0xB0,                      //ģ����
                {0xEB, 0x37, 0x4D, 0xFC, 0x5A, 0x96, 0xB7, 0x1D, 0x28, 0x63, 0x87, 0x5E, 0xDA, 0x2E, 0xAF, 0xB9,
                                                                                                                 0x6B, 0x1B, 0x43, 0x9D, 0x3E, 0xCE, 0x0B, 0x18, 0x26, 0xA2, 0x67, 0x2E, 0xEE, 0xFA, 0x79, 0x90,
                                                                                                                                                                                                                 0x28, 0x67, 0x76, 0xF8, 0xBD, 0x98, 0x9A, 0x15, 0x14, 0x1A, 0x75, 0xC3, 0x84, 0xDF, 0xC1, 0x4F,
                                                                                                                                                                                                                                                                                                                 0xEF, 0x92, 0x43, 0xAA, 0xB3, 0x27, 0x07, 0x65, 0x9B, 0xE9, 0xE4, 0x79, 0x7A, 0x24, 0x7C, 0x2F,
                                                                                                                                                                                                                                                                                                                                                                                                                 0x0B, 0x6D, 0x99, 0x37, 0x2F, 0x38, 0x4A, 0xF6, 0x2F, 0xE2, 0x3B, 0xC5, 0x4B, 0xCD, 0xC5, 0x7A,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x9A, 0xCD, 0x1D, 0x55, 0x85, 0xC3, 0x03, 0xF2, 0x01, 0xEF, 0x4E, 0x8B, 0x80, 0x6A, 0xFB, 0x80,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x9D, 0xB1, 0xA3, 0xDB, 0x1C, 0xD1, 0x12, 0xAC, 0x88, 0x4F, 0x16, 0x4A, 0x67, 0xB9, 0x9C, 0x7D,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x6E, 0x5A, 0x8A, 0x6D, 0xF1, 0xD3, 0xCA, 0xE6, 0xD7, 0xED, 0x3D, 0x5B, 0xE7, 0x25, 0xB2, 0xDE,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x4A, 0xDE, 0x23, 0xFA, 0x67, 0x9B, 0xF4, 0xEB, 0x15, 0xA9, 0x3D, 0x8A, 0x6E, 0x29, 0xC7, 0xFF,
                        0xA1, 0xA7, 0x0D, 0xE2, 0xE5, 0x4F, 0x59, 0x3D, 0x90, 0x8A, 0x3B, 0xF9, 0xEB, 0xBD, 0x76, 0x0B,
                        0xBF, 0xDC, 0x8D, 0xB8, 0xB5, 0x44, 0x97, 0xE6, 0xC5, 0xBE, 0x0E, 0x4A, 0x4D, 0xAC, 0x29, 0xE5
                },
                0x01,     //ָ������
                {0x03},  //ָ��
                0x14,     //CHECKSUM ����
                {0xA0, 0x75, 0x30, 0x6E, 0xAB, 0x00, 0x45, 0xBA, 0xF7, 0x2C, 0xDD, 0x33, 0xB3, 0xB6, 0x78, 0x77,
                        0x9D, 0xE1, 0xF5, 0x27}, //CHECKSUM
                {0x20, 0x49, 0x12, 0x31}, //��Ч��
        },

        {{0xA0, 0x00, 0x00, 0x03, 0x33},  //RID
                0x0B,                      //CAPK����
                0x01,                      //HASH�㷨��ʶ
                0x01,                      //RSA�㷨��ʶ��Ҫ��HASH�㷨��ʶ��ͬ
                0xF8,                      //ģ����
                {0xCF, 0x9F, 0xDF, 0x46, 0xB3, 0x56, 0x37, 0x8E, 0x9A, 0xF3, 0x11, 0xB0, 0xF9, 0x81, 0xB2, 0x1A,
                                                                                                                 0x1F, 0x22, 0xF2, 0x50, 0xFB, 0x11, 0xF5, 0x5C, 0x95, 0x87, 0x09, 0xE3, 0xC7, 0x24, 0x19, 0x18,
                                                                                                                                                                                                                 0x29, 0x34, 0x83, 0x28, 0x9E, 0xAE, 0x68, 0x8A, 0x09, 0x4C, 0x02, 0xC3, 0x44, 0xE2, 0x99, 0x9F,
                                                                                                                                                                                                                                                                                                                 0x31, 0x5A, 0x72, 0x84, 0x1F, 0x48, 0x9E, 0x24, 0xB1, 0xBA, 0x00, 0x56, 0xCF, 0xAB, 0x3B, 0x47,
                                                                                                                                                                                                                                                                                                                                                                                                                 0x9D, 0x0E, 0x82, 0x64, 0x52, 0x37, 0x5D, 0xCD, 0xBB, 0x67, 0xE9, 0x7E, 0xC2, 0xAA, 0x66, 0xF4,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x60, 0x1D, 0x77, 0x4F, 0xEA, 0xEF, 0x77, 0x5A, 0xCC, 0xC6, 0x21, 0xBF, 0xEB, 0x65, 0xFB, 0x00,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x53, 0xFC, 0x5F, 0x39, 0x2A, 0xA5, 0xE1, 0xD4, 0xC4, 0x1A, 0x4D, 0xE9, 0xFF, 0xDF, 0xDF, 0x13,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0x27, 0xC4, 0xBB, 0x87, 0x4F, 0x1F, 0x63, 0xA5, 0x99, 0xEE, 0x39, 0x02, 0xFE, 0x95, 0xE7, 0x29,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 0xFD, 0x78, 0xD4, 0x23, 0x4D, 0xC7, 0xE6, 0xCF, 0x1A, 0xBA, 0xBA, 0xA3, 0xF6, 0xDB, 0x29, 0xB7,
                        0xF0, 0x5D, 0x1D, 0x90, 0x1D, 0x2E, 0x76, 0xA6, 0x06, 0xA8, 0xCB, 0xFF, 0xFF, 0xEC, 0xBD, 0x91,
                        0x8F, 0xA2, 0xD2, 0x78, 0xBD, 0xB4, 0x3B, 0x04, 0x34, 0xF5, 0xD4, 0x51, 0x34, 0xBE, 0x1C, 0x27,
                        0x81, 0xD1, 0x57, 0xD5, 0x01, 0xFF, 0x43, 0xE5, 0xF1, 0xC4, 0x70, 0x96, 0x7C, 0xD5, 0x7C, 0xE5,
                        0x3B, 0x64, 0xD8, 0x29, 0x74, 0xC8, 0x27, 0x59, 0x37, 0xC5, 0xD8, 0x50, 0x2A, 0x12, 0x52, 0xA8,
                        0xA5, 0xD6, 0x08, 0x8A, 0x25, 0x9B, 0x69, 0x4F, 0x98, 0x64, 0x8D, 0x9A, 0xF2, 0xCB, 0x0E, 0xFD,
                        0x9D, 0x94, 0x3C, 0x69, 0xF8, 0x96, 0xD4, 0x9F, 0xA3, 0x97, 0x02, 0x16, 0x2A, 0xCB, 0x5A, 0xF2,
                        0x9B, 0x90, 0xBA, 0xDE, 0x00, 0x5B, 0xC1, 0x57
                },
                0x01,     //ָ������
                {0x03},  //ָ��
                0x14,     //CHECKSUM ����
                {0xBD, 0x33, 0x1F, 0x99, 0x96, 0xA4, 0x90, 0xB3,
                                                                 0x3C, 0x13, 0x44, 0x10, 0x66, 0xA0, 0x9A, 0xD3,
                        0xFE, 0xB5, 0xF6, 0x6C
                }, //CHECKSUM
                {0x20, 0x49, 0x12, 0x31}, //��Ч��
        },

};

void InitMposAid() {
    s32 num = -1;

    sdkIccGetAIDListNum(&num);
    if (num <= 0) {

        sdkIccAddAnyAIDList(CMBAid, sizeof(CMBAid) / sizeof(SDK_ICC_EmvSysIni_STRUCT));
    }
}

void InitMposCapk() {
    s32 num = -1;

    sdkIccGetCAPKListNum(&num);

    if (num <= 0) {

        sdkIccAddAnyCAPKList(CMBCapk, sizeof(CMBCapk) / sizeof(SDK_ICC_CAPK_STRUCT));
    }
}


