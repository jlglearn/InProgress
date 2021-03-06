from random import randint;

SBox = [
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, 
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, 
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, 
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, 
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
];

InvSBox = [
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, 
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, 
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, 
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, 
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, 
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, 
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, 
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, 
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, 
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, 
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, 
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
];

Rcon = [0x00000000, 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000,
        0x1b000000, 0x36000000];

def getRandomBytes(n):
    return [randint(0,255) for i in range(n)];

def getRandomBits(n):
    return getRandomBytes(n/8);

def HexStrToBytes(hs):
    return [ord(hs[i]) for i in range(len(hs))];
    
def HexAsciiToBytes(ha):
    return HexStrToBytes(ha.decode('hex'));
    
def PrintHex(a):
    for i in range(len(a)):
        print "{:08x}".format(a[i]);    
        
def PrintHexBytes(a, w=16):
    if w < 1: w = 16;
    
    s = "";
    for i in range(len(a)):
        s += "{:02x} ".format(a[i]);
        if (i % w) == w - 1:
            print s;
            s = "";
            
    if (len(a) % w) != 0:
        print s;
            
    
def xor(a, b):
    n = len(a);
    if n > len(b): n = len(b);
    return [(a[i] ^ b[i]) & 0xFF for i in range(n)];

def PackWord(b0, b1, b2, b3):
    return ((b0 & 0xFF) << 24) + ((b1 & 0xFF) << 16) + ((b2 & 0xFF) << 8) + ((b3 & 0xFF) << 0);

def SubBytes(s):
    global SBox;
    return [SBox[s[i]] for i in range(len(s))];
    
def InvSubBytes(s):
    global InvSBox;
    return [InvSBox[s[i]] for i in range(len(s))];

def SplitWord(w):
    return [(w >> 24) & 0xFF, (w >> 16) & 0xFF, (w >> 8) & 0xFF, (w >> 0) & 0xFF];
    
def SubWord(w):
    t = SubBytes(SplitWord(w));
    return PackWord(t[0], t[1], t[2], t[3]);
    
def RotWord(w):
    t = SplitWord(w);
    return PackWord(t[1], t[2], t[3], t[0]);
    
def ShiftRows(s):
    r = [s[i] for i in range(len(s))];
    
    r[1*4+0] = s[1*4+1];
    r[1*4+1] = s[1*4+2];
    r[1*4+2] = s[1*4+3];
    r[1*4+3] = s[1*4+0];
    
    r[2*4+0] = s[2*4+2];
    r[2*4+1] = s[2*4+3];
    r[2*4+2] = s[2*4+0];
    r[2*4+3] = s[2*4+1];
    
    r[3*4+0] = s[3*4+3];
    r[3*4+1] = s[3*4+0];
    r[3*4+2] = s[3*4+1];
    r[3*4+3] = s[3*4+2];
    
    return r;
    
def InvShiftRows(s):
    r = [s[i] for i in range(len(s))];
    
    r[1*4+3] = s[1*4+2];
    r[1*4+2] = s[1*4+1];
    r[1*4+1] = s[1*4+0];
    r[1*4+0] = s[1*4+3];
    
    r[2*4+3] = s[2*4+1];
    r[2*4+2] = s[2*4+0];
    r[2*4+1] = s[2*4+3];
    r[2*4+0] = s[2*4+2];
    
    r[3*4+3] = s[3*4+0];
    r[3*4+2] = s[3*4+3];
    r[3*4+1] = s[3*4+2];
    r[3*4+0] = s[3*4+1];
    
    return r;

def MultiplyX(y):
    return ((y << 1) & 0xFF) ^ (((y >> 7) & 0x01) * 0x1b);
    
def Multiply(x,y):
    return ((((y >> 0) & 1) * x) ^
            (((y >> 1) & 1) * MultiplyX(x)) ^
            (((y >> 2) & 1) * MultiplyX(MultiplyX(x))) ^
            (((y >> 3) & 1) * MultiplyX(MultiplyX(MultiplyX(x)))) ^
            (((y >> 4) & 1) * MultiplyX(MultiplyX(MultiplyX(MultiplyX(x))))));

