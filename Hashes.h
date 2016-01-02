#pragma once

#include "Types.h"

#include "MurmurHash1.h"
#include "MurmurHash2.h"
#include "MurmurHash3.h"

#include "xxhash.h"

//----------
// These are _not_ hash functions (even though people tend to use crc32 as one...)

void sumhash               ( const void * key, int len, uint32_t seed, void * out );
void sumhash32             ( const void * key, int len, uint32_t seed, void * out );

void DoNothingHash         ( const void * key, int len, uint32_t seed, void * out );
void crc32                 ( const void * key, int len, uint32_t seed, void * out );

extern		"C" {
  uint32_t	  crc32c_hw(const void *input, int len, uint32_t seed);
  uint64_t	  crc64c_hw(const void *input, int len, uint32_t seed);
  uint32_t fletcher32(const void *buf1, size_t len, uint32_t fletcher);
  uint32_t fletcher32_4(const void *buf1, size_t len, uint32_t fletcher);
}
void crc32c_hw_test        ( const void * key, int len, uint32_t seed, void * out );
void crc64c_hw_test        ( const void * key, int len, uint32_t seed, void * out );


void fletcher32_test        ( const void * key, int len, uint32_t seed, void * out );
void fletcher32_4_test      ( const void * key, int len, uint32_t seed, void * out );

void randhash_32           ( const void * key, int len, uint32_t seed, void * out );
void randhash_64           ( const void * key, int len, uint32_t seed, void * out );
void randhash_128          ( const void * key, int len, uint32_t seed, void * out );

//----------
// Cryptographic hashes

void md5_32                ( const void * key, int len, uint32_t seed, void * out );
void sha1_32a              ( const void * key, int len, uint32_t seed, void * out );

//----------
// General purpose hashes

void FNV                   ( const void * key, int len, uint32_t seed, void * out );
void FNV32a(const void *key, int len, uint32_t seed, void *out);
void FNV64a(const void *key, int len, uint32_t seed, void *out);
void Bernstein             ( const void * key, int len, uint32_t seed, void * out );
void SuperFastHash         ( const void * key, int len, uint32_t seed, void * out );
void lookup3_test          ( const void * key, int len, uint32_t seed, void * out );
void MurmurOAAT_test       ( const void * key, int len, uint32_t seed, void * out );
void Crap8_test            ( const void * key, int len, uint32_t seed, void * out );
void CityHash128_test      ( const void * key, int len, uint32_t seed, void * out );
void CityHash64_test       ( const void * key, int len, uint32_t seed, void * out );
void sdbm                  (const void *key, int len, uint32_t hash, void *out);
void SpookyHash32_test     ( const void * key, int len, uint32_t seed, void * out );
void SpookyHash64_test     ( const void * key, int len, uint32_t seed, void * out );
void SpookyHash128_test    ( const void * key, int len, uint32_t seed, void * out );

uint32_t MurmurOAAT ( const void * key, int len, uint32_t seed );

/* Sanmayce */
void FNV1A_penumbra        ( const void * key, int len, uint32_t seed, void * out);

