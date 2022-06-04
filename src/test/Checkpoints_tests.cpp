// Copyright (c) 2011-2013 The Bitcoin Core developers
// Copyright (c) 2017-2020 The PIVX developers
// Copyright (c) 2022 The PCOIN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Unit tests for block-chain checkpoints
//

#include "checkpoints.h"

#include "uint256.h"
#include "test_pcoin.h"

#include <boost/test/unit_test.hpp>


BOOST_FIXTURE_TEST_SUITE(Checkpoints_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(sanity)
{
    uint256 p259201 = uint256S("0x001");
    uint256 p623933 = uint256S("0x001");
    BOOST_CHECK(Checkpoints::CheckBlock(1, p259201));
    BOOST_CHECK(Checkpoints::CheckBlock(2, p623933));


    // Wrong hashes at checkpoints should fail:
    BOOST_CHECK(!Checkpoints::CheckBlock(1, p623933));
    BOOST_CHECK(!Checkpoints::CheckBlock(2, p259201));

    // ... but any hash not at a checkpoint should succeed:
    BOOST_CHECK(Checkpoints::CheckBlock(1+1, p623933));
    BOOST_CHECK(Checkpoints::CheckBlock(2+1, p259201));

    BOOST_CHECK(Checkpoints::GetTotalBlocksEstimate() >= 100);
}

BOOST_AUTO_TEST_SUITE_END()