def MixColumns(s):

    n = len(s);
    
    #s1 is a copy of the input state
    s1 = [s[i] for i in range(n)];
    
    #s2 is s[i] * 2 in GF8;  This implies that s2 + s1 (s2 xor s1) is s[i] * 3 in GF8
    s2 = [((s[i] << 1) & 0xFF) ^ (0x1B if ((s[i] & 0x80) != 0) else 0x00) for i in range(n)];
    
    #create space for return state
    r  = [0 for i in range(n)];
    
    for col in range(4):
        r[0 * 4 + col] = s2[0 * 4 + col] ^ s1[3 * 4 + col] ^ s1[2 * 4 + col] ^ s2[1 * 4 + col] ^ s1[1 * 4 + col]; # 2 * a0 + a3 + a2 + 3 * a1
        r[1 * 4 + col] = s2[1 * 4 + col] ^ s1[0 * 4 + col] ^ s1[3 * 4 + col] ^ s2[2 * 4 + col] ^ s1[2 * 4 + col]; # 2 * a1 + a0 + a3 + 3 * a2
        r[2 * 4 + col] = s2[2 * 4 + col] ^ s1[1 * 4 + col] ^ s1[0 * 4 + col] ^ s2[3 * 4 + col] ^ s1[3 * 4 + col]; # 2 * a2 + a1 + a0 + 3 * a3
        r[3 * 4 + col] = s2[3 * 4 + col] ^ s1[2 * 4 + col] ^ s1[1 * 4 + col] ^ s2[0 * 4 + col] ^ s1[0 * 4 + col]; # 2 * a3 + a2 + a1 + 3 * a0
            
    return r;

def InvMixColumns(s):

    n = len(s);
    
    #create space for return state
    r  = [0 for i in range(n)];
    
    for col in range(4):
        r[0 * 4 + col] = Multiply(s[0 * 4 + col], 0x0e) ^ Multiply(s[1 * 4 + col], 0x0b) ^ Multiply(s[2 * 4 + col], 0x0d) ^ Multiply(s[3 * 4 + col], 0x09);
        r[1 * 4 + col] = Multiply(s[0 * 4 + col], 0x09) ^ Multiply(s[1 * 4 + col], 0x0e) ^ Multiply(s[2 * 4 + col], 0x0b) ^ Multiply(s[3 * 4 + col], 0x0d);
        r[2 * 4 + col] = Multiply(s[0 * 4 + col], 0x0d) ^ Multiply(s[1 * 4 + col], 0x09) ^ Multiply(s[2 * 4 + col], 0x0e) ^ Multiply(s[3 * 4 + col], 0x0b);
        r[3 * 4 + col] = Multiply(s[0 * 4 + col], 0x0b) ^ Multiply(s[1 * 4 + col], 0x0d) ^ Multiply(s[2 * 4 + col], 0x09) ^ Multiply(s[3 * 4 + col], 0x0e);
            
    return r;    
    
    
def KeyExpansion(key):
    global Rcon;
    L = len(key);
    
    Nr = 10;
    if L == 24: Nr = 12;
    if L == 32: Nr = 14;
    
    Nk = 4;
    W = [PackWord(key[i*4+0], key[i*4+1], key[i*4+2], key[i*4+3]) for i in range(Nk)];
    i = Nk;
    
    while i < 4 * (Nr+1):
        t = W[i-1];
        if (i % Nk) == 0:
            t = (SubWord(RotWord(t)) ^ Rcon[i/Nk]);
        elif (Nk > 6) and ((i % Nk) == 4):
            t = SubWord(t);
        W.append(W[i-Nk] ^ t);
        i += 1;
    
    return W;
    
def CopyState(a):
    return [a[row+col*4] for row in range(4) for col in range(4)];

def InvCopyState(a):
    return [a[row*4+col] for col in range(4) for row in range(4)];
    
def PrintState(s):
    for row in range(4):
        print "{} {} {} {}".format(chr(s[row*4+0]).encode('hex'), 
                                   chr(s[row*4+1]).encode('hex'),
                                   chr(s[row*4+2]).encode('hex'),
                                   chr(s[row*4+3]).encode('hex'));
                                   
                                   
def KeyMatrix(keys):
    return [(keys[j] >> (24 - (8 * i))) & 0xFF for i in range(4) for j in range(4)];
                                   
def AddRoundKey(s, keys):
    k = KeyMatrix(keys);
    return [s[i] ^ k[i] for i in range(len(s))];
    

def AESEncryptBlock(sIn, keys, Nr):

    s = AddRoundKey(sIn, keys[0:4]);
    
    for round in range(1, Nr+1):
        s = ShiftRows(SubBytes(s));
        
        if (round < Nr):
            s = MixColumns(s);
            
        s = AddRoundKey(s, keys[round * 4 : (round + 1) * 4]);
        
    return s;
    
