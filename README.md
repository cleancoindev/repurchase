# repurchase编译部署

## 依赖环境

1. eosio.cdt --- branch: release/1.5.x 或以上
2. eosio.contract --- tag:  v1.5.2 或以上

## 编译

#### 1. 将repurchase目录放入eosio.contracts/contract目录内。
#### 2. 编辑文件 eosio.contracts/CMakeLists.txt:

```
add_subdirectory(repurchase)
```
#### 3. 运行eosio.contracts/build.sh完成编译
 ```
 ./build.sh
 ```
 
 ## 部署
 
 ```
 cd build
 cleos set contract itokenpocket ./repurchase -p itokenpocket
 ```


# repurchase测试

## 转账

```
cleos -u 'https://kylin.eoscanada.com' transfer ptokenpocket itokenpocket '2.0000 EOS' ''

```