uint32_t FNV1A_Hash_Yoshimura(const char *str, uint32_t wrdlen, uint32_t seed);
inline void FNV1A_Hash_Yoshimura_test  ( const void * key, int len, uint32_t seed, void * out)
{
    *(uint32_t*)out = FNV1A_Hash_Yoshimura((const char*)key, len, seed);
}
uint32_t FNV1A_Hash_Yoshimitsu(const char *str, uint32_t wrdlen, uint32_t seed);
inline void FNV1A_Hash_Yoshimitsu_test ( const void * key, int len, uint32_t seed, void * out)
{
    *(uint32_t*)out = FNV1A_Hash_Yoshimitsu((const char*)key, len, seed);
}
uint32_t FNV1A_Hash_Jesteress(const char *str, size_t wrdlen, uint32_t seed);
inline void FNV1A_Hash_Jesteress_test  ( const void * key, int len, uint32_t seed, void * out)
{
    *(uint32_t*)out = FNV1A_Hash_Jesteress((const char *)key, len, seed);
}
uint32_t FNV1A_Hash_Meiyan(const char *str, size_t wrdlen, uint32_t seed);
inline void FNV1A_Hash_Meiyan_test   ( const void * key, int len, uint32_t seed, void * out)
{
    *(uint32_t*)out = FNV1A_Hash_Meiyan((const char*)key, len, seed);
}
uint32_t FNV1A_Hash_Mantis(const char *str, size_t wrdlen, uint32_t seed);
inline void FNV1A_Hash_Mantis_test   ( const void * key, int len, uint32_t seed, void * out)
{
    *(uint32_t*)out = FNV1A_Hash_Mantis((const char*)key, len, seed);
}
uint32_t FNV1A_Hash_Yorikke(const char *str, size_t wrdlen, uint32_t seed);
inline void FNV1A_Hash_Yorikke_test   ( const void * key, int len, uint32_t seed, void * out)
{
    *(uint32_t*)out = FNV1A_Hash_Yorikke((const char*)key, len, seed);
}
void FNV32a_YoshimitsuTRIAD(const void *key, int len, uint32_t seed, void *out);

/* XXHash */
inline void xxhash32_test  ( const void * key, int len, uint32_t seed, void * out)
{
    *(uint32_t*)out = XXH32(key,len,seed);
}
inline void xxhash64_test  ( const void * key, int len, uint32_t seed, void * out)
{
    *(uint64_t*)out = XXH64(key,len,seed);
}
extern "C" __declspec(dllexport) void XXH_256(const void* input, size_t len, uint32_t seed, uint64_t* out);
inline void xxhash256_test( const void * key, int len, uint32_t seed, void * out)
{
    XXH_256(key, len, seed, (uint64_t*)out);
}

/* Jesse Towner's memhash as seen here: http://locklessinc.com/articles/fast_hash/ */
extern "C" __declspec(dllexport) uint64_t memhash(void const* key, size_t n, uint64_t seed);
inline void memhash32_test ( const void * key, int len, uint32_t seed, void * out)
{
    *(uint64_t*)out = memhash(key,len,seed);
}

/* falkhash */
extern "C" __declspec(dllexport) __m128i falkhash(_In_reads_bytes_(len) void *pbuf, _In_ uint64_t  len, _In_ uint64_t  pseed);
inline void falkhash_test   ( const void * key, int len, uint32_t seed, void * out)
{
    uint64_t hash[2];
    __m128i val;
    
    if (!len) {
        *(uint32_t *) out = 0;
        return;
    }
    val = falkhash((uint8_t *)key, (uint64_t)len, seed);
    _mm_storeu_si128((__m128i*)hash, val);
    *(uint64_t *) out = hash[0];
}

//----------
// MurmurHash2

void MurmurHash2_test      ( const void * key, int len, uint32_t seed, void * out );
void MurmurHash2A_test     ( const void * key, int len, uint32_t seed, void * out );

//-----------------------------------------------------------------------------
// Test harnesses for Murmur1/2

inline void MurmurHash1_test ( const void * key, int len, uint32_t seed, void * out )
{
  *(uint32_t*)out = MurmurHash1(key,len,seed);
}

inline void MurmurHash2_test ( const void * key, int len, uint32_t seed, void * out )
{
  *(uint32_t*)out = MurmurHash2(key,len,seed);
}

inline void MurmurHash2A_test ( const void * key, int len, uint32_t seed, void * out )
{
  *(uint32_t*)out = MurmurHash2A(key,len,seed);
}

inline void MurmurHash64A_test ( const void * key, int len, uint32_t seed, void * out )
{
  *(uint64_t*)out = MurmurHash64A(key,len,seed);
}

inline void MurmurHash64B_test ( const void * key, int len, uint32_t seed, void * out )
{
  *(uint64_t*)out = MurmurHash64B(key,len,seed);
}
