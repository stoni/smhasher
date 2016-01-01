#include "Platform.h"


// FNV1A_YoshimitsuTRIADiiXMMx2 (revision 2 of FNV1A_YoshimitsuTRIADiiXMM, just unrolled once) aka FNV1A_penumbra, copyleft 2013-Jun-15 Kaze.
// PENUMBRA: Any partial shade or shadow round a thing; a surrounding area of uncertain extent (lit. & fig.). [mod. Latin, from Latin paene almost + umbra shadow.] 
//
// Hoy en mi ventana brilla el sol / The sun shines through my window today
// Y el coraz�n se pone triste contemplando la ciudad / And my heart feels sad while contemplating the city
// Porque te vas / Because you are leaving
// Como cada noche despert� pensando en ti / Just like every night, I woke up thinking of you
// Y en mi reloj todas las horas vi pasar / And I saw as all the hours passed by in my clock
// Porque te vas / Because you are leaving
// Todas las promesas de mi amor se ir�n contigo / All my love promises will be gone with you
// Me olvidaras, me olvidaras / You will forget me, you will forget me
// Junto a la estaci�n llorar� igual que un ni�o / Next to the station I will cry like a child
// Porque te vas, porque te vas / Because you are leaving, because you are leaving
// Bajo la penumbra de un farol se dormir�n / Under the shadow of a street lamp they will sleep
// Todas las cosas que quedaron por decir se dormir�n / All the things left unsaid will sleep there
// Junto a las manillas de un reloj esperar�n / They will wait next to a clock's hands
// Todas las horas que quedaron por vivir esperar�n / They will wait for all those hours that we had yet to live
// /J[e]anette - 'Porque te vas' lyrics/
//
// Many dependencies, many mini-goals, many restrictions... Blah-blah-blah...
// Yet in my amateurish view the NIFTIEST HT lookups function emerged, it is FNV1A_YoshimitsuTRIADii.
// Main feature: general purpose HT lookups function targeted as 32bit code and 32bit stamp, superfast for 'any length' keys, escpecially useful for text messages.
//
//#include <emmintrin.h> //SSE2
//#include <smmintrin.h> //SSE4.1
//#include <immintrin.h> //AVX
#define xmmload(p) _mm_load_si128((__m128i const*)(p))
#define xmmloadu(p) _mm_loadu_si128((__m128i const*)(p))
#define _rotl_KAZE128(x, n) _mm_or_si128(_mm_slli_si128(x, n) , _mm_srli_si128(x, 128-n))
#define _rotl_KAZE32(x, n) (((x) << (n)) | ((x) >> (32-(n))))
#define XMM_KAZE_SSE2
// For better mixing the above 'define' should be commented while the next one uncommented!
//#define XMM_KAZE_SSE4
void FNV1A_penumbra(const void * key, int len, uint32_t seed, void *out)
{
    const uint32_t PRIME = 709607;
    uint32_t hash32 = seed+2166136261;
    uint32_t hash32B = seed+2166136261;
    uint32_t hash32C = seed+2166136261;
    const char *p = (const char*)key;
    uint32_t Loop_Counter;
    uint32_t Second_Line_Offset;
    uint32_t h[4];

#if defined(XMM_KAZE_SSE2) || defined(XMM_KAZE_SSE4) || defined(XMM_KAZE_AVX)
    __m128i xmm0;   // Defined for clarity: No need of defining it, the compiler sees well and uses no intermediate.
    __m128i xmm1;   // Defined for clarity: No need of defining it, the compiler sees well and uses no intermediate.
    __m128i xmm2;   // Defined for clarity: No need of defining it, the compiler sees well and uses no intermediate.
    __m128i xmm3;   // Defined for clarity: No need of defining it, the compiler sees well and uses no intermediate.
    __m128i xmm4;   // Defined for clarity: No need of defining it, the compiler sees well and uses no intermediate.
    __m128i xmm5;   // Defined for clarity: No need of defining it, the compiler sees well and uses no intermediate.
    __m128i xmm0nd; // Defined for clarity: No need of defining it, the compiler sees well and uses no intermediate.
    __m128i xmm1nd; // Defined for clarity: No need of defining it, the compiler sees well and uses no intermediate.
    __m128i xmm2nd; // Defined for clarity: No need of defining it, the compiler sees well and uses no intermediate.
    __m128i xmm3nd; // Defined for clarity: No need of defining it, the compiler sees well and uses no intermediate.
    __m128i xmm4nd; // Defined for clarity: No need of defining it, the compiler sees well and uses no intermediate.
    __m128i xmm5nd; // Defined for clarity: No need of defining it, the compiler sees well and uses no intermediate.
    __m128i hash32xmm = _mm_set1_epi32(2166136261);
    __m128i hash32Bxmm = _mm_set1_epi32(2166136261);
    __m128i hash32Cxmm = _mm_set1_epi32(2166136261);
    __m128i PRIMExmm = _mm_set1_epi32(709607);
#endif

#if defined(XMM_KAZE_SSE2) || defined(XMM_KAZE_SSE4) || defined(XMM_KAZE_AVX)
if (len >= 2*4*24) { // Actually 2*4*24 is the minimum and not useful, 200++ makes more sense.
    Loop_Counter = (len/(2*4*24));
    Loop_Counter++;
    Second_Line_Offset = len-(Loop_Counter)*(2*4*3*4);
    for(; Loop_Counter; Loop_Counter--, p += 2*4*3*sizeof(uint32_t)) {
	xmm0 = xmmloadu(p+0*16);
	xmm1 = xmmloadu(p+0*16+Second_Line_Offset);
	xmm2 = xmmloadu(p+1*16);
	xmm3 = xmmloadu(p+1*16+Second_Line_Offset);
	xmm4 = xmmloadu(p+2*16);
	xmm5 = xmmloadu(p+2*16+Second_Line_Offset);
	xmm0nd = xmmloadu(p+3*16);
	xmm1nd = xmmloadu(p+3*16+Second_Line_Offset);
	xmm2nd = xmmloadu(p+4*16);
	xmm3nd = xmmloadu(p+4*16+Second_Line_Offset);
	xmm4nd = xmmloadu(p+5*16);
	xmm5nd = xmmloadu(p+5*16+Second_Line_Offset);
#if defined(XMM_KAZE_SSE2)
		hash32xmm = _mm_mullo_epi16(_mm_xor_si128(hash32xmm , _mm_xor_si128(_rotl_KAZE128(xmm0,5) , xmm1)) , PRIMExmm);       
		hash32Bxmm = _mm_mullo_epi16(_mm_xor_si128(hash32Bxmm , _mm_xor_si128(_rotl_KAZE128(xmm3,5) , xmm2)) , PRIMExmm);        
		hash32Cxmm = _mm_mullo_epi16(_mm_xor_si128(hash32Cxmm , _mm_xor_si128(_rotl_KAZE128(xmm4,5) , xmm5)) , PRIMExmm);      
		hash32xmm = _mm_mullo_epi16(_mm_xor_si128(hash32xmm , _mm_xor_si128(_rotl_KAZE128(xmm0nd,5) , xmm1nd)) , PRIMExmm);       
		hash32Bxmm = _mm_mullo_epi16(_mm_xor_si128(hash32Bxmm , _mm_xor_si128(_rotl_KAZE128(xmm3nd,5) , xmm2nd)) , PRIMExmm);        
		hash32Cxmm = _mm_mullo_epi16(_mm_xor_si128(hash32Cxmm , _mm_xor_si128(_rotl_KAZE128(xmm4nd,5) , xmm5nd)) , PRIMExmm);      
#else
		hash32xmm = _mm_mullo_epi32(_mm_xor_si128(hash32xmm , _mm_xor_si128(_rotl_KAZE128(xmm0,5) , xmm1)) , PRIMExmm);       
		hash32Bxmm = _mm_mullo_epi32(_mm_xor_si128(hash32Bxmm , _mm_xor_si128(_rotl_KAZE128(xmm3,5) , xmm2)) , PRIMExmm);        
		hash32Cxmm = _mm_mullo_epi32(_mm_xor_si128(hash32Cxmm , _mm_xor_si128(_rotl_KAZE128(xmm4,5) , xmm5)) , PRIMExmm);      
		hash32xmm = _mm_mullo_epi32(_mm_xor_si128(hash32xmm , _mm_xor_si128(_rotl_KAZE128(xmm0nd,5) , xmm1nd)) , PRIMExmm);       
		hash32Bxmm = _mm_mullo_epi32(_mm_xor_si128(hash32Bxmm , _mm_xor_si128(_rotl_KAZE128(xmm3nd,5) , xmm2nd)) , PRIMExmm);        
		hash32Cxmm = _mm_mullo_epi32(_mm_xor_si128(hash32Cxmm , _mm_xor_si128(_rotl_KAZE128(xmm4nd,5) , xmm5nd)) , PRIMExmm);      
#endif
    }
#if defined(XMM_KAZE_SSE2)
	hash32xmm = _mm_mullo_epi16(_mm_xor_si128(hash32xmm , hash32Bxmm) , PRIMExmm);       
 	hash32xmm = _mm_mullo_epi16(_mm_xor_si128(hash32xmm , hash32Cxmm) , PRIMExmm);       
#else
	hash32xmm = _mm_mullo_epi32(_mm_xor_si128(hash32xmm , hash32Bxmm) , PRIMExmm);       
 	hash32xmm = _mm_mullo_epi32(_mm_xor_si128(hash32xmm , hash32Cxmm) , PRIMExmm);       
#endif
        _mm_storeu_si128((__m128i *)h, hash32xmm);
        hash32 = (hash32 ^ h[0]) * PRIME;
        hash32B = (hash32B ^ h[3]) * PRIME;
        hash32 = (hash32 ^ h[1]) * PRIME;
        hash32B = (hash32B ^ h[2]) * PRIME;
        /*hash32 = (hash32 ^ hash32xmm.m128i_u32[0]) * PRIME;
	hash32B = (hash32B ^ hash32xmm.m128i_u32[3]) * PRIME;
	hash32 = (hash32 ^ hash32xmm.m128i_u32[1]) * PRIME;
	hash32B = (hash32B ^ hash32xmm.m128i_u32[2]) * PRIME;*/
} else if (len >= 24)
#else
if (len >= 24)
#endif
{
    Loop_Counter = (len/24);
    Loop_Counter++;
    Second_Line_Offset = len-(Loop_Counter)*(3*4);
    for(; Loop_Counter; Loop_Counter--, p += 3*sizeof(uint32_t)) {
		hash32 = (hash32 ^ (_rotl_KAZE32(*(uint32_t *)(p+0),5) ^ *(uint32_t *)(p+0+Second_Line_Offset))) * PRIME;        
		hash32B = (hash32B ^ (_rotl_KAZE32(*(uint32_t *)(p+4+Second_Line_Offset),5) ^ *(uint32_t *)(p+4))) * PRIME;        
		hash32C = (hash32C ^ (_rotl_KAZE32(*(uint32_t *)(p+8),5) ^ *(uint32_t *)(p+8+Second_Line_Offset))) * PRIME;        
    }
		hash32 = (hash32 ^ _rotl_KAZE32(hash32C,5) ) * PRIME;
} else {
    // 1111=15; 10111=23
    if (len & 4*sizeof(uint32_t)) {	
		hash32 = (hash32 ^ (_rotl_KAZE32(*(uint32_t *)(p+0),5) ^ *(uint32_t *)(p+4))) * PRIME;        
		hash32B = (hash32B ^ (_rotl_KAZE32(*(uint32_t *)(p+8),5) ^ *(uint32_t *)(p+12))) * PRIME;        
		p += 8*sizeof(uint16_t);
    }
    // Cases: 0,1,2,3,4,5,6,7,...,15
    if (len & 2*sizeof(uint32_t)) {
		hash32 = (hash32 ^ *(uint32_t*)(p+0)) * PRIME;
		hash32B = (hash32B ^ *(uint32_t*)(p+4)) * PRIME;
		p += 4*sizeof(uint16_t);
    }
    // Cases: 0,1,2,3,4,5,6,7
    if (len & sizeof(uint32_t)) {
		hash32 = (hash32 ^ *(uint16_t*)(p+0)) * PRIME;
		hash32B = (hash32B ^ *(uint16_t*)(p+2)) * PRIME;
		p += 2*sizeof(uint16_t);
    }
    if (len & sizeof(uint16_t)) {
        hash32 = (hash32 ^ *(uint16_t*)p) * PRIME;
        p += sizeof(uint16_t);
    }
    if (len & 1) 
        hash32 = (hash32 ^ *p) * PRIME;
}
    hash32 = (hash32 ^ _rotl_KAZE32(hash32B,5) ) * PRIME;
    *(uint32_t*)out = hash32 ^ (hash32 >> 16);
}

