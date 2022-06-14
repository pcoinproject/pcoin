// Copyright (c) 2017-2020 The PIVX developers
// Copyright (c) 2022 The PCOIN developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "zpcoin/zpos.h"

#include "validation.h"
#include "zpcoin/zpcoinmodule.h"


/*
 * LEGACY: Kept for IBD in order to verify zerocoin stakes occurred when zPoS was active
 * Find the first occurrence of a certain accumulator checksum.
 * Return block index pointer or nullptr if not found
 */

uint32_t ParseAccChecksum(uint256 nCheckpoint, const libzerocoin::CoinDenomination denom)
{
    int pos = std::distance(libzerocoin::zerocoinDenomList.begin(),
            find(libzerocoin::zerocoinDenomList.begin(), libzerocoin::zerocoinDenomList.end(), denom));
    return (UintToArith256(nCheckpoint) >> (32*((libzerocoin::zerocoinDenomList.size() - 1) - pos))).Get32();
}

static const CBlockIndex* FindIndexFrom(uint32_t nChecksum, libzerocoin::CoinDenomination denom, int cpHeight)
{
    return nullptr;
}

CLegacyZPcoinStake* CLegacyZPcoinStake::NewZPcoinStake(const CTxIn& txin, int nHeight)
{
    return nullptr;
}

const CBlockIndex* CLegacyZPcoinStake::GetIndexFrom() const
{
    return pindexFrom;
}

CAmount CLegacyZPcoinStake::GetValue() const
{
    return denom * COIN;
}

CDataStream CLegacyZPcoinStake::GetUniqueness() const
{
    CDataStream ss(SER_GETHASH, 0);
    ss << hashSerial;
    return ss;
}
