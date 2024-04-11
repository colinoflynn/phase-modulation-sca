// The number of shares used for masking 
#define NUM_SHARES 2



void Encrypt(uint8_t* output, uint8_t* input, uint8_t* key);
void MaskArray(uint8_t y[][NUM_SHARES], uint8_t x[], uint8_t length);
void UnMaskArray(uint8_t y[], uint8_t x[][NUM_SHARES], uint8_t length);
void CombinedSbox(uint8_t s[16][NUM_SHARES]);
void DualSbox(uint8_t y1[], uint8_t y2[], uint8_t x1[], uint8_t x2[]);
void ShiftRowsMixColumns(uint8_t s[][NUM_SHARES], uint8_t round);
void SecEvalCombined(uint8_t w[16][NUM_SHARES], uint8_t z[16][NUM_SHARES], const uint8_t h[]);
void SecMultCombined(uint8_t c[][NUM_SHARES], uint8_t a[][NUM_SHARES], uint8_t b[][NUM_SHARES], const uint8_t f[]);
void SecTwoMultSmall(uint8_t c0[], uint8_t c1[], uint8_t aa0[], uint8_t aa1[], uint8_t bb0[], uint8_t bb1[], const uint8_t f[]);
void SecEvalTwoQuadraticRand(uint8_t y0[], uint8_t y1[], uint8_t aa0[], uint8_t aa1[], const uint8_t h[]);



static const uint8_t fifth[256] = {
    0x00, 0x01, 0x20, 0x33, 0x6c, 0x72, 0x3a, 0x36, 0x2f, 0x8d, 0xc2, 0x72, 0x01, 0xbc, 0x9a, 0x35, 
    0x97, 0xd8, 0x10, 0x4d, 0x33, 0x63, 0xc2, 0x80, 0x20, 0xcc, 0x6a, 0x94, 0xc6, 0x35, 0xfa, 0x1b, 
    0x7d, 0xcb, 0x5e, 0xfa, 0x36, 0x9f, 0x63, 0xd8, 0x3a, 0x2f, 0xd4, 0xd3, 0x33, 0x39, 0xab, 0xb3,
    0x6c, 0x94, 0xe8, 0x02, 0xef, 0x08, 0x1d, 0xe8, 0xb3, 0xe8, 0xfa, 0xb3, 0x72, 0x36, 0x4d, 0x1b,
    0x39, 0xcb, 0x08, 0xe8, 0x35, 0xd8, 0x72, 0x8d, 0x9a, 0xcb, 0x66, 0x25, 0xd4, 0x9a, 0x5e, 0x02,
    0x01, 0xbd, 0x97, 0x39, 0xc5, 0x66, 0x25, 0x94, 0x3a, 0x25, 0x61, 0x6c, 0xbc, 0xbc, 0x91, 0x83,
    0x2f, 0x6a, 0x1d, 0x4a, 0x04, 0x5e, 0x40, 0x08, 0xe4, 0x02, 0x1b, 0xef, 0x8d, 0x74, 0x04, 0xef,
    0x91, 0x9a, 0x04, 0x1d, 0x72, 0x66, 0x91, 0x97, 0xc2, 0x6a, 0x9a, 0x20, 0x63, 0xd4, 0x4d, 0xe8,
    0x61, 0x25, 0x08, 0x5e, 0x1b, 0x40, 0x04, 0x4d, 0xfa, 0x1d, 0x5e, 0xab, 0xc2, 0x3a, 0x10, 0xfa,
    0xc6, 0xcc, 0x08, 0x10, 0x74, 0x61, 0xcc, 0xcb, 0xc5, 0x6c, 0xc6, 0x7d, 0x35, 0x83, 0x40, 0xe4,
    0x20, 0xd3, 0x4a, 0xab, 0x7d, 0x91, 0x61, 0x9f, 0xd3, 0x83, 0x74, 0x36, 0xcc, 0x83, 0x1d, 0x40,
    0x01, 0xbc, 0xcc, 0x63, 0x94, 0x36, 0x2f, 0x9f, 0x6a, 0x74, 0x6a, 0x66, 0xbd, 0xbc, 0xcb, 0xd8,
    0x97, 0x20, 0xef, 0x4a, 0x8d, 0x25, 0x83, 0x39, 0x80, 0x94, 0x35, 0x33, 0xd8, 0xd3, 0x1b, 0x02,
    0x9f, 0x66, 0x40, 0xab, 0x4d, 0xab, 0xe4, 0x10, 0x10, 0x4a, 0x02, 0x4a, 0x80, 0xc5, 0xe4, 0xb3,
    0xbd, 0xbd, 0xc6, 0xd4, 0x80, 0x9f, 0x8d, 0x80, 0xc2, 0x61, 0x74, 0xc5, 0xbd, 0x01, 0x7d, 0xd3, 
    0x33, 0x7d, 0xef, 0xb3, 0xc6, 0x97, 0x6c, 0x2f, 0xd4, 0x39, 0xc5, 0x3a, 0x63, 0x91, 0x04, 0xe4
};