// FNV1A_YoshimitsuTRIADiiXMM revision 1+ aka FNV1A_SaberFatigue, copyleft 2013-Apr-26 Kaze.
// Targeted purpose: x-gram table lookups for Leprechaun r17.
// Targeted machine: assuming SSE2 is present always - no non-SSE2 counterpart.
//#include <emmintrin.h> //SSE2
//#include <smmintrin.h> //SSE4.1
//#include <immintrin.h> //AVX
#define xmmload(p) _mm_load_si128((__m128i const*)(p))
#define xmmloadu(p) _mm_loadu_si128((__m128i const*)(p))
#define _rotl_KAZE128(x, n) _mm_or_si128(_mm_slli_si128(x, n) , _mm_srli_si128(x, 128-n))
#define _rotl_KAZE32(x, n) (((x) << (n)) | ((x) >> (32-(n))))
#define XMM_KAZE_SSE2
uint32_t FNV1A_Hash_YoshimitsuTRIADiiXMM(const char *str, uint32_t wrdlen)
{
    const uint32_t PRIME = 709607;
    uint32_t hash32 = 2166136261;
    uint32_t hash32B = 2166136261;
    uint32_t hash32C = 2166136261;
    const char *p = str;
    uint32_t Loop_Counter;
    uint32_t Second_Line_Offset;
    uint32_t h[4];

#if defined(XMM_KAZE_SSE2) || defined(XMM_KAZE_SSE4) || defined(XMM_KAZE_AVX)
    __m128i xmm0;
    __m128i xmm1;
    __m128i xmm2;
    __m128i xmm3;
    __m128i xmm4;
    __m128i xmm5;
    __m128i hash32xmm = _mm_set1_epi32(2166136261);
    __m128i hash32Bxmm = _mm_set1_epi32(2166136261);
    __m128i hash32Cxmm = _mm_set1_epi32(2166136261);
    __m128i PRIMExmm = _mm_set1_epi32(709607);
#endif

#if defined(XMM_KAZE_SSE2) || defined(XMM_KAZE_SSE4) || defined(XMM_KAZE_AVX)
if (wrdlen >= 4*24) { // Actually 4*24 is the minimum and not useful, 200++ makes more sense.
    Loop_Counter = (wrdlen/(4*24));
    Loop_Counter++;
    Second_Line_Offset = wrdlen-(Loop_Counter)*(4*3*4);
    for(; Loop_Counter; Loop_Counter--, p += 4*3*sizeof(uint32_t)) {
	xmm0 = xmmloadu(p+0*16);
	xmm1 = xmmloadu(p+0*16+Second_Line_Offset);
	xmm2 = xmmloadu(p+1*16);
	xmm3 = xmmloadu(p+1*16+Second_Line_Offset);
	xmm4 = xmmloadu(p+2*16);
	xmm5 = xmmloadu(p+2*16+Second_Line_Offset);
#if defined(XMM_KAZE_SSE2)
		hash32xmm = _mm_mullo_epi16(_mm_xor_si128(hash32xmm , _mm_xor_si128(_rotl_KAZE128(xmm0,5) , xmm1)) , PRIMExmm);       
		hash32Bxmm = _mm_mullo_epi16(_mm_xor_si128(hash32Bxmm , _mm_xor_si128(_rotl_KAZE128(xmm3,5) , xmm2)) , PRIMExmm);        
		hash32Cxmm = _mm_mullo_epi16(_mm_xor_si128(hash32Cxmm , _mm_xor_si128(_rotl_KAZE128(xmm4,5) , xmm5)) , PRIMExmm);      
#else
		hash32xmm = _mm_mullo_epi32(_mm_xor_si128(hash32xmm , _mm_xor_si128(_rotl_KAZE128(xmm0,5) , xmm1)) , PRIMExmm);       
		hash32Bxmm = _mm_mullo_epi32(_mm_xor_si128(hash32Bxmm , _mm_xor_si128(_rotl_KAZE128(xmm3,5) , xmm2)) , PRIMExmm);        
		hash32Cxmm = _mm_mullo_epi32(_mm_xor_si128(hash32Cxmm , _mm_xor_si128(_rotl_KAZE128(xmm4,5) , xmm5)) , PRIMExmm);      
#endif
    }
#if defined(XMM_KAZE_SSE2)
	hash32xmm = _mm_mullo_epi16(_mm_xor_si128(hash32xmm , hash32Bxmm) , PRIMExmm);       
 	hash32xmm = _mm_mullo_epi16(_mm_xor_si128(hash32xmm , hash32Cxmm) , PRIMExmm);       
#else
	hash32xmm = _mm_mullo_epi32(_mm_xor_si128(hash32xmm , hash32Bxmm) , PRIMExmm);       
 	hash32xmm = _mm_mullo_epi32(_mm_xor_si128(hash32xmm , hash32Cxmm) , PRIMExmm);       
#endif
        _mm_storeu_si128((__m128i *)h, hash32xmm);
        hash32 = (hash32 ^ h[0]) * PRIME;
        hash32B = (hash32B ^ h[3]) * PRIME;
        hash32 = (hash32 ^ h[1]) * PRIME;
        hash32B = (hash32B ^ h[2]) * PRIME;
	/*hash32 = (hash32 ^ hash32xmm.m128i_u32[0]) * PRIME;
	hash32B = (hash32B ^ hash32xmm.m128i_u32[3]) * PRIME;
	hash32 = (hash32 ^ hash32xmm.m128i_u32[1]) * PRIME;
	hash32B = (hash32B ^ hash32xmm.m128i_u32[2]) * PRIME;*/
} else if (wrdlen >= 24)
#else
if (wrdlen >= 24)
#endif
{
    Loop_Counter = (wrdlen/24);
    Loop_Counter++;
    Second_Line_Offset = wrdlen-(Loop_Counter)*(3*4);
    for(; Loop_Counter; Loop_Counter--, p += 3*sizeof(uint32_t)) {
		hash32 = (hash32 ^ (_rotl_KAZE32(*(uint32_t *)(p+0),5) ^ *(uint32_t *)(p+0+Second_Line_Offset))) * PRIME;        
		hash32B = (hash32B ^ (_rotl_KAZE32(*(uint32_t *)(p+4+Second_Line_Offset),5) ^ *(uint32_t *)(p+4))) * PRIME;        
		hash32C = (hash32C ^ (_rotl_KAZE32(*(uint32_t *)(p+8),5) ^ *(uint32_t *)(p+8+Second_Line_Offset))) * PRIME;        
    }
		hash32 = (hash32 ^ _rotl_KAZE32(hash32C,5) ) * PRIME;
} else {
    // 1111=15; 10111=23
    if (wrdlen & 4*sizeof(uint32_t)) {	
		hash32 = (hash32 ^ (_rotl_KAZE32(*(uint32_t *)(p+0),5) ^ *(uint32_t *)(p+4))) * PRIME;        
		hash32B = (hash32B ^ (_rotl_KAZE32(*(uint32_t *)(p+8),5) ^ *(uint32_t *)(p+12))) * PRIME;        
		p += 8*sizeof(uint16_t);
    }
    // Cases: 0,1,2,3,4,5,6,7,...,15
    if (wrdlen & 2*sizeof(uint32_t)) {
		hash32 = (hash32 ^ *(uint32_t*)(p+0)) * PRIME;
		hash32B = (hash32B ^ *(uint32_t*)(p+4)) * PRIME;
		p += 4*sizeof(uint16_t);
    }
    // Cases: 0,1,2,3,4,5,6,7
    if (wrdlen & sizeof(uint32_t)) {
		hash32 = (hash32 ^ *(uint16_t*)(p+0)) * PRIME;
		hash32B = (hash32B ^ *(uint16_t*)(p+2)) * PRIME;
		p += 2*sizeof(uint16_t);
    }
    if (wrdlen & sizeof(uint16_t)) {
        hash32 = (hash32 ^ *(uint16_t*)p) * PRIME;
        p += sizeof(uint16_t);
    }
    if (wrdlen & 1) 
        hash32 = (hash32 ^ *p) * PRIME;
}
    hash32 = (hash32 ^ _rotl_KAZE32(hash32B,5) ) * PRIME;
    return hash32 ^ (hash32 >> 16);
}

