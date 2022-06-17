// Copyright (c) 2012-2013 The Bitcoin Core developers
// Copyright (c) 2017-2019 The PIVX developers
// Copyright (c) 2022 The PCOIN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "key.h"

#include "base58.h"
#include "key_io.h"
#include "uint256.h"
#include "util/system.h"
#include "utilstrencodings.h"
#include "test_pcoin.h"

#include <string>
#include <vector>

#include <boost/test/unit_test.hpp>


static const std::string strSecret1  = "87hkEaaCSp85sS6ga6EkSQ5jAgenARrsbGdYLZJ88uLpnD5Fhkn";
static const std::string strSecret2  = "88EuBx4D7H9ujWF7gMALTMtB3jAyYoMSMvxuPtGbrmvoKamkCno";
static const std::string strSecret1C = "YQaqpwBXWvzGVbn5f47gj6ss7k1kLcj8JFzHKKYaVt2Vt52muUaK";
static const std::string strSecret2C = "YSxMML1ASiJKkc5JUfzCsMAsM392bMbCwPBkG1HrrhJTSYD847Z3";
static const std::string addr1 = "PBrgE6EEJitJkfA8GJAsw9VAKM3b91sij7";
static const std::string addr2 = "PBAhmbyphYHovHKXC1M5efNr6DWUxxcxBK";
static const std::string addr1C = "PJX8v7AdrFoL3Qb13qPNb46s1E7zut241e";
static const std::string addr2C = "PVNjCw7HsQ6cCyu1A48YdM9gJDkdEb2gRc";


static const std::string strAddressBad ="Xta1praZQjyELweyMByXyiREw1ZRsjXzVP";


BOOST_FIXTURE_TEST_SUITE(key_tests, TestingSetup)

BOOST_AUTO_TEST_CASE(key_test1)
{
    CKey key1  = KeyIO::DecodeSecret(strSecret1);
    BOOST_CHECK(key1.IsValid() && !key1.IsCompressed());
    CKey key2  = KeyIO::DecodeSecret(strSecret2);
    BOOST_CHECK(key2.IsValid() && !key2.IsCompressed());
    CKey key1C = KeyIO::DecodeSecret(strSecret1C);
    BOOST_CHECK(key1C.IsValid() && key1C.IsCompressed());
    CKey key2C = KeyIO::DecodeSecret(strSecret2C);
    BOOST_CHECK(key2C.IsValid() && key2C.IsCompressed());
    CKey bad_key = KeyIO::DecodeSecret(strAddressBad);
    BOOST_CHECK(!bad_key.IsValid());

    CPubKey pubkey1  = key1. GetPubKey();
    CPubKey pubkey2  = key2. GetPubKey();
    CPubKey pubkey1C = key1C.GetPubKey();
    CPubKey pubkey2C = key2C.GetPubKey();

    BOOST_CHECK(key1.VerifyPubKey(pubkey1));
    BOOST_CHECK(!key1.VerifyPubKey(pubkey1C));
    BOOST_CHECK(!key1.VerifyPubKey(pubkey2));
    BOOST_CHECK(!key1.VerifyPubKey(pubkey2C));

    BOOST_CHECK(!key1C.VerifyPubKey(pubkey1));
    BOOST_CHECK(key1C.VerifyPubKey(pubkey1C));
    BOOST_CHECK(!key1C.VerifyPubKey(pubkey2));
    BOOST_CHECK(!key1C.VerifyPubKey(pubkey2C));

    BOOST_CHECK(!key2.VerifyPubKey(pubkey1));
    BOOST_CHECK(!key2.VerifyPubKey(pubkey1C));
    BOOST_CHECK(key2.VerifyPubKey(pubkey2));
    BOOST_CHECK(!key2.VerifyPubKey(pubkey2C));

    BOOST_CHECK(!key2C.VerifyPubKey(pubkey1));
    BOOST_CHECK(!key2C.VerifyPubKey(pubkey1C));
    BOOST_CHECK(!key2C.VerifyPubKey(pubkey2));
    BOOST_CHECK(key2C.VerifyPubKey(pubkey2C));

    BOOST_CHECK(DecodeDestination(addr1)  == CTxDestination(pubkey1.GetID()));
    BOOST_CHECK(DecodeDestination(addr2)  == CTxDestination(pubkey2.GetID()));
    BOOST_CHECK(DecodeDestination(addr1C) == CTxDestination(pubkey1C.GetID()));
    BOOST_CHECK(DecodeDestination(addr2C) == CTxDestination(pubkey2C.GetID()));

    for (int n=0; n<16; n++)
    {
        std::string strMsg = strprintf("Very secret message %i: 11", n);
        uint256 hashMsg = Hash(strMsg.begin(), strMsg.end());

        // normal signatures

        std::vector<unsigned char> sign1, sign2, sign1C, sign2C;

        BOOST_CHECK(key1.Sign (hashMsg, sign1));
        BOOST_CHECK(key2.Sign (hashMsg, sign2));
        BOOST_CHECK(key1C.Sign(hashMsg, sign1C));
        BOOST_CHECK(key2C.Sign(hashMsg, sign2C));

        BOOST_CHECK( pubkey1.Verify(hashMsg, sign1));
        BOOST_CHECK(!pubkey1.Verify(hashMsg, sign2));
        BOOST_CHECK( pubkey1.Verify(hashMsg, sign1C));
        BOOST_CHECK(!pubkey1.Verify(hashMsg, sign2C));

        BOOST_CHECK(!pubkey2.Verify(hashMsg, sign1));
        BOOST_CHECK( pubkey2.Verify(hashMsg, sign2));
        BOOST_CHECK(!pubkey2.Verify(hashMsg, sign1C));
        BOOST_CHECK( pubkey2.Verify(hashMsg, sign2C));

        BOOST_CHECK( pubkey1C.Verify(hashMsg, sign1));
        BOOST_CHECK(!pubkey1C.Verify(hashMsg, sign2));
        BOOST_CHECK( pubkey1C.Verify(hashMsg, sign1C));
        BOOST_CHECK(!pubkey1C.Verify(hashMsg, sign2C));

        BOOST_CHECK(!pubkey2C.Verify(hashMsg, sign1));
        BOOST_CHECK( pubkey2C.Verify(hashMsg, sign2));
        BOOST_CHECK(!pubkey2C.Verify(hashMsg, sign1C));
        BOOST_CHECK( pubkey2C.Verify(hashMsg, sign2C));

        // compact signatures (with key recovery)

        std::vector<unsigned char> csign1, csign2, csign1C, csign2C;

        BOOST_CHECK(key1.SignCompact (hashMsg, csign1));
        BOOST_CHECK(key2.SignCompact (hashMsg, csign2));
        BOOST_CHECK(key1C.SignCompact(hashMsg, csign1C));
        BOOST_CHECK(key2C.SignCompact(hashMsg, csign2C));

        CPubKey rkey1, rkey2, rkey1C, rkey2C;

        BOOST_CHECK(rkey1.RecoverCompact (hashMsg, csign1));
        BOOST_CHECK(rkey2.RecoverCompact (hashMsg, csign2));
        BOOST_CHECK(rkey1C.RecoverCompact(hashMsg, csign1C));
        BOOST_CHECK(rkey2C.RecoverCompact(hashMsg, csign2C));

        BOOST_CHECK(rkey1  == pubkey1);
        BOOST_CHECK(rkey2  == pubkey2);
        BOOST_CHECK(rkey1C == pubkey1C);
        BOOST_CHECK(rkey2C == pubkey2C);
    }
}

BOOST_AUTO_TEST_SUITE_END()