static const uint8_t snd[256] = {
    0x00, 0x01, 0x04, 0x05, 0x10, 0x11, 0x14, 0x15, 0x40, 0x41, 0x44, 0x45, 0x50, 0x51, 0x54, 0x55, 
    0x1b, 0x1a, 0x1f, 0x1e, 0x0b, 0x0a, 0x0f, 0x0e, 0x5b, 0x5a, 0x5f, 0x5e, 0x4b, 0x4a, 0x4f, 0x4e, 
    0x6c, 0x6d, 0x68, 0x69, 0x7c, 0x7d, 0x78, 0x79, 0x2c, 0x2d, 0x28, 0x29, 0x3c, 0x3d, 0x38, 0x39, 
    0x77, 0x76, 0x73, 0x72, 0x67, 0x66, 0x63, 0x62, 0x37, 0x36, 0x33, 0x32, 0x27, 0x26, 0x23, 0x22, 
    0xab, 0xaa, 0xaf, 0xae, 0xbb, 0xba, 0xbf, 0xbe, 0xeb, 0xea, 0xef, 0xee, 0xfb, 0xfa, 0xff, 0xfe, 
    0xb0, 0xb1, 0xb4, 0xb5, 0xa0, 0xa1, 0xa4, 0xa5, 0xf0, 0xf1, 0xf4, 0xf5, 0xe0, 0xe1, 0xe4, 0xe5, 
    0xc7, 0xc6, 0xc3, 0xc2, 0xd7, 0xd6, 0xd3, 0xd2, 0x87, 0x86, 0x83, 0x82, 0x97, 0x96, 0x93, 0x92, 
    0xdc, 0xdd, 0xd8, 0xd9, 0xcc, 0xcd, 0xc8, 0xc9, 0x9c, 0x9d, 0x98, 0x99, 0x8c, 0x8d, 0x88, 0x89, 
    0x9a, 0x9b, 0x9e, 0x9f, 0x8a, 0x8b, 0x8e, 0x8f, 0xda, 0xdb, 0xde, 0xdf, 0xca, 0xcb, 0xce, 0xcf, 
    0x81, 0x80, 0x85, 0x84, 0x91, 0x90, 0x95, 0x94, 0xc1, 0xc0, 0xc5, 0xc4, 0xd1, 0xd0, 0xd5, 0xd4, 
    0xf6, 0xf7, 0xf2, 0xf3, 0xe6, 0xe7, 0xe2, 0xe3, 0xb6, 0xb7, 0xb2, 0xb3, 0xa6, 0xa7, 0xa2, 0xa3, 
    0xed, 0xec, 0xe9, 0xe8, 0xfd, 0xfc, 0xf9, 0xf8, 0xad, 0xac, 0xa9, 0xa8, 0xbd, 0xbc, 0xb9, 0xb8, 
    0x31, 0x30, 0x35, 0x34, 0x21, 0x20, 0x25, 0x24, 0x71, 0x70, 0x75, 0x74, 0x61, 0x60, 0x65, 0x64,
    0x2a, 0x2b, 0x2e, 0x2f, 0x3a, 0x3b, 0x3e, 0x3f, 0x6a, 0x6b, 0x6e, 0x6f, 0x7a, 0x7b, 0x7e, 0x7f, 
    0x5d, 0x5c, 0x59, 0x58, 0x4d, 0x4c, 0x49, 0x48, 0x1d, 0x1c, 0x19, 0x18, 0x0d, 0x0c, 0x09, 0x08, 
    0x46, 0x47, 0x42, 0x43, 0x56, 0x57, 0x52, 0x53, 0x06, 0x07, 0x02, 0x03, 0x16, 0x17, 0x12, 0x13 
};