// [North Star One-Sword School]
// - My name is Kanichiro Yoshimura.
//   I'm a new man. Just so you'll know who I am...
//   Saito-sensei.
//   What land are you from?
// - 'Land'?
//   Yes.
//   I was born in Morioka, in Nanbu, Oshu.
//   It's a beautiful place.
//   Please...
//   Away to the south is Mt Hayachine...
//   with Mt Nansho and Mt Azumane to the west.
//   In the north are Mt Iwate and Mt Himekami.
//   Out of the high mountains flows the Nakatsu River...
//   through the castle town into the Kitakami below Sakuranobaba.
//   Ah, it's pretty as a picture!
//   There's nowhere like it in all Japan!
// ...
// - Hijikata-sensei... as you're aware, the circumstances... made the task quite difficult.
//   It caused a chip in the blade of my sword.
//   Could I perhaps ask for... the cost of a new sword?
// - That should do, Your blade doesn't bear its maker's name.
// - You're too kind. My humble thanks!
// - What kind of a samurai is that?
// - He's really something!
// ...
// - Where's it chipped?
// - My sword's as worn down as I am.
// ...
// The Shinsengumi was... the sterile flower of the Shoguns' end.
// /Paragon Kiichi Nakai in the paragon piece-of-art 'The Wolves of Mibu' aka 'WHEN THE LAST SWORD IS DRAWN'/
// As I said on one Japanese forum, Kiichi Nakai deserves an award worth his weight in gold, nah-nah, in DIAMONDS!
#ifndef WIN32
#define yoshi_rotl ROTL32
#else
#define yoshi_rotl ROTL32
#endif
uint32_t FNV1A_Hash_Yoshimura(const char *str, uint32_t wrdlen, uint32_t seed)
{
    const uint32_t PRIME = 709607;
    uint32_t hash32 = 2166136261+seed;
    uint32_t hash32B = 2166136261+seed;
    const char *p = str;
    uint32_t Loop_Counter;
    uint32_t Second_Line_Offset;

if (wrdlen >= 2*2*sizeof(uint32_t)) {
    Second_Line_Offset = wrdlen-((wrdlen>>4)+1)*(2*4); // ((wrdlen>>1)>>3)
    Loop_Counter = (wrdlen>>4);
    //if (wrdlen%16) Loop_Counter++;
    Loop_Counter++;
    for(; Loop_Counter; Loop_Counter--, p += 2*sizeof(uint32_t)) {
		// revision 1:
		//hash32 = (hash32 ^ (_rotl(*(uint32_t *)(p+0),5) ^ *(uint32_t *)(p+4))) * PRIME;        
		//hash32B = (hash32B ^ (_rotl(*(uint32_t *)(p+0+Second_Line_Offset),5) ^ *(uint32_t *)(p+4+Second_Line_Offset))) * PRIME;        
		// revision 2:
		hash32 = (hash32 ^ (yoshi_rotl(*(uint32_t *)(p+0),5) ^ *(uint32_t *)(p+0+Second_Line_Offset))) * PRIME;        
		hash32B = (hash32B ^ (yoshi_rotl(*(uint32_t *)(p+4+Second_Line_Offset),5) ^ *(uint32_t *)(p+4))) * PRIME;        
    }
} else {
    // Cases: 0,1,2,3,4,5,6,7,...,15
    if (wrdlen & 2*sizeof(uint32_t)) {
		hash32 = (hash32 ^ *(uint32_t*)(p+0)) * PRIME;
		hash32B = (hash32B ^ *(uint32_t*)(p+4)) * PRIME;
		p += 4*sizeof(uint16_t);
    }
    // Cases: 0,1,2,3,4,5,6,7
    if (wrdlen & sizeof(uint32_t)) {
		hash32 = (hash32 ^ *(uint16_t*)(p+0)) * PRIME;
		hash32B = (hash32B ^ *(uint16_t*)(p+2)) * PRIME;
		p += 2*sizeof(uint16_t);
    }
    if (wrdlen & sizeof(uint16_t)) {
        hash32 = (hash32 ^ *(uint16_t*)p) * PRIME;
        p += sizeof(uint16_t);
    }
    if (wrdlen & 1) 
        hash32 = (hash32 ^ *p) * PRIME;
}
    hash32 = (hash32 ^ yoshi_rotl(hash32B,5) ) * PRIME;
    return hash32 ^ (hash32 >> 16);
}

