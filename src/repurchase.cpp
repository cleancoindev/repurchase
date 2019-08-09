#include "repurchase.hpp"

using namespace rp;
using namespace eosio;

struct [[eosio::table, eosio::contract("eosio.token")]] account {
    asset balance;

    uint64_t primary_key() const { return balance.symbol.code().raw(); }
};

typedef eosio::multi_index<"accounts"_n, account> accounts_table;

void repurchase::transfer(name from, name to, asset quantity, string memo) {
    require_auth(from);
    if (!(from != _self && to == _self)) {
        return;
    }
    eosio_assert(memo.size() <= 256, "memo has more than 256 bytes");

    accounts_table acnts("eosio.token"_n, _self.value);
    auto it = acnts.find(EOS_SYMBOL.code().raw());
    check(it != acnts.end(), "Balance row already deleted or never existed. Action won't have any effect.");

//    auto eos_token = eosio::token("eosio.token"_n);
//    auto balance = eos_token.get_balance(_self/* 要查的账户名 */, EOS_SYMBOL);
    name contract_name = name(EOS_CONTRACT);
    if (it->balance.amount >= 1000) {
//        check(it->balance.amount == 0, "Cannot close because the balance is not zero." + it->balance.to_string());
        asset repurchase_asset(int64_t(it->balance.amount * 0.25), EOS_SYMBOL);
//        print("do repurchase asset balance: ", repurchase_asset.amount);
/*
        action(
                permission_level{_self, active_permission},
                contract_name,
                "transfer"_n,
                make_tuple(_self, EXCHANGE_ACCOUNT, repurchase_asset, std::string("auto transfer"))
        ).send();
*/

        asset income_asset(int64_t(it->balance.amount * 0.75), EOS_SYMBOL);
/*
        action(
                permission_level{_self, active_permission},
                contract_name,
                "transfer"_n,
                make_tuple(_self, TP_INCOME_ACCOUNT, income_asset, std::string("redirect to income account"))
        ).send();
*/
        print("do income asset %lld, repurchase asset %lld", income_asset.amount, repurchase_asset.amount);
        eosio::transaction out;
        out.actions.emplace_back(permission_level{_self, active_permission},
                                 contract_name, "transfer"_n,
                                 make_tuple(_self, EXCHANGE_ACCOUNT, repurchase_asset, std::string("{\"type\":\"buy-market\",\"symbol\":\"eosiotptoken-tpt-eos\",\"price\":\"0.0000\",\"channel\":\"dapp\",\"ref\":\"tokenpocket\"}"))
        );
        out.actions.emplace_back(permission_level{_self, active_permission},
                                 contract_name, "transfer"_n,
                                 make_tuple(_self, TP_INCOME_ACCOUNT, income_asset,
                                            std::string("redirect to income account"))
        );
        out.delay_sec = delay_sec;
        out.send(_self.value, _self, false);
    }
    return;
}