static const uint8_t l_affine_snd[256] = {
    0x63, 0x7c, 0x1f, 0x00, 0x92, 0x8d, 0xee, 0xf1, 0xa4, 0xbb, 0xd8, 0xc7, 0x55, 0x4a, 0x29, 0x36, 
    0x4b, 0x54, 0x37, 0x28, 0xba, 0xa5, 0xc6, 0xd9, 0x8c, 0x93, 0xf0, 0xef, 0x7d, 0x62, 0x01, 0x1e, 
    0xc3, 0xdc, 0xbf, 0xa0, 0x32, 0x2d, 0x4e, 0x51, 0x04, 0x1b, 0x78, 0x67, 0xf5, 0xea, 0x89, 0x96, 
    0xeb, 0xf4, 0x97, 0x88, 0x1a, 0x05, 0x66, 0x79, 0x2c, 0x33, 0x50, 0x4f, 0xdd, 0xc2, 0xa1, 0xbe, 
    0xd6, 0xc9, 0xaa, 0xb5, 0x27, 0x38, 0x5b, 0x44, 0x11, 0x0e, 0x6d, 0x72, 0xe0, 0xff, 0x9c, 0x83, 
    0xfe, 0xe1, 0x82, 0x9d, 0x0f, 0x10, 0x73, 0x6c, 0x39, 0x26, 0x45, 0x5a, 0xc8, 0xd7, 0xb4, 0xab, 
    0x76, 0x69, 0x0a, 0x15, 0x87, 0x98, 0xfb, 0xe4, 0xb1, 0xae, 0xcd, 0xd2, 0x40, 0x5f, 0x3c, 0x23, 
    0x5e, 0x41, 0x22, 0x3d, 0xaf, 0xb0, 0xd3, 0xcc, 0x99, 0x86, 0xe5, 0xfa, 0x68, 0x77, 0x14, 0x0b, 
    0xdb, 0xc4, 0xa7, 0xb8, 0x2a, 0x35, 0x56, 0x49, 0x1c, 0x03, 0x60, 0x7f, 0xed, 0xf2, 0x91, 0x8e, 
    0xf3, 0xec, 0x8f, 0x90, 0x02, 0x1d, 0x7e, 0x61, 0x34, 0x2b, 0x48, 0x57, 0xc5, 0xda, 0xb9, 0xa6, 
    0x7b, 0x64, 0x07, 0x18, 0x8a, 0x95, 0xf6, 0xe9, 0xbc, 0xa3, 0xc0, 0xdf, 0x4d, 0x52, 0x31, 0x2e, 
    0x53, 0x4c, 0x2f, 0x30, 0xa2, 0xbd, 0xde, 0xc1, 0x94, 0x8b, 0xe8, 0xf7, 0x65, 0x7a, 0x19, 0x06, 
    0x6e, 0x71, 0x12, 0x0d, 0x9f, 0x80, 0xe3, 0xfc, 0xa9, 0xb6, 0xd5, 0xca, 0x58, 0x47, 0x24, 0x3b, 
    0x46, 0x59, 0x3a, 0x25, 0xb7, 0xa8, 0xcb, 0xd4, 0x81, 0x9e, 0xfd, 0xe2, 0x70, 0x6f, 0x0c, 0x13, 
    0xce, 0xd1, 0xb2, 0xad, 0x3f, 0x20, 0x43, 0x5c, 0x09, 0x16, 0x75, 0x6a, 0xf8, 0xe7, 0x84, 0x9b, 
    0xe6, 0xf9, 0x9a, 0x85, 0x17, 0x08, 0x6b, 0x74, 0x21, 0x3e, 0x5d, 0x42, 0xd0, 0xcf, 0xac, 0xb3
};