#define _rotl_KAZE(x, n) (((x) << (n)) | ((x) >> (32-(n))))
//E:\Knight-tour_r8dump_Yorikke_CRC8_BRUTUS+\HASH_linearspeed_Yorikke_vs_CRC32_FURY>HASH_linearspeed_FURY.exe
//HASH_linearspeed_FURY: FNV1A_Yorikke vs CRC32_8slice, rev.3 written by Kaze.
//Note1: The size-and-speed of L1/L2/L3 cache affects FNV1A_Yorikke and mostly FNV1A_Yoshimitsu a lot!
//Note2: Executable was compiled with Microsoft 32bit VS2010 /Ox.
//Note3: Results on my laptop:
//
//OS: Windows XP 32bit
//Motherboard Name: Toshiba Satellite L305
//CPU Type: Mobile DualCore Intel Core 2 Duo T7500
//CPU Alias: Merom
//CPU Clock: 2194.7 MHz (original: 2200 MHz)
//CPU Multiplier: 11x
//CPU FSB: 199.5 MHz (original: 200 MHz)
//Memory Bus: 332.5 MHz
//L1 Code Cache: 32 KB per core
//L1 Data Cache: 32 KB per core
//L2 Cache: 4 MB (On-Die, ECC, ASC, Full-Speed)
//Memory Timings: 5-5-5-13 (CL-RCD-RP-RAS)
//Instruction Set: x86, x86-64, MMX, SSE, SSE2, SSE3, SSSE3
//Transistors: 291 million
//Process Technology: 8M, 65 nm, CMOS, Cu, Low-K Inter-Layer, 2nd Gen Strained Si
//Front Side Bus Properties:
//   Bus Type: Intel AGTL+
//   Bus Width: 64-bit
//   Real Clock: 200 MHz (QDR)
//   Effective Clock: 800 MHz
//   Bandwidth: 6400 MB/s
//Memory Bus Properties:
//   Bus Type: Dual DDR2 SDRAM
//   Bus Width: 128-bit
//   DRAM:FSB Ratio: 10:6
//   Real Clock: 333 MHz (DDR)
//   Effective Clock: 667 MHz
//   Bandwidth: 10667 MB/s
//
//And the results:
//
//Hashing a 64MB block 1024 times i.e. 64GB ...
//FNV1A_Yoshimitsu: (64MB block); 65536MB hashed in 16875 clocks or 3.884MB per clock
//FNV1A_Yorikke: (64MB block); 65536MB hashed in 16782 clocks or 3.905MB per clock
//CRC_SlicingBy8K2: (64MB block); 65536MB hashed in 66390 clocks or 0.987MB per clock
//
//Hashing a 10MB block 8*1024 times ...
//FNV1A_Yoshimitsu: (10MB block); 81920MB hashed in 20610 clocks or 3.975MB per clock
//FNV1A_Yorikke: (10MB block); 81920MB hashed in 20546 clocks or 3.987MB per clock
//CRC_SlicingBy8K2: (10MB block); 81920MB hashed in 82938 clocks or 0.988MB per clock
//
//Hashing a 5MB block 8*1024 times ...
//FNV1A_Yoshimitsu: (5MB block); 40960MB hashed in 9562 clocks or 4.284MB per clock
//FNV1A_Yorikke: (5MB block); 40960MB hashed in 9531 clocks or 4.298MB per clock
//CRC_SlicingBy8K2: (5MB block); 40960MB hashed in 41110 clocks or 0.996MB per clock
//
//Hashing a 2MB block 8*1024 times ...
//FNV1A_Yoshimitsu: (2MB block); 16384MB hashed in 2578 clocks or 6.355MB per clock
//FNV1A_Yorikke: (2MB block); 16384MB hashed in 2657 clocks or 6.166MB per clock
//CRC_SlicingBy8K2: (2MB block); 16384MB hashed in 16156 clocks or 1.014MB per clock
//
//Hashing a 16KB block 1024*1024 times ...
//FNV1A_Yoshimitsu: (16KB block); 16384MB hashed in 2437 clocks or 6.723MB per clock
//FNV1A_Yorikke: (16KB block); 16384MB hashed in 2547 clocks or 6.433MB per clock
//CRC_SlicingBy8K2: (16KB block); 16384MB hashed in 16078 clocks or 1.019MB per clock
//
//Yoshimitsu is beyond hope - he fights as a true underdog - nothing to lose everything to gain, sharing with those in need - a true copylefter.
uint32_t FNV1A_Hash_Yoshimitsu(const char *str, uint32_t wrdlen, uint32_t seed)
{
    const uint32_t PRIME = 709607;
    uint32_t hash32 = 2166136261+seed;
    uint32_t hash32B = 2166136261+seed;
    uint32_t hash32C = 2166136261+seed;
    uint32_t hash32D = 2166136261+seed;
    const char *p = str;

    for(; wrdlen >= 4*2*sizeof(uint32_t); wrdlen -= 4*2*sizeof(uint32_t), p += 4*2*sizeof(uint32_t)) {
        //hash32 = (hash32 ^ (_rotl_KAZE(*(DWORD *)(p+0),5) ^ *(DWORD *)(p+4))) * PRIME;        
        //hash32B = (hash32B ^ (_rotl_KAZE(*(DWORD *)(p+8),5) ^ *(DWORD *)(p+12))) * PRIME;        
//        hash32 = _rotl_KAZE( (hash32 ^ (_rotl_KAZE(*(DWORD *)(p+0),5) ^ *(DWORD *)(p+4))) , 19); // 1<<19 = 524288 ~ 709607; * -> <<'s
//        hash32B = _rotl_KAZE( (hash32B ^ (_rotl_KAZE(*(DWORD *)(p+8),5) ^ *(DWORD *)(p+12))) , 19); 
/*
$LL7@FNV1A_Hash@2:
; Line 159
	mov	ebx, DWORD PTR [eax]
	rol	ebx, 5
	xor	ebx, DWORD PTR [eax+4]
	sub	edx, 16
	xor	esi, ebx
; Line 160
	mov	ebx, DWORD PTR [eax+8]
	ror	esi, 13
	rol	ebx, 5
	xor	ebx, DWORD PTR [eax+12]
	add	eax, 16
	xor	ecx, ebx
	ror	ecx, 13
	dec	edi
	jne	SHORT $LL7@FNV1A_Hash@2
*/

		//3*2*sizeof(DWORD)
		//hash32 = ( (hash32 ^ (_rotl(*(DWORD *)(p+0),5) ^ *(DWORD *)(p+4))) * PRIME ) ^ ( *(DWORD *)(p+8) * PRIME );
		//hash32B = ( (hash32B ^ (_rotl(*(DWORD *)(p+12),5) ^ *(DWORD *)(p+16))) * PRIME ) ^ ( *(DWORD *)(p+20) * PRIME );

		//4*2*sizeof(DWORD)
		hash32 = (hash32 ^ (yoshi_rotl(*(uint32_t *)(p+0),5) ^ *(uint32_t *)(p+4))) * PRIME;        
		hash32B = (hash32B ^ (yoshi_rotl(*(uint32_t *)(p+8),5) ^ *(uint32_t *)(p+12))) * PRIME;        
		hash32C = (hash32C ^ (yoshi_rotl(*(uint32_t *)(p+16),5) ^ *(uint32_t *)(p+20))) * PRIME;        
		hash32D = (hash32D ^ (yoshi_rotl(*(uint32_t *)(p+24),5) ^ *(uint32_t *)(p+28))) * PRIME;        
// 442-3f5+2= 79 bytes, CARAMBA: there is no free register to house the counter: 'dec DWORD PTR tv170[esp+12]' instead of 'dec edi' as in "ROL 13" lines below.
/*
$LL9@Hash_Yoshi:
; 1498 : 		hash32 = (hash32 ^ (_rotl(*(DWORD *)(p+0),5) ^ *(DWORD *)(p+4))) * PRIME;        

  003f5	8b 18		 mov	 ebx, DWORD PTR [eax]
  003f7	c1 c3 05	 rol	 ebx, 5
  003fa	33 58 04	 xor	 ebx, DWORD PTR [eax+4]
  003fd	83 ea 20	 sub	 edx, 32
  00400	33 f3		 xor	 esi, ebx

; 1499 : 		hash32B = (hash32B ^ (_rotl(*(DWORD *)(p+8),5) ^ *(DWORD *)(p+12))) * PRIME;        

  00402	8b 58 08	 mov	 ebx, DWORD PTR [eax+8]
  00405	69 f6 e7 d3 0a
	00		 imul	 esi, 709607
  0040b	c1 c3 05	 rol	 ebx, 5
  0040e	33 58 0c	 xor	 ebx, DWORD PTR [eax+12]
  00411	83 c0 20	 add	 eax, 32
  00414	33 cb		 xor	 ecx, ebx

; 1500 : 		hash32C = (hash32C ^ (_rotl(*(DWORD *)(p+16),5) ^ *(DWORD *)(p+20))) * PRIME;        

  00416	8b 58 f0	 mov	 ebx, DWORD PTR [eax-16]
  00419	69 c9 e7 d3 0a
	00		 imul	 ecx, 709607
  0041f	c1 c3 05	 rol	 ebx, 5
  00422	33 58 f4	 xor	 ebx, DWORD PTR [eax-12]
  00425	33 fb		 xor	 edi, ebx

; 1501 : 		hash32D = (hash32D ^ (_rotl(*(DWORD *)(p+24),5) ^ *(DWORD *)(p+28))) * PRIME;        

  00427	8b 58 f8	 mov	 ebx, DWORD PTR [eax-8]
  0042a	69 ff e7 d3 0a
	00		 imul	 edi, 709607
  00430	c1 c3 05	 rol	 ebx, 5
  00433	33 58 fc	 xor	 ebx, DWORD PTR [eax-4]
  00436	33 eb		 xor	 ebp, ebx
  00438	69 ed e7 d3 0a
	00		 imul	 ebp, 709607
  0043e	ff 4c 24 18	 dec	 DWORD PTR tv170[esp+12]
  00442	75 b1		 jne	 SHORT $LL9@Hash_Yoshi
*/
/*
E:\_KAZE_hash_Yoshimitsu>hash KT5million.txt
5000000 lines read
16777216 elements in the table (24 bits)
           Jesteress:    5927892   5892203   5891877   5885865   5895333   5892451   5886949   5891559   5895370   5888990|   5885865 [676877]
              Meiyan:    5850738   5855457   5847735   5849722   5847868   5845189   5850087   5846843   5847820   5845907|   5845189 [676877]
             Yorikke:    5680415   5679205   5684177   5680280   5686396   5680565   5681035   5679829   5682245   5681052|   5679205 [677478]
          Yoshimitsu:    5680400   5680252   5684833   5688414   5680524   5678596   5677554   5678393   5680015   5680396|   5677554 [675312]
        x17 unrolled: ^C
E:\_KAZE_hash_Yoshimitsu>hash KTR5million.txt
5000000 lines read
16777216 elements in the table (24 bits)
           Jesteress:    5772704   5742091   5736934   5738746   5742657   5741104   5741102   5737908   5738002   5742116|   5736934 [676000]
              Meiyan:    5700057   5695889   5700702   5694799   5699576   5697662   5693863   5697606   5696133   5695485|   5693863 [676000]
             Yorikke:    5523796   5527643   5528195   5530897   5523567   5526035   5521989   5527258   5531218   5524544|   5521989 [676570]
          Yoshimitsu:    5520833   5521105   5518502   5517657   5519659   5520483   5517938   5521405   5519923   5518780|   5517657 [676009]
        x17 unrolled: ^C
E:\_KAZE_hash_Yoshimitsu>hash "Word-list_12,561,874_wikipedia-en-html.tar.wrd"
12561874 lines read
33554432 elements in the table (25 bits)
           Jesteress:   10749318  10649149  10646657  10635909  10657954  10645701  10656154  10656770  10672870  10637745|  10635909 [2121868]
              Meiyan:   10632817  10635278  10610784  10636058  10615126  10637501  10633724  10638480  10633424  10632864|  10610784 [2111271]
             Yorikke:   10656032  10672142  10673068  10673462  10671225  10673482  10670054  10672436  10677540  10670661|  10656032 [2084954]
          Yoshimitsu:   10828843  10846370  10843574  10808386  10809707  10807612  10810108  10794792  10808601  10808285|  10794792 [2084954]
        x17 unrolled: ^C
E:\_KAZE_hash_Yoshimitsu>hash 100MB_as_one_line.TXT
1 lines read
4 elements in the table (2 bits)
           Jesteress:     204418    204277    204025    203698    203971    204613    204024    204038    204817    203649    203986    204830    20
3903    204275    203998    203979    206157    203964    204222    204992    203941    204735    203936    203846    204139    204006    204257
204852    204054    204637    204754    203782    204939    203756    204229    203863    203781    204314    203805    204040|    203649 [    0]
              Meiyan:     207161    203931    204162    204427    204259    204067    204779    203789    204676    204227    204166    204609    20
3947    204061    204948    204129    205497    203990    204197    204580    204167    204019    207285    203850    204626    204024    204115
203722    204069    203956    204686    203754    204179    204763    204100    204783    204213    205179    203876    203880|    203722 [    0]
             Yorikke:     182415    182293    181865    182346    185258    181676    182305    181862    182163    181989    181598    181416    18
2537    182109    181896    181729    182853    181463    183008    181789    181859    182113    182420    181404    182609    182124    181972
181936    184815    181467    182594    182138    182085    181910    181963    181442    182487    183794    181833    182204|    181404 [    0]
          Yoshimitsu:     180939    179829    181113    180314    180039    180551    179739    180056    180797    180264    180280    180123    18
3607    180034    180784    180136    180662    180274    180357    180014    180351    180705    180533    180294    180898    180164    180208
180767    179852    180580    180098    180465    180397    180738    180057    180502    216061    180029    180124    180669|    179739 [    0]
        x17 unrolled:     957197^C
E:\_KAZE_hash_Yoshimitsu>
*/

		//hash32 = ( ( ( (hash32 ^ (_rotl(*(DWORD *)(p+0),5) ^ *(DWORD *)(p+4))) * ((1<<13)-1)) ^ (_rotl(*(DWORD *)(p+8),5)^*(DWORD *)(p+12)) ) * ((1<<13)-1));
		//hash32B = ( ( ( (hash32B ^ (_rotl(*(DWORD *)(p+16),5) ^ *(DWORD *)(p+20))) * ((1<<13)-1)) ^ (_rotl(*(DWORD *)(p+24),5) ^ *(DWORD *)(p+28)) ) * ((1<<13)-1)); // ((1<<13)-1)=8191 prime

		//hash32 = _rotl( ( _rotl( (hash32 ^ (_rotl(*(DWORD *)(p+0),5) ^ *(DWORD *)(p+4))) , 13) ^ (_rotl(*(DWORD *)(p+8),5)^*(DWORD *)(p+12)) ) , 13);
		//hash32B = _rotl( ( _rotl( (hash32B ^ (_rotl(*(DWORD *)(p+16),5) ^ *(DWORD *)(p+20))) , 13) ^ (_rotl(*(DWORD *)(p+24),5) ^ *(DWORD *)(p+28)) ) , 13);
// !!! 62e-5f0+2= 64 bytes !!!
/*
$LL8@Hash_Yoshi:
; 1384 : 		hash32 = _rotl( ( _rotl( (hash32 ^ (_rotl(*(DWORD *)(p+0),5) ^ *(DWORD *)(p+4))) , 13) ^ (_rotl(*(DWORD *)(p+8),5)^*(DWORD *)(p+12)) ) , 13);

  005f0	8b 18		 mov	 ebx, DWORD PTR [eax]
  005f2	c1 c3 05	 rol	 ebx, 5
  005f5	33 58 04	 xor	 ebx, DWORD PTR [eax+4]
  005f8	83 ea 20	 sub	 edx, 32
  005fb	33 de		 xor	 ebx, esi
  005fd	8b 70 08	 mov	 esi, DWORD PTR [eax+8]
  00600	c1 c3 0d	 rol	 ebx, 13
  00603	c1 c6 05	 rol	 esi, 5
  00606	33 f3		 xor	 esi, ebx
  00608	33 70 0c	 xor	 esi, DWORD PTR [eax+12]

; 1385 : 		hash32B = _rotl( ( _rotl( (hash32B ^ (_rotl(*(DWORD *)(p+16),5) ^ *(DWORD *)(p+20))) , 13) ^ (_rotl(*(DWORD *)(p+24),5) ^ *(DWORD *)(p+28)) ) , 13);

  0060b	8b 58 10	 mov	 ebx, DWORD PTR [eax+16]
  0060e	c1 c6 0d	 rol	 esi, 13
  00611	c1 c3 05	 rol	 ebx, 5
  00614	33 58 14	 xor	 ebx, DWORD PTR [eax+20]
  00617	83 c0 20	 add	 eax, 32
  0061a	33 d9		 xor	 ebx, ecx
  0061c	8b 48 f8	 mov	 ecx, DWORD PTR [eax-8]
  0061f	c1 c3 0d	 rol	 ebx, 13
  00622	c1 c1 05	 rol	 ecx, 5
  00625	33 cb		 xor	 ecx, ebx
  00627	33 48 fc	 xor	 ecx, DWORD PTR [eax-4]
  0062a	c1 c1 0d	 rol	 ecx, 13
  0062d	4f		 dec	 edi
  0062e	75 c0		 jne	 SHORT $LL8@Hash_Yoshi
  00630	5b		 pop	 ebx
$LN6@Hash_Yoshi:
*/
    }
	if (p != str) {
		hash32 = (hash32 ^ yoshi_rotl(hash32C,5) ) * PRIME;
		hash32B = (hash32B ^ yoshi_rotl(hash32D,5) ) * PRIME;
	}

    // 1111=15; 10111=23
    if (wrdlen & 4*sizeof(uint32_t)) {	
		hash32 = (hash32 ^ (yoshi_rotl(*(uint32_t *)(p+0),5) ^ *(uint32_t *)(p+4))) * PRIME;        
		hash32B = (hash32B ^ (yoshi_rotl(*(uint32_t *)(p+8),5) ^ *(uint32_t *)(p+12))) * PRIME;        
		p += 8*sizeof(uint16_t);
    }
    // Cases: 0,1,2,3,4,5,6,7,...,15
    if (wrdlen & 2*sizeof(uint32_t)) {
		hash32 = (hash32 ^ *(uint32_t*)(p+0)) * PRIME;
		hash32B = (hash32B ^ *(uint32_t*)(p+4)) * PRIME;
		p += 4*sizeof(uint16_t);
    }
    // Cases: 0,1,2,3,4,5,6,7
    if (wrdlen & sizeof(uint32_t)) {
		hash32 = (hash32 ^ *(uint16_t*)(p+0)) * PRIME;
		hash32B = (hash32B ^ *(uint16_t*)(p+2)) * PRIME;
		p += 2*sizeof(uint16_t);
    }
    if (wrdlen & sizeof(uint16_t)) {
        hash32 = (hash32 ^ *(uint16_t*)p) * PRIME;
        p += sizeof(uint16_t);
    }
    if (wrdlen & 1) 
        hash32 = (hash32 ^ *p) * PRIME;

    hash32 = (hash32 ^ yoshi_rotl(hash32B,5) ) * PRIME;
    return hash32 ^ (hash32 >> 16);
}

