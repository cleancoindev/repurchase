#ifndef WORK_REPURCHASE_H
#define WORK_REPURCHASE_H

#include <eosiolib/eosio.hpp>
#include <eosiolib/name.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/crypto.hpp>
#include <eosiolib/types.h>
#include <eosio.system/eosio.system.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/permission.hpp>
#include <iterator>
#include <cstring>
#include <eosio.token/eosio.token.hpp>

#define TEST 1

#if TEST
#define EXCHANGE_ACCOUNT "ptokenpocket"_n
#define TP_INCOME_ACCOUNT "ltokenpocket"_n
#else
#define EXCHANGE_ACCOUNT "newdexpublic"_n
#define TP_INCOME_ACCOUNT "eosiotp.bp"_n
#endif

#define EOS_CONTRACT "eosio.token"
#define TPT_CONTRACT "eosiotptoken"

#define EOS_SYMBOL symbol(symbol_code("EOS"),4)

using namespace eosio;
using namespace std;

namespace rp {

    using namespace eosio;
    using eosio::name;
    using eosio::contract;
    using eosio::asset;
    using eosio::symbol_code;
    using std::string;
    using std::vector;
    using eosio::permission_level;
    using eosio::public_key;
    using eosiosystem::wait_weight;
    using eosiosystem::key_weight;
    using eosiosystem::permission_level_weight;
    using eosiosystem::authority;

    class [[eosio::contract]] repurchase : public eosio::contract {
    public:

        using eosio::contract::contract;

        repurchase(name s, name code, eosio::datastream<const char *> ds) : contract(s, code, ds) {}

        [[eosio::action]]
        void transfer(name from, name to, asset quantity, string memo);

    private:
        uint32_t delay_sec = 1;
        static constexpr eosio::name active_permission{"active"_n};
    };
}

#define EOSIO_DISPATCH_EX(TYPE, MEMBERS) \
            extern "C" { \
                void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
                    auto self = receiver; \
                    if( action == eosio::name("onerror").value) { \
                        /* onerror is only valid if it is for the "enumivo" code account and authorized by "eosio"'s "active permission */ \
                        eosio_assert(code == eosio::name("eosio").value, "onerror action's are only valid from the \"eosio\" system account"); \
                    } \
                    if((code == self && action != eosio::name("transfer").value) || action == eosio::name("onerror").value || ((action == eosio::name("transfer").value) && (code == eosio::name(TPT_CONTRACT).value || code == eosio::name(EOS_CONTRACT).value))) { \
                            switch( action ) { \
                            EOSIO_DISPATCH_HELPER( TYPE, MEMBERS ) \
                        } \
                    } \
                } \
            } \

EOSIO_DISPATCH_EX(rp::repurchase, (transfer))

#endif //WORK_REPURCHASE_H
