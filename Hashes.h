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
void Bernstein             ( const void * key, int len, uint32_t seed, void * out );
void SuperFastHash         ( const void * key, int len, uint32_t seed, void * out );
void lookup3_test          ( const void * key, int len, uint32_t seed, void * out );
void MurmurOAAT_test       ( const void * key, int len, uint32_t seed, void * out );
void Crap8_test            ( const void * key, int len, uint32_t seed, void * out );
void CityHash128_test      ( const void * key, int len, uint32_t seed, void * out );
void CityHash64_test       ( const void * key, int len, uint32_t seed, void * out );

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


/* XXHash */
inline void xxhash32_test  ( const void * key, int len, uint32_t seed, void * out)
{
    *(uint32_t*)out = XXH32(key,len,seed);
}
inline void xxhash64_test  ( const void * key, int len, uint32_t seed, void * out)
{
    *(uint64_t*)out = XXH64(key,len,seed);
}

/* Jesse Towner's memhash as seen here: http://locklessinc.com/articles/fast_hash/ */
extern "C" __declspec(dllexport) uint64_t memhash(void const* key, size_t n, uint64_t seed);
inline void memhash32_test ( const void * key, int len, uint32_t seed, void * out)
{
    *(uint64_t*)out = memhash(key,len,seed);
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