// "No High-Speed Limit", says Tesla.
// 
// As a youth, Tesla exhibited a peculiar trait that he considered the basis of all his invention.
// He had an abnormal ability, usually involuntary, to visualize scenes, people, and things so vividly that he was sometimes unsure of what was real and what imaginary.
// Strong flashes of light often accompanied these images.
// Tormented, he would move his hand in front of his eyes to determine whether the objects were simply in his mind or outside.
// He considered the strange ability an affliction at first, but for an inventor it could be a gift.
// Tesla wrote of these phenomena and of his efforts to find an explanation for them, since no psychologist or physiologist was ever able to help him.
// "The theory I have formulated," he wrote much later, is that the images were the result of a reflex action from the brain on the retina under great excitation.
// They certainly were not hallucinations, for in other respects I was normal and composed.
// To give an idea of my distress, suppose that I had witnessed a funeral or some such nerve-wracking spectacle.
// Then, inevitably, in the stillness of the night, a vivid picture of the scene would thrust itself before my eyes and persist despite all my efforts to banish it.
// Sometimes it would even remain fixed in space though I pushed my hand through it.
// /Excerpts from 'Tesla: Master of Lightning', by Margaret Cheney and Robert Uth/
// 
// "It is not the time," said Dr. Tesla yesterday, "to go into the details of this thing.
// It is founded on a principle that means great things in peace, it can be used for great things in war.
// But I repeat, this is no time to talk of such things."
// /New York Times, Dec. 8, 1915/
// 
// On the occasion of his 75th birthday, Tesla talked about new developments.
// "I am working now upon two things," he said.
// "First, an explanation based upon pure mathematics of certain things which Professor Einstein has also attempted to explain.
// My conclusions in certain respects differ from and to that extent tend to disprove the Einstein Theory . . . 
// My explanations of natural phenomena are not so involved as his.
// They are simpler, and when I am ready to make a full announcement it will be seen that I have proved my conclusions."
// "Secondly, I am working to develop a new source of power.
// When I say a new source, I mean that I have turned for power to a source which no previous scientist has turned, to the best of my knowledge.
// The conception, the idea when it first burst upon me was a tremendous shock."
// "It will throw light on many puzzling phenomena of the cosmos, and may prove also of great industrial value, particularly in creating a new and virtually unlimited market for steel."
// Tesla said it will come from an entirely new and unsuspected source, and will be for all practical purposes constant day and night, and at all times of the year.
// The apparatus for capturing the energy and transforming it will partake both of mechanical and electrical features, and will be of ideal simplicity.
// Tesla has already conceived a means that will make it possible for man to transmit energy in large amounts, thousands of horsepower, from one planet to another, absolutely regardless of distance.
// He considered that nothing can be more important than interplanetary communication.
// It will certainly come some day, and the certitude that there are other human beings in the universe, working, suffering, struggling, like ourselves, will produce a magic effect on mankind and will form the foundation of a universal brotherhood that will last as long as humanity itself.
// /Time, July 20, 1931/
// 
// Dr. Nikola Tesla asserted in an interview with Hugo Gernsback that speeds greater than that of light, which are considered impossible by the Einstein theory of relativity, have been produced.
// Stating that the Einstein theory is erroneous in many respects, Dr. Tesla stated as early as 1900, in his patent 787,412, that the current of his radio-power transmitter passed over the surface of the earth with a speed of 292,830 miles a second.
// According to the Einstein theory, the highest possible speed is 186,300 miles a second.
// Tesla indicated knowledge of speeds several times greater than light, and had apparatus designed to project so-called electrons with a speed equal to twice that of light.
// /The Literary Digest, Nov. 7, 1931/
// 
// He had discovered the so-called cosmic ray in 1896, at least five years before any other scientist took it up and twenty years before it became popular among scientists, and he is now convinced that many of the cosmic particles travel fifty times faster than light, some of them 500 times faster.
// /New York World-Telegram, July 11, 1935/
 
