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
    // Construct the stakeinput object
    if (!txin.IsZerocoinSpend()) {
        LogPrintf("%s: unable to initialize CLegacyZPcoinStake from non zc-spend", __func__);
        return nullptr;
    }

    // Disable zPOS
    const Consensus::Params& consensus = Params().GetConsensus();
    if (!consensus.NetworkUpgradeActive(nHeight, Consensus::BASE_NETWORK)) {
        LogPrint(BCLog::LEGACYZC, "%s : zPCOIN stake block: height %d outside range", __func__, nHeight);
        return nullptr;
    }

    // Check spend type
    libzerocoin::CoinSpend spend = ZPCOINModule::TxInToZerocoinSpend(txin);
    if (spend.getSpendType() != libzerocoin::SpendType::STAKE) {
        LogPrintf("%s : spend is using the wrong SpendType (%d)", __func__, (int)spend.getSpendType());
        return nullptr;
    }

    uint32_t _nChecksum = spend.getAccumulatorChecksum();
    libzerocoin::CoinDenomination _denom = spend.getDenomination();
    const arith_uint256& nSerial = spend.getCoinSerialNumber().getuint256();
    const uint256& _hashSerial = Hash(nSerial.begin(), nSerial.end());

    // The checkpoint needs to be from 200 blocks ago
    const int cpHeight = nHeight - 1 - consensus.ZC_MinStakeDepth;
    if (ParseAccChecksum(chainActive[cpHeight]->nAccumulatorCheckpoint, _denom) != _nChecksum) {
        LogPrint(BCLog::LEGACYZC, "%s : accum. checksum at height %d is wrong.", __func__, nHeight);
    }

    // Find the pindex of the first block with the accumulator checksum
    const CBlockIndex* _pindexFrom = FindIndexFrom(_nChecksum, _denom, cpHeight);
    if (_pindexFrom == nullptr) {
        LogPrintf("%s : Failed to find the block index for zpcoin stake origin", __func__);
        return nullptr;
    }

    // All good
    return new CLegacyZPcoinStake(_pindexFrom, _nChecksum, _denom, _hashSerial);
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
