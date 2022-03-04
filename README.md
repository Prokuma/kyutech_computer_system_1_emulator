# 九州工業大学情報工学部計算機システムIエミュレータ

## Compile
```
make emulator
make assembler
```

## Command
```
./assembler test.asm test.bin
./emulator test.bin 10(Step)
```

## Assembly
- movi *im* 即値の代入(mov immediate)
- addi *im* 即値の加算(add immediate)
- add *\$reg* レジスタ値の加算(add)
- movfmd *addr* メモリを直接参照し加算(move from memory directly)
- movfmi *addr* メモリを間接参照し加算(move from memory indirectly)
- movfmr *\$reg*, *offset* メモリをインデックスレジスタ修飾で参照し加算(move from memory by using register)
- com *\$reg* 2の補数に変換(complement)
- not *\$reg* ビット反転(not)
- or *\$reg* 論理和(or)
- and *\$reg* 論理積(and)
- mone *\$reg* レジスタに-1を代入(minus one)
- movr *\$org*, *\$dst* レジスタの値移動(move to register)
- movtm *addr* メモリへの書き込み(move to memory)
- jmpz *addr* ZFが1の場合ジャンプ(jump if zero)
- jmpnz *addr* ZFが0の場合ジャンプ(jump if not zero)
- jmps *addr* SFが1の場合ジャンプ(jump if sign)