// Revision 2 (main mix should be refined), written 2012-Jun-16, thanks to Maciej Adamczyk for his testbed and openness.
 
#define ROL64(x, n) (((x) << (n)) | ((x) >> (64-(n))))
#define ROL(x, n) (((x) << (n)) | ((x) >> (32-(n))))
uint64_t FNV1A_Hash_Tesla(const char *str, size_t wrdlen, uint32_t seed)
{
	const uint64_t PRIME = 11400714819323198393ULL;
	uint64_t hash64 = 2166136261U+seed; // should be much bigger
	uint64_t hash64B = 2166136261U+seed; // should be much bigger
	const char *p = str;
	
	for(; wrdlen >= 2*2*2*sizeof(uint32_t); wrdlen -= 2*2*2*sizeof(uint32_t), p += 2*2*2*sizeof(uint32_t)) {
		hash64 = (hash64 ^ (ROL64(*(unsigned long long *)(p+0),5-0)^*(unsigned long long *)(p+8))) * PRIME;
		hash64B = (hash64B ^ (ROL64(*(unsigned long long *)(p+8+8),5-0)^*(unsigned long long *)(p+8+8+8))) * PRIME;
	}

        hash64 = (hash64 ^ hash64B); // Some mix, the simplest is given, maybe the B-line should be rolled by 32bits before xoring.

	// Cases: 0,1,2,3,4,5,6,7,... 15,... 31
	if (wrdlen & (2*2*sizeof(uint32_t))) {
		hash64 = (hash64 ^ (ROL(*(uint32_t *)(p+0),5-0)^*(uint32_t *)(p+4))) * PRIME;		
		hash64 = (hash64 ^ (ROL(*(uint32_t *)(p+4+4),5-0)^*(uint32_t *)(p+4+4+4))) * PRIME;		
		p += 2*2*sizeof(uint32_t);
	}
	if (wrdlen & (2*sizeof(uint32_t))) {
		hash64 = (hash64 ^ (ROL(*(uint32_t *)p,5-0)^*(uint32_t *)(p+4))) * PRIME;		
		p += 2*sizeof(uint32_t);
	}
	if (wrdlen & sizeof(uint32_t)) {
		hash64 = (hash64 ^ *(uint32_t*)p) * PRIME;
		p += sizeof(uint32_t);
	}
	if (wrdlen & sizeof(uint16_t)) {
		hash64 = (hash64 ^ *(uint16_t*)p) * PRIME;
		p += sizeof(uint16_t);
	}
	if (wrdlen & 1) 
		hash64 = (hash64 ^ *p) * PRIME;
	
	return hash64 ^ (hash64 >> 32);
}

