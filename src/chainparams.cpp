// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
//Copyright (c) 2015-2020 The PIVX developers
//Copyright (c) 2020 The BinJit developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "chainparamsseeds.h"
#include "consensus/merkle.h"
#include "util.h"
#include "utilstrencodings.h"

#include <boost/assign/list_of.hpp>

#include <assert.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.nVersion = nVersion;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of the genesis coinbase cannot
 * be spent as it did not originally exist in the database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "BNJ - this is cryptocurrency and the main fuel of the P2P platform";
    const CScript genesisOutputScript = CScript() << ParseHex("04c10e83b2703ccf322f7dba62dd5855ac4c10bd015814ce121ba3260b2573b8810c02c0582aed05b4deb9c4b77b26d92428c61256cd44774babea0c073b2ed0c9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256S("000008e754a94f82f40dbbd7623a0b2c243eb2bb578558d0f2c98ccc0d692b7c"))
    ; 

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    0, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    0        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of
    (0, uint256S("0x001"))
 ;
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    0,
    0,
    0};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256S("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    0,
    0,
    0};


class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";

        
        genesis = CreateGenesisBlock(1606883210, 9729726, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000008e754a94f82f40dbbd7623a0b2c243eb2bb578558d0f2c98ccc0d692b7c"));
        assert(genesis.hashMerkleRoot == uint256S("0xa91592f613a49f324f35215e035c1ed34a2772449a3e395bbac881eacc067283"));
 
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.powLimit   = ~UINT256_ZERO >> 20;   // BinJit starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 43200;       // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations = 6;      // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity = 6;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 20;       // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 25000000 * COIN;
        consensus.nPoolMaxTransactions = 3;
        consensus.nProposalEstablishmentTime = 60 * 60 * 24;    // must be at least a day old to make it into a budget
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 200;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;

        // spork keys
        consensus.strSporkPubKey = "047be59a2773b2efdca718ea8318991049b3b5380fd5232f6fbfec481e3558590c651971ad2d941b29e1b31672087d069b276cd4cb11dc3d620f6190b746df9a04";
        consensus.strSporkPubKeyOld = "04D64D2442D72B7FF2705F4CC474A66527102219ED51AB7764D8791D30F5F962AFC6F1AF763AD1008625434FD7597B23533AE803C4BDA497C86941B5DFAD1D3AF9";
        consensus.nTime_EnforceNewSporkKey = 1610087400;    //!> Monday, 29 June 2020 10:00:00 AM GMT
        consensus.nTime_RejectOldSporkKey = 1593475200;     //!> Tuesday, 30 June 2020 12:00:00 AM GMT

        // height-based activations
        consensus.height_last_PoW = 350;
        consensus.height_start_BIP65 = 570;
        consensus.height_start_MessSignaturesV2 = 470;  // height_start_TimeProtoV2
        consensus.height_start_StakeModifierNewSelection = 1;
        consensus.height_start_StakeModifierV2 = 350;
        consensus.height_start_TimeProtoV2 = 470;

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_PURPLE_FENIX].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x4a;
        pchMessageStart[1] = 0xb6;
        pchMessageStart[2] = 0x6e;
        pchMessageStart[3] = 0x9a;
        nDefaultPort = 8018; //8018

        // Note that of those with the service bits flag, most only support a subset of possible options

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 25);	// starting with 'b'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 13);
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 60);     // starting with 'R'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }

};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";

        //genesis = CreateGenesisBlock(1454124731, 499337, 0x1e0ffff0, 1, 250 * COIN);
        
        //consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == uint256S("0x0000058019ef55f1f5d7152f7b57d7ce9a4c4f66bb164b232b694a3fc9260834"));
        //assert(genesis.hashMerkleRoot == uint256S("0x43dfcdbe8d459df8813b70ce5868bd0908ecf0b746d9918172f1b5f4a974c63f"));
	genesis = CreateGenesisBlock(1606883210, 9729726, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000008e754a94f82f40dbbd7623a0b2c243eb2bb578558d0f2c98ccc0d692b7c"));
        assert(genesis.hashMerkleRoot == uint256S("0xa91592f613a49f324f35215e035c1ed34a2772449a3e395bbac881eacc067283"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~UINT256_ZERO >> 20;   // BinJit starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 144;         // approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3;      // (only 8-blocks window for finalization on testnet)
        consensus.nCoinbaseMaturity = 15;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 4;        // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 43199500 * COIN;
        consensus.nPoolMaxTransactions = 2;
        consensus.nProposalEstablishmentTime = 60 * 5;  // at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 100;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;

        // spork keys
        consensus.strSporkPubKey = "04E88BB455E2A04E65FCC41D88CD367E9CCE1F5A409BE94D8C2B4B35D223DED9C8E2F4E061349BA3A38839282508066B6DC4DB72DD432AC4067991E6BF20176127";
        consensus.strSporkPubKeyOld = "04A8B319388C0F8588D238B9941DC26B26D3F9465266B368A051C5C100F79306A557780101FE2192FE170D7E6DEFDCBEE4C8D533396389C0DAFFDBC842B002243C";
        consensus.nTime_EnforceNewSporkKey = 1566860400;    //!> August 26, 2019 11:00:00 PM GMT
        consensus.nTime_RejectOldSporkKey = 1569538800;     //!> September 26, 2019 11:00:00 PM GMT

        // height based activations
        consensus.height_last_PoW = 200;
        consensus.height_start_BIP65 = 851019;                  // Block v5: d1ec8838ba8f644e78dd4f8e861d31e75457dfe607b31deade30e806b5f46c1c
        consensus.height_start_MessSignaturesV2 = 1347000;      // height_start_TimeProtoV2
        consensus.height_start_StakeModifierNewSelection = 51197;
        consensus.height_start_StakeModifierV2 = 1214000;       // Block v6: 1822577176173752aea33d1f60607cefe9e0b1c54ebaa77eb40201a385506199
        consensus.height_start_TimeProtoV2 = 1347000;           // Block v7: 30c173ffc09a13f288bf6e828216107037ce5b79536b1cebd750a014f4939882

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_PURPLE_FENIX].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        
        pchMessageStart[0] = 0xa3;
        pchMessageStart[1] = 0x1c;
        pchMessageStart[2] = 0xcb;
        pchMessageStart[3] = 0x5d;
        nDefaultPort = 51474;

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet binjit addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet binjit script addresses start with '8' or '9'
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 73);     // starting with 'W'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet binjit BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet binjit BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet binjit BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";

        //genesis = CreateGenesisBlock(1454124731, 499337, 0x1e0ffff0, 1, 250 * COIN);
        
        //consensus.hashGenesisBlock = genesis.GetHash();
        //assert(consensus.hashGenesisBlock == uint256S("0x0000058019ef55f1f5d7152f7b57d7ce9a4c4f66bb164b232b694a3fc9260834"));
        //assert(genesis.hashMerkleRoot == uint256S("0x43dfcdbe8d459df8813b70ce5868bd0908ecf0b746d9918172f1b5f4a974c63f"));

	genesis = CreateGenesisBlock(1606883210, 9729726, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000008e754a94f82f40dbbd7623a0b2c243eb2bb578558d0f2c98ccc0d692b7c"));
        assert(genesis.hashMerkleRoot == uint256S("0xa91592f613a49f324f35215e035c1ed34a2772449a3e395bbac881eacc067283"));


        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~UINT256_ZERO >> 20;   // BinJit starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 144;         // approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3;      // (only 8-blocks window for finalization on regtest)
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 4;        // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 43199500 * COIN;
        consensus.nPoolMaxTransactions = 2;
        consensus.nProposalEstablishmentTime = 60 * 5;  // at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 0;
        consensus.nStakeMinDepth = 2;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;

        /* Spork Key for RegTest:
        WIF private key: 932HEevBSujW2ud7RfB1YF91AFygbBRQj3de3LyaCRqNzKKgWXi
        private key hex: bd4960dcbd9e7f2223f24e7164ecb6f1fe96fc3a416f5d3a830ba5720c84b8ca
        Address: yCvUVd72w7xpimf981m114FSFbmAmne7j9
        */
        consensus.strSporkPubKey = "043969b1b0e6f327de37f297a015d37e2235eaaeeb3933deecd8162c075cee0207b13537618bde640879606001a8136091c62ec272dd0133424a178704e6e75bb7";
        consensus.strSporkPubKeyOld = "";
        consensus.nTime_EnforceNewSporkKey = 0;
        consensus.nTime_RejectOldSporkKey = 0;

        // height based activations
        consensus.height_last_PoW = 250;
        consensus.height_start_BIP65 = 851019;              // Not defined for regtest. Inherit TestNet value.
        consensus.height_start_MessSignaturesV2 = 1;
        consensus.height_start_StakeModifierNewSelection = 0;
        consensus.height_start_StakeModifierV2 = 251;       // start with modifier V2 on regtest
        consensus.height_start_TimeProtoV2 = 999999999;

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_PURPLE_FENIX].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */

        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;
        nDefaultPort = 51476;

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }

    void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
    {
        assert(idx > Consensus::BASE_NETWORK && idx < Consensus::MAX_NETWORK_UPGRADES);
        consensus.vUpgrades[idx].nActivationHeight = nActivationHeight;
    }
};
static CRegTestParams regTestParams;

static CChainParams* pCurrentParams = 0;

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}

void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    regTestParams.UpdateNetworkUpgradeParameters(idx, nActivationHeight);
}