def AESDecryptBlock(sIn, keys, Nr):
    s = AddRoundKey(sIn, keys[Nr * 4 : (Nr + 1)*4]);
    
    for round in range(Nr-1, -1, -1):
        s = InvSubBytes(InvShiftRows(s));     
        s = AddRoundKey(s, keys[round*4 : (round+1)*4]);
        if (round > 0): 
            s = InvMixColumns(s);
                
    return s;
        
        

def AESEncrypt(input, key):

    L = len(key);
    Nr = 10;
    if L == 24: Nr = 12;
    if L == 32: Nr = 14;
    
    keys = KeyExpansion(key);
    
    output = [];
    for block in range(len(input)/16):
        s = CopyState(input[block*16: (block+1)*16]);
        s = AESEncryptBlock(s, keys, Nr);
        output += InvCopyState(s);
        
    return output;
    
    
    
def AESDecryptCBC(sIn, key):

    # first extract iv
    iv = sIn[0:16];
    # input follows iv
    input = sIn[16:]
    
    L = len(key);
    Nr = 10;
    if L == 24: Nr = 12;
    if L == 32: Nr = 14;
    
    keys = KeyExpansion(key);
    output = [];
    mask = iv;
    
    for block in range(len(input)/16):
        t = input[block*16:(block+1)*16];
        s = CopyState(t);
        s = AESDecryptBlock(s, keys, Nr);
        output += xor(mask, InvCopyState(s));
        mask = t;
        
    # extract length of padding
    p = output[len(output)-1];
    
    return output[0:len(output)-p];

def IncCTR128(s):
    x = 0L;
    shiftcount = 120;
    for i in range(16):
        x += ((s[i] & 0xFF) << shiftcount);
        shiftcount -= 8;
        
    x += 1;
        
    return [(x >> (8*i)) & 0xFF for i in range(15,-1,-1)]
    
def AESCTR(sIn, key, fDecrypt):

    if fDecrypt != 0:
        iv = sIn[0:16];
        input = sIn[16:];
    else:
        iv = getRandomBytes(16);
        input = [sIn[i] for i in range(len(sIn))];
        
    output = [];
    ctr = [iv[i] for i in range(len(iv))];
    
    for block in range((15+len(input))/16):
        mask = AESEncrypt(ctr, key);
        output += xor(mask, input[block*16:(block+1)*16]);
        print "AESCTR(): block={}".format(block);
        print "Previous counter:"
        PrintHexBytes(ctr);
        ctr = IncCTR128(ctr);
        print "After increment:"
        PrintHexBytes(ctr);
       
    return output if fDecrypt != 0 else iv + output;
    
def AESEncryptCTR(sIn, key):
    assert(isinstance(sIn, str));
    return AESCTR([ord(sIn[i]) for i in range(len(sIn))], key, 0); 
    
def AESDecryptCTR(sIn, key):
    return AESCTR(sIn, key, 1);        
    
def AESEncryptCBC(sIn, key):

    assert(isinstance(sIn,str));

    L = len(key);
    Nr = 10;
    if L == 24: Nr = 12;
    if L == 32: Nr = 14;
    
    keys = KeyExpansion(key);
    iv = getRandomBytes(16);
    output = [];
    
    #pad input to full 16-byte blocks
    p = len(sIn) % 16;
    input = [ord(sIn[i]) for i in range(len(sIn))] + [p for i in range(16-p)];
    
    mask = iv;
    for block in range(len(input)/16):
        
        b = xor(mask, input[block*16:(block+1)*16]);
        s = CopyState(b);
        s = AESEncryptBlock(s, keys, Nr);
        c = InvCopyState(s);
        output += c;
        mask = c;       
        
    return iv + output;
        
    
    
def AESDecrypt(input, key):

    L = len(key);
    Nr = 10;
    if L == 24: Nr = 12;
    if L == 32: Nr = 14;
    
    keys = KeyExpansion(key);
    
    output = [];
    
    for block in range(len(input)/16):
        s = CopyState(input[block*16 : (block + 1)*16]);
        s = AESDecryptBlock(s, keys, Nr);
        output += InvCopyState(s);
        
    return output;
    
def GetPlaintext():
    return [0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff];
    
def GetKey():
    return [0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f];