// The jester as a symbol:
// The root of the word "fool" is from the Latin follis, which means "bag of wind" or that which contains air or breath.
// In Tarot, "The Fool" is the first card of the Major Arcana.
// The tarot depiction of the Fool includes a man, (or less often, a woman), juggling unconcernedly or otherwise distracted, with a dog (sometimes cat) at his heels.
// The fool is in the act of unknowingly walking off the edge of a cliff, precipice or other high place.
// Another Tarot character is Death. In the Middle Ages Death is often shown in Jester's garb because "The last laugh is reserved for death." Also, Death humbles everyone just as Jesters make fun of everyone regardless of standing.
// In literature, the jester is symbolic of common sense and of honesty, notably in King Lear, the court jester is a character used for insight and advice on the part of the monarch, taking advantage of his license to mock and speak freely to dispense frank observations and highlight the folly of his monarch.
// This presents a clashing irony as a "greater" man could dispense the same advice and find himself being detained in the dungeons or even executed. Only as the lowliest member of the court can the jester be the monarch's most useful adviser.
// Distinction was made between fools and clowns, or country bumpkins.
// The fool's status was one of privilege within a royal or noble household.
// His folly could be regarded as the raving of a madman but was often deemed to be divinely inspired.
// The 'natural' fool was touched by God. Much to Gonerill's annoyance, Lear's 'all-licensed' Fool enjoys a privileged status. His characteristic idiom suggests he is a 'natural' fool, not an artificial one, though his perceptiveness and wit show that he is far from being an idiot, however 'touched' he might be.
// The position of the Joker playing card, as a wild card which has no fixed place in the hierarchy of King, Queen, Knave, etc. might be a remnant of the position of the court jester.
// This lack of any place in the hierarchy meant Kings could trust the counsel of the jesters, as they had no vested interest in any region, estate or church.
// Source: en.wikipedia.org
#define ROL(x, n) (((x) << (n)) | ((x) >> (32-(n))))
uint32_t FNV1A_Hash_Jesteress(const char *str, size_t wrdlen, uint32_t seed)
{
    const uint32_t PRIME = 709607;
    uint32_t hash32 = 2166136261+seed;
    const char *p = str;

    // Idea comes from Igor Pavlov's 7zCRC, thanks.
/*
    for(; wrdlen && ((unsigned)(ptrdiff_t)p&3); wrdlen -= 1, p++) {
        hash32 = (hash32 ^ *p) * PRIME;
    }
*/
    for(; wrdlen >= 2*sizeof(uint32_t); wrdlen -= 2*sizeof(uint32_t), p += 2*sizeof(uint32_t)) {
        hash32 = (hash32 ^ (ROL(*(uint32_t *)p,5)^*(uint32_t *)(p+4))) * PRIME;        
    }
    // Cases: 0,1,2,3,4,5,6,7
    if (wrdlen & sizeof(uint32_t)) {
        hash32 = (hash32 ^ *(uint32_t*)p) * PRIME;
        p += sizeof(uint32_t);
    }
    if (wrdlen & sizeof(uint32_t)) {
        hash32 = (hash32 ^ *(uint16_t*)p) * PRIME;
        p += sizeof(uint16_t);
    }
    if (wrdlen & 1) 
        hash32 = (hash32 ^ *p) * PRIME;
    
    return hash32 ^ (hash32 >> 16);
}

// Meiyan means Beauty, Charming Eyes or most precisely: SOULFUL EYES.
uint32_t FNV1A_Hash_Meiyan(const char *str, size_t wrdlen, uint32_t seed)
{
    const uint32_t PRIME = 709607;
    uint32_t hash32 = 2166136261+seed;
    const char *p = str;

    // Idea comes from Igor Pavlov's 7zCRC, thanks.
/*
    for(; wrdlen && ((unsigned)(ptrdiff_t)p&3); wrdlen -= 1, p++) {
        hash32 = (hash32 ^ *p) * PRIME;
    }
*/
    for(; wrdlen >= 2*sizeof(uint32_t); wrdlen -= 2*sizeof(uint32_t), p += 2*sizeof(uint32_t)) {
        hash32 = (hash32 ^ (ROL(*(uint32_t *)p,5)^*(uint32_t *)(p+4))) * PRIME;        
    }
    // Cases: 0,1,2,3,4,5,6,7
    if (wrdlen & sizeof(uint32_t)) {
//		hash32 = (hash32 ^ *(DWORD*)p) * PRIME;
//		p += sizeof(DWORD);
		hash32 = (hash32 ^ *(uint16_t*)p) * PRIME;
		p += sizeof(uint16_t);
		hash32 = (hash32 ^ *(uint16_t*)p) * PRIME;
		p += sizeof(uint16_t);        
    }
    if (wrdlen & sizeof(uint16_t)) {
        hash32 = (hash32 ^ *(uint16_t*)p) * PRIME;
        p += sizeof(uint16_t);
    }
    if (wrdlen & 1) 
        hash32 = (hash32 ^ *p) * PRIME;
    
    return hash32 ^ (hash32 >> 16);
}

// Mantis has two(three to be exact) gears: it operates as WORD based FNV1A for 1..15 lengths and as QWORD based FNV1A 16.. lengths.
// I see the instant mantis' grasping-and-devouring as MONSTROUS QUADRO-BYTE-PAIRs BAITs(IN-MIX) while target secured within FIRM-GRIP of forelimbs(PRE-MIX & POST-MIX).
// Word 'mantical'(Of or relating to the foretelling of events by or as if by supernatural means) comes from Greek mantikos, from the Greek word mantis, meaning "prophet, seer."
// The Greeks, who made the connection between the upraised front legs of a mantis waiting for its prey and the hands of a prophet in prayer, used the name mantis to mean "the praying mantis."
uint32_t FNV1A_Hash_Mantis(const char *str, size_t wrdlen, uint32_t seed)
{
	const uint32_t PRIME = 709607;
	uint32_t hash32 = 2166136261+seed;
	const char *p = str;
	// Cases: 0,1,2,3,4,5,6,7
	if (wrdlen & sizeof(uint32_t)) {
		hash32 = (hash32 ^ *(uint16_t*)p) * PRIME;
		p += sizeof(uint16_t);
		hash32 = (hash32 ^ *(uint16_t*)p) * PRIME;
		p += sizeof(uint16_t);
		//wrdlen -= sizeof(DWORD);
	}
	if (wrdlen & sizeof(uint16_t)) {
		hash32 = (hash32 ^ *(uint16_t*)p) * PRIME;
		p += sizeof(uint16_t);
		//wrdlen -= sizeof(WORD);
	}
	if (wrdlen & 1) {
		hash32 = (hash32 ^ *p) * PRIME;
		p += sizeof(char);
		//wrdlen -= sizeof(char);
	}
		wrdlen -= p-str;
// The goal is to avoid the weak range [8, 8+2, 8+1] that is 8..10 in practice 1..15 i.e. 1..8+4+2+1, thus amending FNV1A_Meiyan and FNV1A_Jesteress.
// FNV1A_Jesteress: fastest strong
// FNV1A_Meiyan   : faster  stronger
// FNV1A_Mantis   : fast    strongest
	if (wrdlen) {
	for(; wrdlen > 2*sizeof(uint32_t); wrdlen -= 2*sizeof(uint32_t), p += 2*sizeof(uint32_t)) {
		hash32 = (hash32 ^ (ROL(*(uint32_t *)p,5)^*(uint32_t *)(p+4))) * PRIME;		
	}
		hash32 = (hash32 ^ *(uint16_t*)(p+0*sizeof(uint16_t))) * PRIME;
		hash32 = (hash32 ^ *(uint16_t*)(p+1*sizeof(uint16_t))) * PRIME;
		hash32 = (hash32 ^ *(uint16_t*)(p+2*sizeof(uint16_t))) * PRIME;
		hash32 = (hash32 ^ *(uint16_t*)(p+3*sizeof(uint16_t))) * PRIME;
	} // Bug Fixed!        
	return hash32 ^ (hash32 >> 16);
}

// "There it now stands for ever. Black on white.
// I can't get away from it. Ahoy, Yorikke, ahoy, hoy, ho!
// Go to hell now if you wish. What do I care? It's all the same now to me.
// I am part of you now. Where you go I go, where you leave I leave, when you go to the devil I go. Married.
// Vanished from the living. Damned and doomed. Of me there is not left a breath in all the vast world.
// Ahoy, Yorikke! Ahoy, hoy, ho!
// I am not buried in the sea,
// The death ship is now part of me
// So far from sunny New Orleans
// So far from lovely Louisiana."
// /An excerpt from 'THE DEATH SHIP - THE STORY OF AN AMERICAN SAILOR' by B.TRAVEN/
// 
// "Walking home to our good old Yorikke, I could not help thinking of this beautiful ship, with a crew on board that had faces as if they were seeing ghosts by day and by night.
// Compared to that gilded Empress, the Yorikke was an honorable old lady with lavender sachets in her drawers.
// Yorikke did not pretend to anything she was not. She lived up to her looks. Honest to her lowest ribs and to the leaks in her bilge.
// Now, what is this? I find myself falling in love with that old jane.
// All right, I cannot pass by you, Yorikke; I have to tell you I love you. Honest, baby, I love you.
// I have six black finger-nails, and four black and green-blue nails on my toes, which you, honey, gave me when necking you.
// Grate-bars have crushed some of my toes. And each finger-nail has its own painful story to tell.
// My chest, my back, my arms, my legs are covered with scars of burns and scorchings.
// Each scar, when it was being created, caused me pains which I shall surely never forget.
// But every outcry of pain was a love-cry for you, honey.
// You are no hypocrite. Your heart does not bleed tears when you do not feel heart-aches deeply and truly.
// You do not dance on the water if you do not feel like being jolly and kicking chasers in the pants.
// Your heart never lies. It is fine and clean like polished gold. Never mind the rags, honey dear.
// When you laugh, your whole soul and all your body is laughing.
// And when you weep, sweety, then you weep so that even the reefs you pass feel like weeping with you.
// I never want to leave you again, honey. I mean it. Not for all the rich and elegant buckets in the world.
// I love you, my gypsy of the sea!"
// /An excerpt from 'THE DEATH SHIP - THE STORY OF AN AMERICAN SAILOR' by B.TRAVEN/
uint32_t FNV1A_Hash_Yorikke(const char *str, size_t wrdlen, uint32_t seed)
{
    const uint32_t PRIME = 709607;
    uint32_t hash32 = 2166136261+seed;
    uint32_t hash32B = 2166136261+seed;
    const char *p = str;

    for(; wrdlen >= 2*2*sizeof(uint32_t); wrdlen -= 2*2*sizeof(uint32_t), p += 2*2*sizeof(uint32_t)) {
        hash32 = (hash32 ^ (yoshi_rotl(*(uint32_t *)(p+0),5) ^ *(uint32_t *)(p+4))) * PRIME;        
        hash32B = (hash32B ^ (yoshi_rotl(*(uint32_t *)(p+8),5) ^ *(uint32_t *)(p+12))) * PRIME;        
    }

    // Cases: 0,1,2,3,4,5,6,7,...,15
    if (wrdlen & 2*sizeof(uint32_t)) {
		hash32 = (hash32 ^ *(uint32_t*)(p+0)) * PRIME;
		hash32B = (hash32B ^ *(uint32_t*)(p+4)) * PRIME;
		p += 4*sizeof(uint16_t);
    }
    // Cases: 0,1,2,3,4,5,6,7
    if (wrdlen & sizeof(uint32_t)) {
		hash32 = (hash32 ^ *(uint16_t*)(p+0)) * PRIME;
		hash32B = (hash32B ^ *(uint16_t*)(p+2)) * PRIME;
		p += 2*sizeof(uint16_t);
    }
    if (wrdlen & sizeof(uint16_t)) {
        hash32 = (hash32 ^ *(uint16_t*)p) * PRIME;
        p += sizeof(uint16_t);
    }
    if (wrdlen & 1) 
        hash32 = (hash32 ^ *p) * PRIME;

    hash32 = (hash32 ^ yoshi_rotl(hash32B,5) ) * PRIME;
    return hash32 ^ (hash32 >> 16);
}

